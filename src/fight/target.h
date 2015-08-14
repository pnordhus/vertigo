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

#ifndef FIGHT_TARGET_H
#define FIGHT_TARGET_H


namespace fight {


class Scenario;
class ActiveObject;
class NavPoint;


class Target
{
public:
    Target(Scenario *scenario);

public:
    ActiveObject* locked() const { return m_locked; }
    NavPoint* lockedNavPoint() const { return m_lockedNavPoint; }

    void lockReset();
    void lockReticle();
    void lockNavPoint(NavPoint *navPoint);

private:
    Scenario *m_scenario;
    ActiveObject *m_locked;
    NavPoint *m_lockedNavPoint;
};


}


#endif // FIGHT_TARGET_H
