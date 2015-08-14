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

#include "target.h"
#include "scenario.h"
#include "objects/activeobject.h"
#include "objects/navpoint.h"

#include <glm/geometric.hpp>


namespace fight {


Target::Target(Scenario *scenario) :
    m_scenario(scenario),
    m_locked(nullptr),
    m_lockedNavPoint(nullptr)
{
}


void Target::lockReset()
{
    m_locked = nullptr;
}


void Target::lockReticle()
{
    ActiveObject *object = nullptr;
    float minAngle = -1.0f;

    for (const auto &entry : m_scenario->sonar())
    {
        glm::vec3 dir = glm::vec3(m_scenario->cameraMatrix() * glm::vec4(entry.object->center() - m_scenario->position(), 1));
        dir /= -glm::length(dir);
        if (minAngle < dir.z)
        {
            minAngle = dir.z;
            object = entry.object;
        }
    }

    if (minAngle > 0.98f)
    {
        m_locked = object;
        m_lockedNavPoint = nullptr;
    }

    for (NavPoint *navPoint : m_scenario->navPoints())
    {
        if (!navPoint->isEnabled())
            continue;
        glm::vec3 dir = glm::vec3(m_scenario->cameraMatrix() * glm::vec4(navPoint->position() - m_scenario->position(), 1));
        dir /= -glm::length(dir);
        if (minAngle < dir.z && dir.z > 0.98f)
        {
            minAngle = dir.z;
            m_lockedNavPoint = navPoint;
            m_locked = nullptr;
        }
    }
}


void Target::lockNavPoint(NavPoint *navPoint)
{
    m_lockedNavPoint = navPoint;
    m_locked = nullptr;
}


} // namespace fight
