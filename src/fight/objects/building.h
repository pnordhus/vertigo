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

#ifndef FIGHT_BUILDING_H
#define FIGHT_BUILDING_H


#include "object.h"


namespace fight {


class Surface;
class Module;


class Building : public Object
{
public:
    Building(Scenario *scenario, txt::DesFile &file, int size, float angle, int x, int y, int refx, int refy);

public:
    void draw();
    bool intersect(const Vector3D &start, const Vector3D &dir, float radius, float &distance, Vector3D &normal);

private:
    struct Cluster
    {
        Module *module;
        Vector3D offset;
        float scale;
        float angle;
        Matrix transform;
        Matrix invTransform;
    };

    std::vector<Cluster> m_clusters;
    int m_size;
    float m_angle;
};


} // namespace fight


#endif // FIGHT_BUILDING_H
