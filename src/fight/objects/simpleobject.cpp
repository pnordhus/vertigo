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

#include "simpleobject.h"
#include "fight/scenario.h"
#include "fight/module.h"


namespace fight {


SimpleObject::SimpleObject(Scenario *scenario, txt::DesFile &file, const ObjectInfo &info, float scale) :
    ActiveObject(scenario, file, info)
{
    file.setSection("cluster");
    m_base = scenario->moduleManager().get(file.value("base").toString());

    file.setSection("size");
    m_scale = file.value("scale").toFloat() * scale;

    m_box = m_base->box().scale(m_scale);
    scenario->collisionManager().addObject(this);
}


void SimpleObject::draw()
{
    glPushMatrix();
    glTranslatef(m_position.x, m_position.y, m_position.z);
    glScalef(m_scale, m_scale, m_scale);
    m_base->draw();
    glPopMatrix();
}


bool SimpleObject::intersect(const Vector3D &start, const Vector3D &dir, float radius, float &distance, Vector3D &normal)
{
    if (m_base->intersect((start - m_position)/m_scale, dir, radius/m_scale, distance, normal))
    {
        distance *= m_scale;
        return true;
    }
    return false;
}


} // namespace fight
