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

#ifndef FIGHT_BOUNDINGBOX_H
#define FIGHT_BOUNDINGBOX_H


#include <QVector3D>
#include <QMatrix4x4>


namespace fight {


class BoundingBox
{
public:
    BoundingBox();
    BoundingBox(const BoundingBox &box);
    BoundingBox(const QVector3D &min, const QVector3D &max);

public:
    const QVector3D& minPoint() const { return m_min; }
    const QVector3D& maxPoint() const { return m_max; }
    QVector3D dim() const { return m_max - m_min; }
    BoundingBox transform(QMatrix4x4 m);

    void add(const QVector3D &point);
    void add(const BoundingBox &box);
    bool test(const QVector3D &center, float radius) const;

public:
    QVector3D m_min;
    QVector3D m_max;
};


} // namespace fight


#endif // FIGHT_BOUNDINGBOX_H
