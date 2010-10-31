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

#ifndef GAME_BOAT_H
#define GAME_BOAT_H


#include <QString>


namespace game {


class Boat
{
public:
    Boat(int id);

public:
    const QString& name() const { return m_name; }
    const QString& cockpit() const { return m_cockpit; }
    const QString& flipMovie1() const { return m_flipMovie1; }
    const QString& flipMovie2() const { return m_flipMovie2; }

private:
    QString m_name;
    QString m_cockpit;
    QString m_flipMovie1;
    QString m_flipMovie2;
};


} // namespace game


#endif // GAME_BOAT_H
