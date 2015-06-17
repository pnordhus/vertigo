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

#ifndef FIGHT_TURRET_H
#define FIGHT_TURRET_H


#include "object.h"
#include "module.h"


namespace fight {


class Turret : public Object
{
public:
    Turret(Scenario *scenario, const QString &name);

public:
    void draw();

private:
    Module *m_armLeft;
    Module *m_armRight;
    glm::vec3 m_armLeftPosition;
    glm::vec3 m_armRightPosition;
};


} // namespace fight


#endif // FIGHT_TURRET_H
