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

#ifndef GAME_CHAPTER_H
#define GAME_CHAPTER_H


#include "area.h"
#include "desktop.h"
#include "movie.h"


namespace game {


class Chapter : public QObject
{
    Q_OBJECT

public:
    Chapter();
    ~Chapter();

signals:
    void setRenderer(Renderer*);
    void endGame();

public slots:
    void quit();

public:
    void load(int chapter);
    void setStation(int station);
    Area* area() const { return m_area; }
    const QMap<int, Station>& stations() const { return m_stations; }
    int currentStation() const { return m_currentStation; }
    void replaceApproachMovie(int station, const QString &movie);

private:
    void playMovies();

private slots:
    void movieFinished();

public:
    static Chapter* get() { Q_ASSERT(m_singleton); return m_singleton; }

private:
    int m_code;
    Area *m_area;
    Desktop *m_desktop;
    Movie *m_movie;
    QStringList m_movies;
    QMap<int, QString> m_approachMovieReplacement;
    QMap<int, Station> m_stations;
    int m_currentStation;
    QSet<int> m_playedMovies;
    static Chapter *m_singleton;
};


} // namespace game


#endif // GAME_CHAPTER_H
