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


class Object;


class CollisionCache
{
public:
    CollisionCache();

public:
    void addObject(Object *object, bool collision, const glm::vec3 &position, const glm::vec3 &normal);
    bool testObject(Object *object, bool &collision, glm::vec3 &position, glm::vec3 &normal);

private:
    struct CacheEntry
    {
        Object *object;
        bool collision;
        glm::vec3 position;
        glm::vec3 normal;
    };

private:
    std::vector<CacheEntry> m_entries;
};


class CollisionManager
{
public:
    CollisionManager();

public:
    void addObject(Object *object);
    Object* testCollision(const glm::vec3 &start, const glm::vec3 &end, float radius, glm::vec3 &position, glm::vec3 &normal, CollisionCache *collisionCache = nullptr);

private:

private:
    std::vector<Object*> m_objects;
};


} // namespace fight


#endif // FIGHT_COLLISIONMANAGER_H
