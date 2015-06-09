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
#include <qmath.h>


namespace fight {


CollisionMesh::CollisionMesh()
{
}


void CollisionMesh::addTriangles(const QVector<QVector3D> &vertices, const QVector<quint16> &indices)
{
    for (int i = 0; i < indices.count(); i += 3)
    {
        Triangle tri;
        tri.vertices[0] = vertices[indices[i]];
        tri.vertices[1] = vertices[indices[i + 2]];
        tri.vertices[2] = vertices[indices[i + 1]];

        tri.u = tri.vertices[1] - tri.vertices[0];
        tri.v = tri.vertices[2] - tri.vertices[0];

        QVector3D n = QVector3D::normal(tri.u, tri.v);
        float d = -QVector3D::dotProduct(tri.vertices[0], n);
        tri.plane = QVector4D(n, d);

        tri.uu = QVector3D::dotProduct(tri.u, tri.u);
        tri.uv = QVector3D::dotProduct(tri.u, tri.v);
        tri.vv = QVector3D::dotProduct(tri.v, tri.v);
        d = tri.uv * tri.uv - tri.uu * tri.vv;
        tri.invD = 1 / d;

        for (int i = 0; i < 3; i++)
        {
            tri.lineDir[i] = tri.vertices[(i + 1)%3] - tri.vertices[i];
            tri.lineDir[i].normalize();
        }

        m_triangles << tri;
    }
}


bool CollisionMesh::intersect(const QVector3D &start, const QVector3D &dir, float radius, float &distance, QVector3D &normal)
{
    QVector4D dir4D(dir, 0);
    QVector4D center(start, 1);
    float radiusSquared = radius*radius;
    bool collision = false;
    int i;

    distance = 1e10f;

    foreach (const Triangle &tri, m_triangles)
    {
        float cos = QVector4D::dotProduct(dir4D, tri.plane);
        if (cos > -1e-5f)
		    continue;

        float h = QVector4D::dotProduct(center, tri.plane);
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
                normal = tri.plane.toVector3D();
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
            const QVector3D &edge0 = tri.vertices[i];
            const QVector3D &edge1 = tri.vertices[(i + 1)%3];
            const QVector3D &lineDir = tri.lineDir[i];

            QVector3D n = QVector3D::crossProduct(edge1 - edge0, edge1 - edge0 - dir);
            h = QVector3D::dotProduct(start, n) - QVector3D::dotProduct(edge0, n);
            float hSquared = h*h/n.lengthSquared();
            if (hSquared > radiusSquared)
                continue;
            QVector3D pt0 = start - n*h/n.lengthSquared();

            QVector3D fPoint = edge0 + QVector3D::dotProduct(pt0 - edge0, lineDir)*lineDir - pt0;
            QVector3D fDir = QVector3D::dotProduct(dir, lineDir)*lineDir - dir;
            if (!intersectSphereLine(fPoint, fDir, radiusSquared - hSquared, t))
                continue;

            QVector3D inter = edge0 + QVector3D::dotProduct(dir*t + pt0 - edge0, lineDir)*lineDir;
            if (QVector3D::dotProduct(edge0 - inter, edge1 - inter) > 0)
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


bool CollisionMesh::isPointInsideTriangle(const Triangle &tri, const QVector3D &point)
{
    QVector3D w = point - tri.vertices[0];
    float wu = QVector3D::dotProduct(w, tri.u);
    float wv = QVector3D::dotProduct(w, tri.v);
    float s = (tri.uv * wv - tri.vv * wu) * tri.invD;
    if (s < 0 || s > 1)
        return false;
    float t = (tri.uv * wu - tri.uu * wv) * tri.invD;
    if (t < 0 || (s + t) > 1)
        return false;
    return true;
}


bool CollisionMesh::intersectSphereLine(const QVector3D &point, const QVector3D &dir, float radiusSquared, float &t)
{
    float a = dir.lengthSquared();
    float b = 2*QVector3D::dotProduct(dir, point);
    float c = point.lengthSquared() - radiusSquared;
    float d = b*b - 4*a*c;
    if (d < 0)
        return false;
    d = qSqrt(d);
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
