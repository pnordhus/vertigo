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

#ifndef GAME_AREA_H
#define GAME_AREA_H


#include "station.h"
#include <QMap>


namespace game {


class Area
{
public:
    Area();
    Area(const QString &name);

public:
    QString map() const { return m_map; }
    QMap<int, Station> stations() const { return m_stations; }

private:
    Q_DISABLE_COPY(Area);
    void load(const QString &name);

private:
    int m_code;
    QString m_name;
    QString m_map;
    QMap<int, Station> m_stations;
};


} // namespace game


#endif // GAME_AREA_H
