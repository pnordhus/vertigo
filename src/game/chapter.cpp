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
#include <QSettings>


namespace game {


Chapter *Chapter::m_singleton = NULL;


Chapter::Chapter() :
    m_area(NULL),
    m_desktop(NULL),
    m_movie(NULL),
    m_currentStation(-1),
    m_credits(0)
{
    Q_ASSERT(m_singleton == NULL);
    m_singleton = this;

    QSettings s;
    s.beginGroup("Movie");
    m_movieAutopilot = s.value("autopilot", true).toBool();
    m_movieApproach = s.value("approach", true).toBool();
    m_movieHarbour = s.value("harbour", true).toBool();
    s.endGroup();
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
    m_singleton = NULL;
    delete m_movie;
    delete m_desktop;
    delete m_area;
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
    }

    file.setSection("pendingdialogues");
    foreach (const QString &key, file.keys().filter(QRegExp("^dialogue\\d*")))
        addDialog(file.value(key).toInt());

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

    file.setSection("messageboard");
    file.setValue("clearboard", "yes");
    i = 1;
    foreach (int message, m_messages) {
        file.setValue(QString("message%1").arg(i++), message);
    }

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

    if (m_movieHarbour && previousStation >= 0)
        m_movies << QString("gfx:mvi/film/%1hf.mvi").arg("hiob");

    if (m_approachMovieReplacement.contains(m_currentStation)) {
        const QString movie = m_approachMovieReplacement.take(m_currentStation);
        QRegExp reg("d(\\d*)\\.mvi");
        if (reg.indexIn(movie) >= 0)
            m_playedMovies << reg.cap(1).toInt();
        m_movies << "gfx:mvi/film/" + movie;
    } else {
        if (m_movieAutopilot)
            m_movies << QString("gfx:mvi/film/%1fl.mvi").arg("hiob");
        if (m_movieApproach)
            m_movies << m_desktop->approachMovie();
    }

    playMovies();
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
    m_tasks.append(task);
}


void Chapter::removeTask(int task)
{
    m_tasks.removeAll(task);
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
    qDebug() << "Add mission" << mission << station;
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


} // namespace game
