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
    m_movie(NULL)
{
    Q_ASSERT(m_singleton == NULL);
    m_singleton = this;
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
    delete m_desktop;
    delete m_movie;
    m_movie = NULL;

    txt::DesFile file(QString("dat:story/ch%1.des").arg(chapter));

    file.beginGroup("Chapter");
    m_code = file.value("Code").toInt();
    m_area = new Area(file.value("Area").toString());
    m_stations = m_area->stations();
    const QString startStation = file.value("StartStation").toString();
    file.endGroup();

    setStation(startStation);
}


void Chapter::setStation(const QString &station)
{
    if (m_desktop)
        m_desktop->deleteLater();
    m_currentStation = station;
    m_desktop = new Desktop(m_currentStation);

    QString approachMovie = m_desktop->approachMovie();
    m_movies << approachMovie;
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


} // namespace game
