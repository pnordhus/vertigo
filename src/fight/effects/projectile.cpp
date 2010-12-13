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
#include "effectmanager.h"
#include "../surface/surface.h"
#include "../collisionmanager.h"


namespace fight {


Projectile::Projectile(Scenario *scenario, Billboard *billboard) : 
    Effect(scenario, billboard, 0, 2)
{
}


void Projectile::setPosition(const QVector3D &pos)
{
    m_originPos = pos;
    Object::setPosition(pos);
}

void Projectile::setDirection(const QVector3D &direction)
{
    m_direction = direction;
}


void Projectile::update()
{
    m_elapsedTime = m_time.elapsed();
    if (m_elapsedTime == 0)
        return;
    if (m_elapsedTime*m_billboard->velocity()/1000 > m_billboard->range())
    {
        disable();
        return;
    }

    QVector3D newPos = m_originPos + m_direction*m_elapsedTime*m_billboard->velocity()/1000;

    QVector3D pos, normal;
    if (m_scenario->surface()->testCollision(m_position, newPos, m_billboard->collisionRadius(), pos, normal))
    {
        m_scenario->effectManager()->addEffect(Explosion_12, pos);
        disable();
    }
    Object *collision = m_scenario->collisionManager()->testCollision(this, newPos, m_billboard->collisionRadius(), pos, normal);
    if (collision)
    {
        if (collision->type() == TrashObject)
            collision->destroy();
        else
            m_scenario->effectManager()->addEffect(Explosion_11, pos, qrand()%360);
        disable();
    }

    m_position = newPos;
}


} // namespace fight
