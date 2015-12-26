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

#ifndef FIGHT_SIMPLEOBJECT_H
#define FIGHT_SIMPLEOBJECT_H


#include "activeobject.h"


namespace fight {


class Scenario;
class Module;


class SimpleObject : public ActiveObject
{
public:
    SimpleObject(Scenario *scenario, txt::DesFile &file, const ObjectInfo &info, float scale = 1/32.0f);

public:

public:
    void draw();
    bool intersect(const Vector3D &start, const Vector3D &dir, float radius, float &distance, Vector3D &normal);

protected:
    Module *m_base;
    float m_scale;
};


} // namespace fight


#endif // FIGHT_SIMPLEOBJECT_H
