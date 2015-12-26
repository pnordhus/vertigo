/***************************************************************************
 *  Copyright (C) 2010  Philipp Nordhus                                    *
 *                                                                         *
 *  This program is free software: you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation, either version 3 of the License, or      *
 *  (at your option) any later version.                                    *
 *                                                                         *
 *  This program is distributed in the hope that it will be useful,        *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *  GNU General Public License for more details.                           *
 *                                                                         *
 *  You should have received a copy of the GNU General Public License      *
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.  *
 ***************************************************************************/

#include "chapter.h"
#include "items.h"
#include "fight/scenario.h"
#include "hud/hud.h"
#include "sfx/soundsystem.h"
#include "txt/desfile.h"
#include <QDesktopServices>
#include <QDir>
#include <QSettings>


namespace game {


Chapter *Chapter::m_singleton = NULL;

static QString dataLocation()
{
#if QT_VERSION >= 0x050000
    return QStandardPaths::writableLocation(QStandardPaths::DataLocation);
#else
    return QDesktopServices::storageLocation(QDesktopServices::DataLocation);
#endif
}

Chapter::Chapter(const QString &name, std::function<void(Renderer*)> funcSetRenderer, std::function<void()> funcEndGame) :
    m_code(-1),
    m_area(NULL),
    m_desktop(NULL),
    m_briefing(NULL),
    m_movie(NULL),
    m_currentStation(-1),
    m_credits(0),
    m_mission(NULL),
    m_scenario(NULL),
    m_HUD(new hud::HUD()),
    m_boat(NULL),
    m_end(false),
    m_name(name),
    m_funcSetRenderer(std::move(funcSetRenderer)),
    m_funcEndGame(std::move(funcEndGame))
{
    Q_ASSERT(m_singleton == NULL);
    m_singleton = this;

    QSettings s;
    s.beginGroup("Movie");
    m_movieAutopilot = s.value("autopilot", true).toBool();
    m_movieApproach = s.value("approach", true).toBool();
    m_movieHarbour = s.value("harbour", true).toBool();
    s.endGroup();

    m_tasksFile.load("dat:story/tasks.des");

    m_HUD->eventSuccess() += [this]() { finishMission(); };
}


Chapter::~Chapter()
{
    QSettings s;
    s.beginGroup("Movie");
    s.setValue("autopilot", m_movieAutopilot);
    s.setValue("approach", m_movieApproach);
    s.setValue("harbour", m_movieHarbour);
    s.endGroup();

    qDeleteAll(m_pendingDialogues);
    qDeleteAll(m_missions);
    delete m_scenario;
    delete m_mission;
    delete m_boat;
    delete m_movie;
    delete m_briefing;
    delete m_desktop;
    delete m_area;
    m_singleton = NULL;
}



void Chapter::loadChapter(int chapter)
{
    if (m_code != -1) {
        m_tasksFile.setSection(QString("localtask_%1").arg(m_code));
        foreach (const QString &key, m_tasksFile.keys().filter(QRegExp("^task\\d*")))
            m_runningTasks.removeAll(m_tasksFile.value(key).toInt());
    }

    load(QString("dat:story/ch%1.des").arg(chapter), false);
}


void Chapter::load(const QString &filename, bool load)
{
    m_movies = std::queue<Movie, std::list<Movie>>();
    delete m_area;
    if (m_desktop)
        m_desktop->deleteLater();
    delete m_movie;
    m_movie = NULL;
    m_currentStation = -1;
    m_approachMovieReplacement.clear();
    qDeleteAll(m_pendingDialogues);
    m_pendingDialogues.clear();
    qDeleteAll(m_missions);
    m_missions.clear();
    delete m_mission;
    m_mission = NULL;
    m_save = false;

    txt::DesFile file(filename);

    if (file.contains("credit"))
        m_credits = file.value("credit").toInt();

    file.setSection("chapter");
    m_code = file.value("code").toInt();
    m_area = new Area(file.value("area").toString());
    m_stations = m_area->stations();
    const QString startStation = file.value("startstation").toString();

    int startStationIndex = 0;
    foreach (const Station &station, m_area->stations()) {
        file.setSection(QString("station%1").arg(station.index()));
        if (file.contains("approachmoviereplacement"))
            replaceApproachMovie(station.index(), file.value("approachmoviereplacement").toString());

        if (station.shortName() == startStation)
            startStationIndex = station.index();

        foreach (const QString &key, file.keys().filter(QRegExp("^mission\\d*")))
            addMission(file.value(key).toString(), station.index());
    }

    file.setSection("pendingdialogues");
    foreach (const QString &key, file.keys().filter(QRegExp("^dialogue\\d*")))
        addDialog(file.value(key).toInt());

    file.setSection("successmissions");
    if (file.value("clearmissions").toString().toLower() == "yes")
        m_successfulMissions.clear();
    foreach (const QString &key, file.keys().filter(QRegExp("^mission\\d*")))
        m_successfulMissions.append(file.value(key).toString());

    file.setSection(QString("area%1").arg(m_area->code()));
    foreach (const QString &key, file.keys().filter(QRegExp("^mission\\d*")))
        addMission(file.value(key).toString(), -1);

    file.setSection("runningtasks");
    if (file.value("cleartasks").toString().toLower() == "yes")
        m_runningTasks.clear();
    foreach (const QString &key, file.keys().filter(QRegExp("^task\\d*")))
        addTask(file.value(key).toInt());

    file.setSection("messageboard");
    if (file.value("clearboard").toString().toLower() == "yes")
        m_messages.clear();
    foreach (const QString &key, file.keys().filter(QRegExp("^message\\d*")))
        addMessage(file.value(key).toInt());

    file.setSection("disabledstations");
    foreach (const QString &key, file.keys().filter(QRegExp("^station\\d*")))
        disableStation(file.value(key).toInt());

    file.setSection("statistics");
    m_numSmallTalks = file.value("numofsmalltalkssincelaststory", 0).toInt();

    file.setSection("movies");
    foreach (const QString &key, file.keys().filter(QRegExp("^movie\\d*")))
        m_playedMovies << file.value(key).toInt();

    file.setSection("Ship");
    if (file.contains("type"))
    {
        delete m_boat;
        m_boat = new Boat(file);
        m_HUD->load(m_boat);
    }

    // the intro has always been played
    m_playedMovies << 2;

    setStation(startStationIndex, load);
}


void Chapter::save() const
{
    txt::DesFile file;

    file.setValue("name", m_desktop->name());
    file.setValue("time", QDateTime::currentDateTime());
    file.setValue("credit", m_credits);

    file.setSection("chapter");
    file.setValue("code", m_code);
    file.setValue("area", m_area->shortName());
    file.setValue("startstation", m_currentStationName);

    file.setSection("statistics");
    file.setValue("numofsmalltalkssincelaststory", m_numSmallTalks);
    int i = 1;
    file.setSection("pendingdialogues");
    foreach (int dialog, m_pendingDialogues.keys()) {
        file.setValue(QString("dialogue%1").arg(i++), dialog);
    }

    file.setSection("runningtasks");
    file.setValue("cleartasks", "yes");
    i = 1;
    foreach (int task, m_runningTasks)
        file.setValue(QString("task%1").arg(i++), task);

    file.setSection("messageboard");
    file.setValue("clearboard", "yes");
    i = 1;
    foreach (int message, m_messages)
        file.setValue(QString("message%1").arg(i++), message);

    file.setSection("successmissions");
    file.setValue("clearmissions", "yes");
    i = 1;
    foreach (const QString &mission, m_successfulMissions)
        file.setValue(QString("mission%1").arg(i++), mission);

    QMapIterator<int, QString> it(m_approachMovieReplacement);
    while (it.hasNext()) {
        it.next();
        file.setSection(QString("station%1").arg(it.key()));
        file.setValue("approachmoviereplacement", it.value());
    }

    file.setSection("disabledstations");
    i = 0;
    foreach (const Station &station, m_stations) {
        if (!station.isEnabled())
            file.setValue(QString("station%1").arg(i++), station.index());
    }

    i = 1;
    foreach (const Mission *mission, m_missions) {
        if (mission->station() == -1) {
            file.setSection(QString("area%1").arg(m_area->code()));
            file.setValue(QString("mission%1").arg(i++), mission->shortName());
        } else {
            file.setSection(QString("station%1").arg(mission->station()));
            file.setValue(QString("mission%1").arg(i++), mission->shortName()); // TODO: Mission index
        }
    }

    file.setSection("movies");
    i = 0;
    foreach (int movie, m_playedMovies)
        file.setValue(QString("movie%1").arg(i++), movie);

    file.setSection("Ship");
    file.setValue("type", m_boat->type());

    for (i = 0; i < m_boat->mountings().size(); i++)
    {
        const Boat::Mounting &mounting = m_boat->mountings()[i];
        file.setSection(QString("ShipMounting%1").arg(i));

        std::vector<int> items = m_boat->getItems(mounting.name);
        file.setValue("side", mounting.side);
        file.setValue("type", mounting.type);
        int buzzer = 0;
        int torpedo = 0;
        for (int model : items)
        {
            Items::Item *item = Items::get(model);
            if (item->type == Items::Armor)
                file.setValue("armor", model);
            if (item->type == Items::NRSkin)
                file.setValue("nrs", 0);
            if (item->type == Items::Sensor)
                file.setValue("sensor", model);
            if (item->type == Items::Fixer)
                file.setValue("fixer", 0);
            if (item->type == Items::Buzzer)
                file.setValue(QString("buzzer%1").arg(buzzer++), 0);
            if (item->type == Items::Engine)
                file.setValue("engine", model);
            if (item->type == Items::Booster)
                file.setValue("booster", 0);
            if (item->type == Items::Silator)
                file.setValue("silator", 0);
            if (item->type == Items::Gun)
                file.setValue("model", model);
            if (item->type == Items::Magazine)
                file.setValue("model", model);
            if (item->type == Items::Torpedo)
                file.setValue(QString("torpedo%1").arg(torpedo++), model);
            if (item->type == Items::Software)
                file.setValue("software", model);
        }
    }

    file.setSection("Defect");
    file.setValue("Gun", m_boat->defects()[Boat::DefectGun]);
    file.setValue("ToMa", m_boat->defects()[Boat::DefectToMa]);
    file.setValue("FArm", m_boat->defects()[Boat::DefectFArm]);
    file.setValue("LArm", m_boat->defects()[Boat::DefectLArm]);
    file.setValue("RArm", m_boat->defects()[Boat::DefectRArm]);
    file.setValue("BArm", m_boat->defects()[Boat::DefectBArm]);
    file.setValue("Tur1", m_boat->defects()[Boat::DefectTur1]);
    file.setValue("Tur2", m_boat->defects()[Boat::DefectTur2]);

    const QString path = dataLocation();
    file.save(QString("%1/save/%2.des").arg(path, m_name));
}


void Chapter::load(const QString &name)
{
    m_name = name;
    const QString path = dataLocation();
    load(QString("%1/save/%2.des").arg(path, name), true);
}


void Chapter::setStation(int stationIndex, bool load)
{
    if (m_desktop) {
        save();
        m_desktop->deleteLater();
        m_save = true;
    }

    int previousStation = m_currentStation;
    m_currentStation = stationIndex;

    const Station station = m_stations[stationIndex];
    m_currentStationName = station.shortName();
    m_desktop = new Desktop(m_currentStationName);

    QMutableListIterator<Mission*> it(m_missions);
    while (it.hasNext()) {
        it.next();
        if (it.value()->station() == m_currentStation) {
            m_mission = it.value();
            it.remove();
            break;
        }
    }

    if (m_movieHarbour && previousStation >= 0)
        m_movies.emplace(QString("gfx:mvi/film/%1hf.mvi").arg(m_boat->moviePrefix()), Movie::MovieTransit, 
                         txt::StringTable::get(txt::Movie_Dock), 
                         QString("%1 %2 ...").arg(txt::StringTable::get(txt::Movie_Leaving)).arg(m_stations[previousStation].name()));

    if (!m_mission && !load)
        playApproach(true);

    playMovies();
}


void Chapter::startMission(const QString &name)
{
    if (m_desktop) {
        save();
        m_desktop->deleteLater();
        m_desktop = NULL;
    }

    QMutableListIterator<Mission*> it(m_missions);
    while (it.hasNext()) {
        it.next();
        if (it.value()->shortName() == name) {
            m_mission = it.value();
            it.remove();
            break;
        }
    }

    if (m_movieHarbour)
        m_movies.emplace(QString("gfx:mvi/film/%1hf.mvi").arg(m_boat->moviePrefix()), Movie::MovieTransit,
                         txt::StringTable::get(txt::Movie_Dock),
                         QString("%1 %2 ...").arg(txt::StringTable::get(txt::Movie_Leaving)).arg(m_stations[m_currentStation].name()));

    m_currentStation = -1;

    playMovies();
}


void Chapter::startMission()
{
    Q_ASSERT(m_mission);
    Q_ASSERT(m_scenario == NULL);

    if (m_briefing)
        m_briefing->deleteLater();
    m_briefing = new Briefing();
    m_briefing->eventInit() += [this]() { m_scenario = new fight::Scenario(m_mission->scenario()); };
    m_briefing->eventStart() += [this]() { startScenario(); };
    m_funcSetRenderer(m_briefing);
}


void Chapter::startScenario()
{
    Q_ASSERT(m_briefing);
    m_briefing->deleteLater();
    m_briefing = NULL;

    Q_ASSERT(m_mission);
    Q_ASSERT(m_scenario);
    m_HUD->start(m_scenario);
    m_funcSetRenderer(m_HUD.get());
}


void Chapter::finishMission()
{
    Q_ASSERT(m_scenario);
    m_scenario->deleteLater();
    m_scenario = NULL;

    m_successfulMissions.append(m_mission->shortName());

    Q_ASSERT(m_mission);
    delete m_mission;
    m_mission = NULL;
    m_save = true;

    if (m_currentStation < 0) {
        //TODO: this should be selectable from within the mission if more than one station is enabled
        foreach (const Station& station, m_stations) {
            if (station.isEnabled()) {
                setStation(station.index());
                break;
            }
        }
    } else {
        playApproach(true);
        playMovies();
    }
}


void Chapter::playApproach(bool autopilot)
{
    if (m_approachMovieReplacement.contains(m_currentStation)) {
        const QString movie = m_approachMovieReplacement.take(m_currentStation);
        QRegExp reg("d(\\d*)\\.mvi");
        if (reg.indexIn(movie) >= 0) {
            m_playedMovies << reg.cap(1).toInt();
            if (reg.cap(1).toInt() == 36)
                m_end = true;
        }
        m_movies.emplace("gfx:mvi/film/" + movie, Movie::MovieFilm);
    } else {
        if (m_movieAutopilot && autopilot)
            m_movies.emplace(QString("gfx:mvi/film/%1fl.mvi").arg(m_boat->moviePrefix()), Movie::MovieTransit,
                             txt::StringTable::get(txt::Movie_Autopilot),
                             QString("%1 %2 ...").arg(txt::StringTable::get(txt::Movie_Approaching)).arg(m_desktop->name()));
        if (m_movieApproach)
            m_movies.emplace(m_desktop->approachMovie(), Movie::MovieStation, m_desktop->name(), m_desktop->description());
    }
}


void Chapter::playMovie(int movie)
{
    m_movies.emplace(QString("gfx:mvi/film/d%1.mvi").arg(movie, 2, 10, QChar('0')), Movie::MovieFilm);
    playMovies();
}


void Chapter::playMovies()
{
    if (m_movies.empty()) {
        sfx::SoundSystem::get()->resumeAll();
        if (m_mission) {
            startMission();
        } else if (m_end) {
            m_funcEndGame();
        } else {
            if (m_save) {
                save();
                m_save = false;
            }
            m_funcSetRenderer(m_desktop);
        }
    } else {
        sfx::SoundSystem::get()->pauseAll();
        Q_ASSERT(m_movie == NULL);
        m_movie = &m_movies.front();
        m_movie->play([this]() { movieFinished(); });
        m_funcSetRenderer(m_movie);
    }
}


void Chapter::movieFinished()
{
    m_funcSetRenderer(nullptr);
    m_movie = NULL;
    m_movies.pop();
    playMovies();
}


void Chapter::quit()
{
    m_funcEndGame();
}


void Chapter::replaceApproachMovie(int station, const QString &movie)
{
    m_approachMovieReplacement.insert(station, movie);
}


void Chapter::gameOver()
{
    m_funcEndGame();
}


Dialog* Chapter::dialog(int dialogId) const
{
    return m_pendingDialogues.value(dialogId);
}


QList<Dialog*> Chapter::dialogs(int room)
{
    QList<Dialog*> list;
    foreach (Dialog *dialog, m_pendingDialogues) {
        if (dialog->matches(m_area->code(), m_currentStation, room))
            list << dialog;
    }
    return list;
}


QList<Dialog*> Chapter::dialogsEnCom(bool room)
{
    QList<Dialog*> list;
    foreach (Dialog *dialog, m_pendingDialogues) {
        if (dialog->matchesEnCom(m_area->code(), m_currentStation, room))
            list << dialog;
    }
    return list;
}


QList<Dialog*> Chapter::dialogsDirect()
{
    QList<Dialog*> list;
    foreach (Dialog *dialog, m_pendingDialogues) {
        if (dialog->matchesDirect(m_area->code(), m_currentStation))
            list << dialog;
    }
    return list;
}


void Chapter::finishDialog(int dialogId)
{
    Dialog *dialog = m_pendingDialogues.take(dialogId);
    if (dialog && dialog->isSmallTalk())
        m_numSmallTalks++;

    save();
}


void Chapter::addMessage(int message)
{
    m_messages.insert(message);
}


void Chapter::addTask(int task)
{
    m_runningTasks.append(task);
}


void Chapter::removeTask(int task)
{
    m_runningTasks.removeAll(task);
}


void Chapter::changeChapter(int chapter)
{
    loadChapter(chapter);
}


void Chapter::addDialog(int dialogId)
{
    Q_ASSERT(!m_pendingDialogues.contains(dialogId));

    Dialog *dialog = new Dialog(m_code, dialogId, nullptr,
        [this](int n) { finishDialog(n); },
        [this](int n) { addMessage(n); },
        [this](int n) { addTask(n); },
        [this](int n) { removeTask(n); },
        [this](int n) { changeChapter(n); },
        [this](int n) { addDialog(n); },
        [this](int n) { removeDialog(n); },
        [this](int n) { addCredit(n); },
        [this](int n) { enableStation(n); },
        [this](int n) { disableStation(n); },
        [this](QString mission, int station) { addMission(mission, station); },
        [this](int station, QString movie) { replaceApproachMovie(station, movie); },
        [this]() { gameOver(); }
    );
    m_pendingDialogues.insert(dialogId, dialog);
}


void Chapter::removeDialog(int dialogId)
{
    delete m_pendingDialogues.take(dialogId);
}


void Chapter::addCredit(int credit)
{
    m_credits += credit;
    if (m_credits < 0)
        m_credits = 0;
}


void Chapter::enableStation(int station)
{
    Q_ASSERT(m_stations.contains(station));
    m_stations[station].enable();
}


void Chapter::disableStation(int station)
{
    Q_ASSERT(m_stations.contains(station));
    m_stations[station].disable();
}


void Chapter::addMission(const QString &mission, int station)
{
    m_missions.append(new Mission(mission, station));
}


void Chapter::toggleMovieAutopilot()
{
    m_movieAutopilot = !m_movieAutopilot;
}


void Chapter::toggleMovieApproach()
{
    m_movieApproach = !m_movieApproach;
}


void Chapter::toggleMovieHarbour()
{
    m_movieHarbour = !m_movieHarbour;
}


void Chapter::upgradeBoat(int type)
{
    m_boat->upgrade(type);
    m_HUD->load(m_boat);
}


QList<Task> Chapter::tasks()
{
    m_tasksFile.setSection("Task");
    QList<Task> t;
    foreach (int id, m_runningTasks)
        t.append(Task(m_tasksFile.value(QString("Task_%1").arg(id)).toString()));
    return t;
}


QList<Chapter::SavedGame> Chapter::savedGames()
{
    QList<SavedGame> saves;

    const QString path = dataLocation();
    QDir dir(path + "/save");
    QRegExp reg("(.*)\\.des");
    foreach (const QString &save, dir.entryList()) {
        if (reg.indexIn(save) >= 0) {
            const QString filename = dir.filePath(save);

            txt::DesFile file(filename);

            SavedGame game;
            game.name = reg.cap(1);
            game.station = file.value("name").toString();
            game.time = file.value("time").toDateTime();
            saves << game;
        }
    }

    return saves;
}


} // namespace game
