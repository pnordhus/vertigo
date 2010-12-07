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

#ifndef FIGHT_COLLISIONMANAGER_H
#define FIGHT_COLLISIONMANAGER_H


#include "boundingbox.h"


namespace fight {


enum ObjectType
{
    NoObject,
    BuildingObject,
};


class Object;


class CollisionCache
{
public:
    CollisionCache();

public:
    void addObject(Object *object, bool collision, const QVector3D &position, const QVector3D &normal);
    bool testObject(Object *object, bool &collision, QVector3D &position, QVector3D &normal);

private:
    struct CacheEntry
    {
        Object *object;
        bool collision;
        QVector3D position;
        QVector3D normal;
    };

private:
    QList<CacheEntry> m_entries;
};


class CollisionManager
{
public:
    CollisionManager();

public:
    void addObject(ObjectType type, Object *object);
    ObjectType testCollision(const QVector3D &start, const QVector3D &end, float radius, QVector3D &position, QVector3D &normal);
    ObjectType testCollision(Object *object, const QVector3D &end, float radius, QVector3D &position, QVector3D &normal);

private:
    struct ObjectEntry
    {
        ObjectType type;
        Object *object;
    };

private:
    QList<ObjectEntry> m_entries;
};


} // namespace fight


#endif // FIGHT_COLLISIONMANAGER_H
