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

#include "navpoint.h"


namespace fight {


NavPoint::NavPoint(ModuleManager &modMan, int num) :
    m_num(num)
{
    m_state0 = modMan.get("thumper2.mod");
    m_state1 = modMan.get("thumper1.mod");
    m_scale = 0.03;

    m_base = m_state0;
    m_state = 0;
    m_time.restart();
}


void NavPoint::draw()
{
    if (m_time.elapsed() > 500)
    {
        if (m_state == 0)
        {
            m_state = 1;
            m_base = m_state1;
        }
        else
        {
            m_state = 0;
            m_base = m_state0;
        }
        m_time.restart();
    }
    Object::draw();
}


} // namespace fight
