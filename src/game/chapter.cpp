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
#include "sfx/soundsystem.h"
#include "txt/desfile.h"
#include <QDesktopServices>
#include <QDir>
#include <QSettings>


namespace game {


Chapter *Chapter::m_singleton = NULL;


Chapter::Chapter() :
    m_area(NULL),
    m_desktop(NULL),
    m_briefing(NULL),
    m_movie(NULL),
    m_currentStation(-1),
    m_credits(0),
    m_mission(NULL)
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
    delete m_mission;
    delete m_movie;
    delete m_briefing;
    delete m_desktop;
    delete m_area;
    m_singleton = NULL;
}



void Chapter::loadChapter(int chapter)
{
    load(QString("dat:story/ch%1.des").arg(chapter));
}


void Chapter::load(const QString &filename)
{
    m_movies.clear();
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
    if (file.value("clearmissions").toBool())
        m_successfulMissions.clear();
    foreach (const QString &key, file.keys().filter(QRegExp("^mission\\d*")))
        m_successfulMissions.append(file.value(key).toString());

    file.setSection("area2");
    foreach (const QString &key, file.keys().filter(QRegExp("^mission\\d*")))
        addMission(file.value(key).toString(), -1);

    file.setSection("runningtasks");
    if (file.value("cleartasks").toBool())
        m_runningTasks.clear();
    foreach (const QString &key, file.keys().filter(QRegExp("^task\\d*")))
        addTask(file.value(key).toInt());

    file.setSection("messageboard");
    if (file.value("clearboard").toBool())
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

    // the intro has always been played
    m_playedMovies << 2;

    setStation(startStationIndex);
}


void Chapter::save(int slot, const QString &name) const
{
    txt::DesFile file;

    file.setValue("name", name);
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
            file.setSection("area2");
            file.setValue(QString("mission%1").arg(i++), mission->shortName());
        } else {
            file.setSection(QString("station%1").arg(mission->station()));
            file.setValue("mission1", mission->shortName());
        }
    }

    file.setSection("movies");
    i = 0;
    foreach (int movie, m_playedMovies)
        file.setValue(QString("movie%1").arg(i++), movie);

    const QString path = QDesktopServices::storageLocation(QDesktopServices::DataLocation);
    file.save(QString("%1/save%2.des").arg(path).arg(slot));
}


void Chapter::load(int slot)
{
    const QString path = QDesktopServices::storageLocation(QDesktopServices::DataLocation);
    load(QString("%1/save%2.des").arg(path).arg(slot));
}


void Chapter::setStation(int stationIndex)
{
    if (m_desktop)
        m_desktop->deleteLater();

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
        m_movies << QString("gfx:mvi/film/%1hf.mvi").arg("hiob");

    if (!m_mission)
        playApproach(true);

    playMovies();
}


void Chapter::startMission(const QString &name)
{
    QMutableListIterator<Mission*> it(m_missions);
    while (it.hasNext()) {
        it.next();
        if (it.value()->shortName() == name) {
            m_mission = it.value();
            it.remove();
            break;
        }
    }

    if (m_desktop)
        m_desktop->deleteLater();
    m_desktop = NULL;
    m_currentStation = -1;

    startMission();
}


void Chapter::startMission()
{
    Q_ASSERT(m_mission);
    qDebug() << "Start mission" << m_mission->shortName();

    if (m_briefing)
        m_briefing->deleteLater();
    m_briefing = new Briefing();
    connect(m_briefing, SIGNAL(startEngine()), SLOT(finishMission()));
    emit setRenderer(m_briefing);
}

void Chapter::finishMission()
{
    if (m_briefing)
        m_briefing->deleteLater();
    m_briefing = NULL;

    m_successfulMissions.append(m_mission->shortName());
    delete m_mission;
    m_mission = NULL;

    if (m_currentStation < 0) {
        //TODO: this should be selectable from within the mission
        foreach (const Station& station, m_stations) {
            if (station.isEnabled()) {
                setStation(station.index());
                break;
            }
        }
    } else {
        playApproach(false);
        playMovies();
    }
}


void Chapter::playApproach(bool autopilot)
{
    if (m_approachMovieReplacement.contains(m_currentStation)) {
        const QString movie = m_approachMovieReplacement.take(m_currentStation);
        QRegExp reg("d(\\d*)\\.mvi");
        if (reg.indexIn(movie) >= 0)
            m_playedMovies << reg.cap(1).toInt();
        m_movies << "gfx:mvi/film/" + movie;
    } else {
        if (m_movieAutopilot && autopilot)
            m_movies << QString("gfx:mvi/film/%1fl.mvi").arg("hiob");
        if (m_movieApproach)
            m_movies << m_desktop->approachMovie();
    }
}


void Chapter::playMovie(int movie)
{
    m_movies << QString("gfx:mvi/film/d%1.mvi").arg(movie, 2, 10, QChar('0'));
    playMovies();
}


void Chapter::playMovies()
{
    if (m_movies.isEmpty()) {
        sfx::SoundSystem::get()->resumeAll();
        if (m_mission)
            startMission();
        else
            emit setRenderer(m_desktop);
    } else {
        sfx::SoundSystem::get()->pauseAll();
        Q_ASSERT(m_movie == NULL);
        m_movie = new Movie;
        connect(m_movie, SIGNAL(finished()), SLOT(movieFinished()));
        m_movie->play(m_movies.takeFirst());
        emit setRenderer(m_movie);
    }
}


void Chapter::movieFinished()
{
    emit setRenderer(NULL);
    delete m_movie;
    m_movie = NULL;
    playMovies();
}


void Chapter::quit()
{
    save(99, "autosave");
    emit endGame();
}


void Chapter::replaceApproachMovie(int station, const QString &movie)
{
    m_approachMovieReplacement.insert(station, movie);
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


void Chapter::removeDialog(int dialogId)
{
    Dialog *dialog = m_pendingDialogues.take(dialogId);
    if (dialog && dialog->isSmallTalk())
        m_numSmallTalks++;
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

    Dialog *dialog = new Dialog(dialogId);
    connect(dialog, SIGNAL(remove(int)), SLOT(removeDialog(int)));
    connect(dialog, SIGNAL(addMessage(int)), SLOT(addMessage(int)));
    connect(dialog, SIGNAL(addTask(int)), SLOT(addTask(int)));
    connect(dialog, SIGNAL(removeTask(int)), SLOT(removeTask(int)));
    connect(dialog, SIGNAL(changeChapter(int)), SLOT(changeChapter(int)));
    connect(dialog, SIGNAL(addDialog(int)), SLOT(addDialog(int)));
    connect(dialog, SIGNAL(addCredit(int)), SLOT(addCredit(int)));
    connect(dialog, SIGNAL(enableStation(int)), SLOT(enableStation(int)));
    connect(dialog, SIGNAL(disableStation(int)), SLOT(disableStation(int)));
    connect(dialog, SIGNAL(addMission(QString,int)), SLOT(addMission(QString,int)));
    connect(dialog, SIGNAL(replaceApproachMovie(int,QString)), SLOT(replaceApproachMovie(int,QString)));
    m_pendingDialogues.insert(dialogId, dialog);
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


QList<Task> Chapter::tasks()
{
    m_tasksFile.setSection("Task");
    QList<Task> t;
    foreach (int id, m_runningTasks)
        t.append(Task(m_tasksFile.value(QString("Task_%1").arg(id)).toString()));
    return t;
}


QMap<int, QString> Chapter::savedGames()
{
    QMap<int, QString> saves;

    const QString path = QDesktopServices::storageLocation(QDesktopServices::DataLocation);
    QDir dir(path);
    QRegExp reg("save(\\d*)\\.des");
    foreach (const QString &save, dir.entryList()) {
        if (reg.indexIn(save) >= 0) {
            const QString filename = dir.filePath(save);
            const int id = reg.cap(1).toInt();

            txt::DesFile file(filename);
            const QString name = file.value("name").toString();
            saves.insert(id, name);
        }
    }

    return saves;
}


} // namespace game
