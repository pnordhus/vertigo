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

#include "boundingbox.h"


namespace fight {


BoundingBox::BoundingBox() :
    m_min(1e5, 1e5, 1e5),
    m_max(-1e5, -1e5, -1e5)
{
}


BoundingBox::BoundingBox(const BoundingBox &box) :
    m_min(box.minPoint()),
    m_max(box.maxPoint())
{
}


BoundingBox::BoundingBox(const Vector3D &min, const Vector3D &max) :
    m_min(min),
    m_max(max)
{
}


void BoundingBox::add(const Vector3D &point)
{
    if (m_min.x > point.x)
        m_min.x = point.x;
    if (m_min.y > point.y)
        m_min.y = point.y;
    if (m_min.z > point.z)
        m_min.z = point.z;
    if (m_max.x < point.x)
        m_max.x = point.x;
    if (m_max.y < point.y)
        m_max.y = point.y;
    if (m_max.z < point.z)
        m_max.z = point.z;
}


void BoundingBox::add(const BoundingBox &box)
{
    add(box.minPoint());
    add(box.maxPoint());
}


bool BoundingBox::test(const Vector3D &center, float radius) const
{
    if (center.x + radius < m_min.x)
        return false;
    if (center.y + radius < m_min.y)
        return false;
    if (center.z + radius < m_min.z)
        return false;
    if (center.x - radius > m_max.x)
        return false;
    if (center.y - radius > m_max.y)
        return false;
    if (center.z - radius > m_max.z)
        return false;
    return true;
}


BoundingBox BoundingBox::transform(const Matrix &m) const
{
    BoundingBox ret;
    for (int i = 0; i < 8; i++)
    {
        Vector3D vertex = m_min;
        if (i & 1)
            vertex.x = m_max.x;
        if (i & 2)
            vertex.y = m_max.y;
        if (i & 4)
            vertex.z = m_max.z;
        vertex = Vector3D(m * Vector4D(vertex, 1));
        ret.add(vertex);
    }
    return ret;
}


BoundingBox BoundingBox::scale(float scale) const
{
    return BoundingBox(m_min*scale, m_max*scale);
}


BoundingBox BoundingBox::translate(const Vector3D &t) const
{
    return BoundingBox(m_min + t, m_max + t);
}

} // namespace fight
