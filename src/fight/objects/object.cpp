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

#include "object.h"
#include "fight/scenario.h"


namespace fight {


Object::Object(Scenario *scenario) :
    m_scenario(scenario),
    m_enabled(true),
    m_static(true),
    m_condEnable(scenario, this)
{
}


Object::Object(Scenario *scenario, txt::DesFile &file, float scale) :
    m_scenario(scenario),
    m_enabled(true),
    m_static(true),
    m_condEnable(scenario, this)
{
    file.setSection("offense");
    if (!file.contains("kineticstrength"))
        file.setSection("offense0");
    m_kineticStrength = file.value("kineticstrength").toInt();
    m_shockStrength = file.value("shockstrength").toInt();
}


void Object::setEnabled(bool enabled)
{
    m_enabled = enabled;
}


void Object::enable()
{
    m_enabled = true;
}


void Object::disable()
{
    m_enabled = false;
}


void Object::setPosition(const Vector3D &pos)
{
    m_box = m_box.translate(pos - m_position);
    m_position = pos;
}


bool Object::update(float elapsedTime)
{
    return false;
}


void Object::draw()
{
}


bool Object::intersect(const Vector3D &start, const Vector3D &dir, float radius, float &distance, Vector3D &normal)
{
    return false;
}


} // namespace fight
