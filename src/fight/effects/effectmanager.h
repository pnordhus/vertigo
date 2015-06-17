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

#ifndef FIGHT_EFFECTMANAGER_H
#define FIGHT_EFFECTMANAGER_H


#include "billboard.h"
#include "effect.h"
#include "projectile.h"
#include <memory>

namespace fight {


class Scenario;


class EffectManager
{
public:
    EffectManager(Scenario *scenario);

public:
    Billboard* getBillboard(Effects effect) { return &m_billboards.at(effect); }

    Effect* addEffect(Effects effect, const glm::vec3 &position, float angle = 0, float scale = 1);
    Projectile* addProjectile(Effects projectile, const glm::vec3 &position, const glm::vec3 &direction);

    void update();
    void draw();

private:
    Scenario *m_scenario;
    std::map<Effects, Billboard> m_billboards;
    std::list<std::unique_ptr<Effect>> m_effects;
};


} // namespace fight


#endif // FIGHT_EFFECTMANAGER_H
