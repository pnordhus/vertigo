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


void CollisionManager::addObject(Object *object)
{
    m_objects << object;
}


Object* CollisionManager::testCollision(const QVector3D &start, const QVector3D &end, float radius, QVector3D &position, QVector3D &normal)
{
    QVector3D dir = end - start;
    float distance = dir.length();
    dir /= distance;

    Object *collisionObject = NULL;
    foreach (Object *object, m_objects)
        if (object->isEnabled() && object->box().test(end, radius))
        {
            bool collision = false;
            float d;
            QVector3D norm;

            collision = object->intersect(start, dir, radius, d, norm);

            if (collision && distance > d)
            {
                distance = d;
                position = start;
                if (distance > 0)
                    position += dir*distance;
                normal = norm;
                collisionObject = object;
            }
        }
    return collisionObject;
}


Object* CollisionManager::testCollision(Object *cacheObject, const QVector3D &end, float radius, QVector3D &position, QVector3D &normal)
{
    QVector3D start = cacheObject->position();
    QVector3D dir = end - start;
    float distance = dir.length();
    dir /= distance;

    Object *collisionObject = NULL;
    foreach (Object *object, m_objects)
        if (object->isEnabled() && object->box().test(end, radius))
        {
            bool collision = false;
            float d;
            QVector3D pos, norm;

            if (object->isStatic() && cacheObject->isStatic())
            {
                if (cacheObject->collisionCache() == NULL)
                    cacheObject->setCollisionCache(new CollisionCache());
                if (cacheObject->collisionCache()->testObject(object, collision, pos, norm))
                    d = QVector3D::dotProduct(pos - start, dir);
                else
                {
                    collision = object->intersect(start, dir, radius, d, norm);
                    cacheObject->collisionCache()->addObject(object, collision, start + dir*d, norm);
                }
            }
            else
                collision = object->intersect(start, dir, radius, d, norm);

            if (collision && distance > d)
            {
                distance = d;
                position = start;
                if (distance > 0)
                    position += dir*distance;
                normal = norm;
                collisionObject = object;
            }
        }
    return collisionObject;
}


} // namespace fight
