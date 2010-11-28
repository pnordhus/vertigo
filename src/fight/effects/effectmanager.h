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


#include "gfx/texturemanager.h"
#include "effect.h"


namespace fight {


enum Effects
{
    Explosion_0,
    Explosion_1,
    Explosion_2,
    Explosion_3,
    Explosion_4,
    Explosion_5,
    Explosion_6,
    Explosion_7,
    Explosion_8,
    Explosion_9,
    Explosion_10,
    Explosion_11,
    Explosion_12,
    Explosion_13,
    Explosion_14,
    Explosion_15,
    Explosion_16,
    Explosion_17,
    Explosion_18,
    Explosion_19,
    Explosion_20,
    Explosion_21,
    Explosion_22,
    Explosion_23,
    Explosion_24,
    Explosion_25,
    Explosion_26,
    Shoot_0,
    Shoot_1,
    Shoot_2,
    Shoot_3,
    Shoot_4,
    Shoot_Vendetta,
    Shoot_6,
    Shoot_7,
    Shoot_8,
    Debris_0,
    Debris_1,
    Debris_2,
    Debris_3,
    Debris_4,
    Debris_5,
    Debris_6,
    Debris_7,
    Debris_8,
    Debris_9,
    Debris_10,
    Debris_11,
    Debris_12,
    Debris_13,
    Debris_14,
    Debris_15,
    Debris_16,
    Debris_17,
    Debris_18,
    Debris_19,
    Debris_20,
    Debris_21,
    Debris_22,
    Trash_0,
    Trash_1,
    Trash_2,
    Trash_3,
    Trash_4,
    Bubble_0,
    Bubble_1,
    Bubble_2,
};


class Billboard;


class EffectManager
{
public:
    EffectManager(gfx::TextureManager &texMan);
    ~EffectManager();

public:
    Billboard* getBillboard(Effects effect) const { return m_billboards[effect]; }
    Effect* create(Effects effect);

    void addEffect(Effects effect, const QVector3D &position);
    void addProjectile(Effects effect, const QVector3D &position, const QVector3D &direction);
    void draw();

private:
    QMap<Effects, Billboard*> m_billboards;
    QList<Effect *> m_effects;
};


} // namespace fight


#endif // FIGHT_EFFECTMANAGER_H
