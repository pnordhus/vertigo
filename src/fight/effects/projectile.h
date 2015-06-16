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


namespace fight {


class Projectile : public Effect
{
public:
    Projectile(Scenario *scenario, Billboard *billboard);

public:
    void setPosition(const glm::vec3 &pos);
    void setDirection(const glm::vec3 &direction);
    void update();

private:
    glm::vec3 m_originPos;
    glm::vec3 m_direction;
};


} // namespace fight


#endif // FIGHT_PROJECTILE_H
