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
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>


namespace fight {


class CollisionMesh
{
public:
    CollisionMesh();

public:
    void addTriangles(const std::vector<glm::vec3> &vertices, const std::vector<unsigned short> &indices);
    bool intersect(const glm::vec3 &start, const glm::vec3 &dir, float radius, float &distance, glm::vec3 &normal);

    static bool intersectSphereLine(const glm::vec3 &point, const glm::vec3 &dir, float radiusSquared, float &t);

private:
    struct Triangle
    {
        glm::vec3 vertices[3];
        glm::vec3 u;
        glm::vec3 v;
        glm::vec4 plane;
        float uu, uv, vv, invD;
        glm::vec3 lineDir[3];
    };

    bool isPointInsideTriangle(const Triangle &tri, const glm::vec3 &point);

private:
    std::vector<Triangle> m_triangles;
};


} // namespace fight


#endif // FIGHT_COLLISIONMESH_H
