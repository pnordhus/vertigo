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

#include "effect.h"
#include "billboard.h"
#include "fight/scenario.h"


namespace fight {


Effect::Effect(Scenario *scenario, Billboard *billboard, float angle, float scale) : 
    ActiveObject(scenario),
    m_billboard(billboard),
    m_angle(angle),
    m_scale(scale),
    m_time(0),
    m_permanent(false)
{
}


bool Effect::update(float elapsedTime)
{
    m_time += elapsedTime;
    if (!m_permanent && m_time >= m_billboard->duration())
    {
        disable();
        return true;
    }
    return false;
}


void Effect::draw()
{
    m_billboard->draw(m_position - m_scenario->position(), m_angle, m_scale, m_time, m_scenario->cameraMatrixInverted());
}


void Effect::setPermanent(bool permanent)
{
    m_permanent = permanent;
}


} // namespace fight
