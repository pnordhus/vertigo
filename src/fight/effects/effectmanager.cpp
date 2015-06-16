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

#include "effectmanager.h"
#include "billboard.h"
#include "projectile.h"
#include "trash.h"
#include "../surface/surface.h"
#include "../collisionmanager.h"


namespace fight {


EffectManager::EffectManager(Scenario *scenario) :
    m_scenario(scenario)
{
    txt::DesFile file;
    
    file.load("vfx:sobjects/explosio.des");
    for (int i = 0; i < 27; i++)
    {
        Billboard *billboard = new Billboard(m_scenario->textureManager(), file, i);
        m_billboards.insert((Effects)(Explosion_0 + i), billboard);
    }
    file.load("vfx:sobjects/shoot.des");
    for (int i = 0; i < 9; i++)
    {
        Billboard *billboard = new Billboard(m_scenario->textureManager(), file, i);
        m_billboards.insert((Effects)(Shoot_0 + i), billboard);
    }
    file.load("vfx:sobjects/debris.des");
    for (int i = 0; i < 23; i++)
    {
        Billboard *billboard = new Billboard(m_scenario->textureManager(), file, i);
        m_billboards.insert((Effects)(Debris_0 + i), billboard);
    }
    file.load("vfx:sobjects/trash.des");
    for (int i = 0; i < 5; i++)
    {
        Billboard *billboard = new Billboard(m_scenario->textureManager(), file, i);
        m_billboards.insert((Effects)(Trash_0 + i), billboard);
    }
    file.load("vfx:sobjects/bubble.des");
    for (int i = 0; i < 3; i++)
    {
        Billboard *billboard = new Billboard(m_scenario->textureManager(), file, i);
        m_billboards.insert((Effects)(Bubble_0 + i), billboard);
    }
}


EffectManager::~EffectManager()
{
    qDeleteAll(m_effects);
    qDeleteAll(m_billboards.values());
}


Effect* EffectManager::create(Effects effect, float angle, float scale)
{
    return new Effect(m_scenario, getBillboard(effect), angle, scale);
}


void EffectManager::addEffect(Effects effect, const glm::vec3 &position, float angle, float scale)
{
    Effect *object = create(effect, angle, scale);
    object->setPosition(position);
    m_effects << object;
}


void EffectManager::addProjectile(Effects effect, const glm::vec3 &position, const glm::vec3 &direction)
{
    Projectile *object = new Projectile(m_scenario, getBillboard(effect));
    object->setPosition(position);
    object->setDirection(direction);
    m_effects << object;
}


Trash *EffectManager::createTrash(Effects trash, const glm::vec3 &position)
{
    Trash *object = new Trash(m_scenario, getBillboard(trash), qrand()%360);
    
    glm::vec3 pos = position + glm::vec3(qrand()%50 - 25, qrand()%50 - 25, qrand()%25 - 25);
    float height = m_scenario->surface()->heightAt(pos.x, pos.y) + 2;
    if (pos.z < height)
        pos.z = height;
    object->setPosition(pos);

    m_scenario->collisionManager()->addObject(object);

    return object;
}


void EffectManager::update()
{
    foreach (Effect *effect, m_effects)
        effect->update();
    for (int i = 0; i < m_effects.count(); i++)
        if (!m_effects[i]->isEnabled())
        {
            delete m_effects[i];
            m_effects.removeAt(i);
            i--;
            continue;
        }
}


void EffectManager::draw()
{
    foreach (Effect *effect, m_effects)
        effect->draw();
}


} // namespace fight
