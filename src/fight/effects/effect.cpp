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


namespace fight {


Effect::Effect(Scenario *scenario, Billboard *billboard, float angle) : 
    Object(scenario),
    m_billboard(billboard),
    m_angle(angle),
    m_permanent(false)
{
    m_time.restart();
}


void Effect::update()
{
    if (!m_permanent && m_time.elapsed() >= m_billboard->duration())
        disable();
}


void Effect::draw()
{
    m_billboard->draw(m_position, m_angle, 1, m_time.elapsed(), m_scenario->cameraMatrixInverted());
}


void Effect::setPermanent(bool permanent)
{
    m_permanent = permanent;
}


} // namespace fight
