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

#include "projectile.h"
#include "billboard.h"
#include "fight/scenario.h"


namespace fight {


Projectile::Projectile(Scenario *scenario, Billboard *billboard) : 
    Effect(scenario, billboard, 0, 2)
{
}


void Projectile::setPosition(const glm::vec3 &pos)
{
    m_originPos = pos;
    Object::setPosition(pos);
}

void Projectile::setDirection(const glm::vec3 &direction)
{
    m_direction = direction;
}


bool Projectile::update(float elapsedTime)
{
    m_elapsedTime += elapsedTime;
    if (m_elapsedTime == 0)
        return false;
    if (m_elapsedTime*m_billboard->velocity()/1000 > m_billboard->range())
    {
        disable();
        return true;
    }

    glm::vec3 newPos = m_originPos + m_direction*(m_elapsedTime*m_billboard->velocity()/1000);

    glm::vec3 pos, normal;
    if (m_scenario->surface().testCollision(m_position, newPos, m_billboard->collisionRadius(), pos, normal))
    {
        m_scenario->effectManager().addEffect(Explosion_12, pos);
        disable();
        return true;
    }
    Object *collision = m_scenario->collisionManager().testCollision(m_position, newPos, m_billboard->collisionRadius(), pos, normal, &m_collisionCache);
    if (collision)
    {
        ActiveObject *activeObject = dynamic_cast<ActiveObject *>(collision);
        if (activeObject)
            activeObject->destroy();
        else
            m_scenario->effectManager().addEffect(Explosion_11, pos, qrand()%360);
        disable();
        return true;
    }

    m_position = newPos;
    return false;
}


} // namespace fight
