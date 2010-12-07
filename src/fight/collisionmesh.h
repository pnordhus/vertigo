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


#include <QVector>
#include <QVector3D>
#include <QVector4D>


namespace fight {


class CollisionMesh
{
public:
    CollisionMesh();

public:
    void addTriangles(const QVector<QVector3D> &vertices, const QVector<quint16> &indices);
    bool intersect(const QVector3D &start, const QVector3D &dir, float radius, float &distance, QVector3D &normal);

private:
    struct Triangle
    {
        QVector3D vertices[3];
        QVector3D u;
        QVector3D v;
        QVector4D plane;
        float uu, uv, vv, invD;
        QVector3D lineDir[3];
    };

    bool isPointInsideTriangle(const Triangle &tri, const QVector3D &point);
    bool intersectSphereLine(const QVector3D &point, const QVector3D &dir, float radiusSquared, float &t);

private:
    QList<Triangle> m_triangles;
};


} // namespace fight


#endif // FIGHT_COLLISIONMESH_H
