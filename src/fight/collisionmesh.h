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

#ifndef FIGHT_COLLISIONMESH_H
#define FIGHT_COLLISIONMESH_H


#include <vector>
#include "util/geometry3d.h"


namespace fight {


class CollisionMesh
{
public:
    CollisionMesh();

public:
    void addTriangles(const std::vector<Vector3D> &vertices, const std::vector<unsigned short> &indices);
    bool intersect(const Vector3D &start, const Vector3D &dir, float radius, float &distance, Vector3D &normal);

    static bool intersectSphereLine(const Vector3D &point, const Vector3D &dir, float radiusSquared, float &t);

private:
    struct Triangle
    {
        Vector3D vertices[3];
        Vector3D u;
        Vector3D v;
        Vector4D plane;
        float uu, uv, vv, invD;
        Vector3D lineDir[3];
    };

    bool isPointInsideTriangle(const Triangle &tri, const Vector3D &point);

private:
    std::vector<Triangle> m_triangles;
};


} // namespace fight


#endif // FIGHT_COLLISIONMESH_H
