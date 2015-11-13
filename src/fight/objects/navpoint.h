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

#ifndef FIGHT_NAVPOINT_H
#define FIGHT_NAVPOINT_H


#include "object.h"


namespace fight {


class Module;


class NavPoint : public Object
{
public:
    NavPoint(Scenario *scenario, int num);

public:
    int num() const { return m_num; }

public:
    bool update(float elapsedTime);
    void draw();

private:
    int m_num;
    float m_scale;
    Module *m_state0;
    Module *m_state1;
    int m_state;
    float m_time;
    bool m_reached;
};


} // namespace fight


#endif // FIGHT_NAVPOINT_H
