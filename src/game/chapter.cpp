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
#include "txt/desfile.h"


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

    // the intro has always been played
    m_playedMovies << 2;
}


Chapter::~Chapter()
{
    m_singleton = NULL;
    delete m_movie;
    delete m_desktop;
    delete m_area;
}


void Chapter::load(int chapter)
{
    m_movies.clear();
    delete m_area;
    if (m_desktop)
        m_desktop->deleteLater();
    delete m_movie;
    m_movie = NULL;
    m_currentStation = -1;
    m_numSmallTalks = 0;

    txt::DesFile file(QString("dat:story/ch%1.des").arg(chapter));

    file.setSection("Chapter");
    m_code = file.value("Code").toInt();
    m_area = new Area(file.value("Area").toString());
    m_stations = m_area->stations();
    const QString startStation = file.value("StartStation").toString();

    int startStationIndex = 0;
    foreach (const Station &station, m_area->stations()) {
        file.setSection(QString("Station%1").arg(station.index()));
        if (file.contains("ApproachMovieReplacement"))
            replaceApproachMovie(station.index(), file.value("ApproachMovieReplacement").toString());

        if (station.shortName() == startStation)
            startStationIndex = station.index();
    }

    file.setSection("PendingDialogues");
    foreach (const QString &key, file.keys())
        addDialog(file.value(key).toInt());

    setStation(startStationIndex);
}


void Chapter::setStation(int stationIndex)
{
    if (m_desktop)
        m_desktop->deleteLater();

    int previousStation = m_currentStation;
    m_currentStation = stationIndex;

    const Station station = m_stations[stationIndex];
    m_desktop = new Desktop(station.shortName());

    if (previousStation >= 0)
        m_movies << QString("gfx:mvi/film/%1hf.mvi").arg("hiob");

    if (m_approachMovieReplacement.contains(m_currentStation)) {
        const QString movie = m_approachMovieReplacement.take(m_currentStation);
        QRegExp reg("d(\\d*)\\.mvi");
        if (reg.indexIn(movie) >= 0)
            m_playedMovies << reg.cap(1).toInt();
        m_movies << "gfx:mvi/film/" + movie;
    } else {
        m_movies << QString("gfx:mvi/film/%1fl.mvi").arg("hiob");
        m_movies << m_desktop->approachMovie();
    }

    playMovies();
}


void Chapter::playMovies()
{
    if (m_movies.isEmpty()) {
        emit setRenderer(m_desktop);
    } else {
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
    qDebug() << "Add task" << task;
}


void Chapter::removeTask(int task)
{
    qDebug() << "Remove task" << task;
}


void Chapter::changeChapter(int chapter)
{
    load(chapter);
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
    connect(dialog, SIGNAL(replaceApproachMovie(int,QString)), SLOT(replaceApproachMovie(int,QString)));
    m_pendingDialogues.insert(dialogId, dialog);
}


void Chapter::addCredit(int credit)
{
    m_credits += credit;
    if (m_credits < 0)
        m_credits = 0;
}


} // namespace game
