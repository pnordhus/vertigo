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

#ifndef GAME_STATION_H
#define GAME_STATION_H


#include <QString>


namespace game {


class Station
{
public:
    Station();
    Station(int index, const QString &name);

public:
    void enable();
    void disable();
    bool isEnabled() const { return m_enabled; }
    int index() const { return m_index; }
    const QString& shortName() const { return m_shortName; }
    const QString& name() const { return m_name; }

private:
    int m_index;
    QString m_shortName;
    QString m_name;
    bool m_enabled;
};


} // namespace game


#endif // GAME_STATION_H
