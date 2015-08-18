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
#include "effect.h"
#include "projectile.h"
#include "fight/scenario.h"


namespace fight {


EffectManager::EffectManager(Scenario *scenario) :
    m_scenario(scenario)
{
    txt::DesFile file;
    
    file.load("vfx:sobjects/explosio.des");
    for (int i = 0; i < 27; i++)
        m_billboards.emplace(std::piecewise_construct,
            std::forward_as_tuple((Effects)((int)Effects::Explosion_0 + i)),
            std::forward_as_tuple(m_scenario->textureManager(), file, i));
    file.load("vfx:sobjects/shoot.des");
    for (int i = 0; i < 9; i++)
        m_billboards.emplace(std::piecewise_construct,
            std::forward_as_tuple((Effects)((int)Effects::Shoot_0 + i)),
            std::forward_as_tuple(m_scenario->textureManager(), file, i));
    file.load("vfx:sobjects/debris.des");
    for (int i = 0; i < 23; i++)
        m_billboards.emplace(std::piecewise_construct,
            std::forward_as_tuple((Effects)((int)Effects::Debris_0 + i)),
            std::forward_as_tuple(m_scenario->textureManager(), file, i));
    file.load("vfx:sobjects/trash.des");
    for (int i = 0; i < 5; i++)
        m_billboards.emplace(std::piecewise_construct,
            std::forward_as_tuple((Effects)((int)Effects::Trash_0 + i)),
            std::forward_as_tuple(m_scenario->textureManager(), file, i));
    file.load("vfx:sobjects/bubble.des");
    for (int i = 0; i < 3; i++)
        m_billboards.emplace(std::piecewise_construct,
            std::forward_as_tuple((Effects)((int)Effects::Bubble_0 + i)),
            std::forward_as_tuple(m_scenario->textureManager(), file, i));
}


Billboard* EffectManager::getBillboard(Effects effect)
{
    return &m_billboards.at(effect);
}


Effect* EffectManager::addEffect(Effects effect, const Vector3D &position, float angle, float scale)
{
    Effect *object = new Effect(m_scenario, getBillboard(effect), angle, scale);
    m_effects.emplace_back(object);
    object->setPosition(position);
    return object;
}


Projectile* EffectManager::addProjectile(Effects projectile, const Vector3D &position, const Vector3D &direction)
{
    Projectile *object = new Projectile(m_scenario, getBillboard(projectile));
    m_effects.emplace_back(object);
    object->setPosition(position);
    object->setDirection(direction);
    return object;
}


void EffectManager::update(float elapsedTime)
{
    m_effects.remove_if([elapsedTime](const std::unique_ptr<Effect> &effect) { return effect->update(elapsedTime); });
}


void EffectManager::draw()
{
    for (const auto &effect : m_effects)
        effect->draw();
}


} // namespace fight
