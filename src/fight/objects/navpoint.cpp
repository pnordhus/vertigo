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
#include "fight/scenario.h"
#include "sfx/samplemap.h"
#include <glm/geometric.hpp>


namespace fight {


NavPoint::NavPoint(Scenario *scenario, int num) :
    Object(scenario),
    m_num(num),
    m_time(0),
    m_reached(false)
{
    m_state0 = scenario->moduleManager().get("thumper2.mod");
    m_state1 = scenario->moduleManager().get("thumper1.mod");
    m_scale = 0.03;
    m_state = 0;
}


bool NavPoint::update(float elapsedTime)
{
    m_time += elapsedTime;
    while (m_time > 500.0f)
    {
        if (m_state == 0)
            m_state = 1;
        else
            m_state = 0;
        m_time -= 500.0f;
    }
    if (m_enabled && !m_reached)
    {
        glm::vec3 d = m_scenario->position() - m_position;
        if (glm::dot(d, d) < 400.0f)
        {
            sfx::SampleMap::get(sfx::Sample::NavPoint).play();
            m_reached = true;
        }
    }
    return false;
}


void NavPoint::draw()
{
    glPushMatrix();
    glTranslatef(m_position.x, m_position.y, m_position.z);
    glScalef(m_scale, m_scale, m_scale);
    if (m_state == 0)
        m_state0->draw();
    if (m_state == 1)
        m_state1->draw();
    glPopMatrix();
}


} // namespace fight
