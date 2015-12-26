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

#ifndef FIGHT_PROJECTILE_H
#define FIGHT_PROJECTILE_H


#include "effect.h"
#include "fight/collisionmanager.h"


namespace fight {


class Projectile : public Effect
{
public:
    Projectile(Scenario *scenario, Billboard *billboard);

public:
    void setPosition(const Vector3D &pos);
    void setDirection(const Vector3D &direction);
    bool update(float elapsedTime);

private:
    Vector3D m_originPos;
    Vector3D m_direction;
    CollisionCache m_collisionCache;
};


} // namespace fight


#endif // FIGHT_PROJECTILE_H
