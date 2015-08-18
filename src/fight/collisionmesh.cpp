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

#include "collisionmesh.h"

#include <glm/geometric.hpp>
#include <glm/exponential.hpp>
#include <glm/gtx/norm.hpp>

namespace fight {


CollisionMesh::CollisionMesh()
{
}


void CollisionMesh::addTriangles(const std::vector<Vector3D> &vertices, const std::vector<unsigned short> &indices)
{
    for (int i = 0; i < indices.size(); i += 3)
    {
        m_triangles.emplace_back();
        Triangle &tri = m_triangles.back();
        tri.vertices[0] = vertices[indices[i]];
        tri.vertices[1] = vertices[indices[i + 2]];
        tri.vertices[2] = vertices[indices[i + 1]];

        tri.u = tri.vertices[1] - tri.vertices[0];
        tri.v = tri.vertices[2] - tri.vertices[0];

        Vector3D n = glm::normalize(glm::cross(tri.u, tri.v));
        tri.plane = Vector4D(n, -glm::dot(tri.vertices[0], n));

        tri.uu = glm::dot(tri.u, tri.u);
        tri.uv = glm::dot(tri.u, tri.v);
        tri.vv = glm::dot(tri.v, tri.v);
        tri.invD = 1 / (tri.uv * tri.uv - tri.uu * tri.vv);

        for (int i = 0; i < 3; i++)
            tri.lineDir[i] = glm::normalize(tri.vertices[(i + 1)%3] - tri.vertices[i]);
    }
}


bool CollisionMesh::intersect(const Vector3D &start, const Vector3D &dir, float radius, float &distance, Vector3D &normal)
{
    Vector4D dir4D(dir, 0);
    Vector4D center(start, 1);
    float radiusSquared = radius*radius;
    bool collision = false;
    int i;

    distance = 1e10f;

    for (const Triangle &tri : m_triangles)
    {
        float cos = glm::dot(dir4D, tri.plane);
        if (cos > -1e-5f)
		    continue;

        float h = glm::dot(center, tri.plane);
	    if (h < 0)
		    continue;
        float t = 0;
        if (h > radius)
        {
	        h -= radius;
	        t = -h / cos;
        }
	    if (isPointInsideTriangle(tri, start + dir*t))
        {
            if (distance > t)
            {
                distance = t;
                normal = Vector3D(tri.plane);
                collision = true;
            }
	    }

        for (i = 0; i < 3; i++)
        {
            if (!intersectSphereLine(tri.vertices[i] - start, -dir, radiusSquared, t))
                continue;

            if (distance > t)
            {
                distance = t;
                normal = (dir*t + start - tri.vertices[i])/radius;
                collision = true;
            }
        }

        for (i = 0; i < 3; i++)
        {
            const Vector3D &edge0 = tri.vertices[i];
            const Vector3D &edge1 = tri.vertices[(i + 1)%3];
            const Vector3D &lineDir = tri.lineDir[i];

            Vector3D n = glm::cross(edge1 - edge0, edge1 - edge0 - dir);
            h = glm::dot(start, n) - glm::dot(edge0, n);
            float hSquared = h*h/glm::length2(n);
            if (hSquared > radiusSquared)
                continue;
            Vector3D pt0 = start - n*h/glm::length2(n);

            Vector3D fPoint = edge0 + glm::dot(pt0 - edge0, lineDir)*lineDir - pt0;
            Vector3D fDir = glm::dot(dir, lineDir)*lineDir - dir;
            if (!intersectSphereLine(fPoint, fDir, radiusSquared - hSquared, t))
                continue;

            Vector3D inter = edge0 + glm::dot(dir*t + pt0 - edge0, lineDir)*lineDir;
            if (glm::dot(edge0 - inter, edge1 - inter) > 0)
                continue;
            if (distance > t)
            {
                distance = t;
                normal = (dir*t + start - inter)/radius;
                collision = true;
            }
        }
    }
    return collision;
}


bool CollisionMesh::isPointInsideTriangle(const Triangle &tri, const Vector3D &point)
{
    Vector3D w = point - tri.vertices[0];
    float wu = glm::dot(w, tri.u);
    float wv = glm::dot(w, tri.v);
    float s = (tri.uv * wv - tri.vv * wu) * tri.invD;
    if (s < 0 || s > 1)
        return false;
    float t = (tri.uv * wu - tri.uu * wv) * tri.invD;
    if (t < 0 || (s + t) > 1)
        return false;
    return true;
}


bool CollisionMesh::intersectSphereLine(const Vector3D &point, const Vector3D &dir, float radiusSquared, float &t)
{
    float a = glm::length2(dir);
    float b = 2*glm::dot(dir, point);
    float c = glm::length2(point) - radiusSquared;
    float d = b*b - 4*a*c;
    if (d < 0)
        return false;
    d = glm::sqrt(d);
    t = (-b - d)/(2*a);
    if (t < 0)
    {
        float t2 = (-b + d)/(2*a);
        if (t2 < -t)
            return false;
    }
    return true;
}


} // namespace fight
