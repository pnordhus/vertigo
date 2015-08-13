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

#include "trash.h"
#include "billboard.h"
#include "fight/scenario.h"

#include <glm/gtx/norm.hpp>


namespace fight {


Effects Trash::trashCollection[9] = { Effects::Trash_0, Effects::Trash_0, Effects::Trash_1, Effects::Trash_1, Effects::Trash_2, Effects::Trash_2, Effects::Trash_3, Effects::Trash_3, Effects::Trash_4 };


Trash::Trash(Scenario *scenario, Billboard *billboard, const glm::vec3 &position) :
    Effect(scenario, billboard, static_cast<float>(qrand()%360), 1)
{
    m_iff = 0;
    m_noise = billboard->noiseLevel();

    m_scenario->collisionManager().addObject(this);
    m_box = m_billboard->box();

    glm::vec3 pos = position + glm::vec3(qrand()%50 - 25, qrand()%50 - 25, qrand()%25 - 25);
    float height = m_scenario->surface().heightAt(pos.x, pos.y) + 5;
    if (pos.z < height)
        pos.z = height;
    Object::setPosition(pos);
}


bool Trash::intersect(const glm::vec3 &start, const glm::vec3 &dir, float radius, float &distance, glm::vec3 &normal)
{
    return m_billboard->intersect(start - m_position, dir, distance);
}


void Trash::destroy()
{
    m_scenario->effectManager().addEffect(Effects::Explosion_5, m_position, 0, glm::length2(m_box.dim()) > 15 ? 2 : 1);
    ActiveObject::destroy();
}


} // namespace fight
