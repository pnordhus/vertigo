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

#ifndef FIGHT_BUBBLE_H
#define FIGHT_BUBBLE_H


#include "effect.h"
#include "fight/collisionmanager.h"


namespace fight {


class Bubble : public Effect
{
public:
    Bubble(Scenario *scenario, Billboard *billboard);

public:
    void setPosition(const Vector3D &pos);
    bool update(float elapsedTime);

private:
    Vector3D m_originPos;
};


} // namespace fight


#endif // FIGHT_BUBBLE_H
