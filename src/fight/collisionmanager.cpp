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

#include "collisionmanager.h"
#include "object.h"


namespace fight {


CollisionCache::CollisionCache()
{
}


void CollisionCache::addObject(Object *object, bool collision, const QVector3D &position, const QVector3D &normal)
{
    CacheEntry entry;
    entry.object = object;
    entry.collision = collision;
    entry.position = position;
    entry.normal = normal;
    m_entries << entry;
}


bool CollisionCache::testObject(Object *object, bool &collision, QVector3D &position, QVector3D &normal)
{
    foreach (const CacheEntry &entry, m_entries)
    {
        if (entry.object == object)
        {
            collision = entry.collision;
            position = entry.position;
            normal = entry.normal;
            return true;
        }
    }
    return false;
}


CollisionManager::CollisionManager()
{
}


void CollisionManager::addObject(ObjectType type, Object *object)
{
    ObjectEntry entry;
    entry.type = type;
    entry.object = object;
    m_entries << entry;
}


ObjectType CollisionManager::testCollision(const QVector3D &start, const QVector3D &end, float radius, QVector3D &position, QVector3D &normal)
{
    QVector3D dir = end - start;
    float distance = dir.length();
    dir /= distance;

    ObjectType collisionType = NoObject;
    foreach (const ObjectEntry &entry, m_entries)
        if (entry.object->isEnabled() && entry.object->box().test(end, radius))
        {
            bool collision = false;
            float d;
            QVector3D norm;

            collision = entry.object->intersect(start, dir, radius, d, norm);

            if (collision && distance > d)
            {
                distance = d;
                position = start + dir*distance;
                normal = norm;
                collisionType = entry.type;
            }
        }
    return collisionType;
}


ObjectType CollisionManager::testCollision(Object *object, const QVector3D &end, float radius, QVector3D &position, QVector3D &normal)
{
    QVector3D start = object->position();
    QVector3D dir = end - start;
    float distance = dir.length();
    dir /= distance;

    ObjectType collisionType = NoObject;
    foreach (const ObjectEntry &entry, m_entries)
        if (entry.object->isEnabled() && entry.object->box().test(end, radius))
        {
            bool collision = false;
            float d;
            QVector3D pos, norm;

            if (entry.object->isStatic() && object->isStatic())
            {
                if (object->collisionCache() == NULL)
                    object->setCollisionCache(new CollisionCache());
                if (object->collisionCache()->testObject(entry.object, collision, pos, norm))
                    d = QVector3D::dotProduct(pos - start, dir);
                else
                {
                    collision = entry.object->intersect(start, dir, radius, d, norm);
                    object->collisionCache()->addObject(entry.object, collision, start + dir*d, norm);
                }
            }
            else
                collision = entry.object->intersect(start, dir, radius, d, norm);

            if (collision && distance > d)
            {
                distance = d;
                position = start + dir*distance;
                normal = norm;
                collisionType = entry.type;
            }
        }
    return collisionType;
}


} // namespace fight
