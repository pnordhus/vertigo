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
#include "scenario.h"


namespace fight {


Object::Object(Scenario *scenario) :
    m_scenario(scenario),
    m_enabled(true),
    m_type(UnknownObject),
    m_static(true),
    m_condEnable(this)
{
}


Object::Object(Scenario *scenario, const QString &name, float scale) :
    m_scenario(scenario),
    m_enabled(true),
    m_type(UnknownObject),
    m_static(true),
    m_condEnable(this)
{
    txt::DesFile file(QString("vfx:sobjects/%1.des").arg(name));
    file.setSection("cluster");
    m_base = scenario->moduleManager().get(file.value("base").toString());

    file.setSection("size");
    m_scale = file.value("scale").toFloat() * scale;
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


void Object::setCollisionCache(CollisionCache *cache)
{
    m_collisionCache.reset(cache);
}


void Object::setPosition(const glm::vec3 &pos)
{
    m_position = pos;
}


bool Object::update()
{
    return false;
}


void Object::draw()
{
    glPushMatrix();
    glTranslatef(m_position.x, m_position.y, m_position.z);
    glScalef(m_scale, m_scale, m_scale);
    m_base->draw();
    glPopMatrix();
}


bool Object::intersect(const glm::vec3 &start, const glm::vec3 &dir, float radius, float &distance, glm::vec3 &normal)
{
    return false;
}


void Object::destroy()
{
    disable();
    m_eventDestroy.complete();
}


} // namespace fight
