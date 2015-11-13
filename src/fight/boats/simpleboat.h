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

#ifndef FIGHT_SIMPLEBOAT_H
#define FIGHT_SIMPLEBOAT_H


#include "fight/objects/simpleobject.h"
#include "fight/weapons/gun.h"
#include <glm/mat3x3.hpp>

namespace fight {


class SimpleBoat : public SimpleObject
{
public:
    SimpleBoat(Scenario *scenario, txt::DesFile &file, const ObjectInfo &info, float yaw);

public:
    const glm::mat3& orientation() const { return m_orientation; }
    const Vector3D& dir() const { return m_orientationInverted[1]; }
    const Vector3D& up() const { return m_orientationInverted[2]; }
    const Vector3D& right() const { return m_orientationInverted[0]; }

    float maxVelocity() const { return m_maxVelocity; }
    float minVelocity() const { return m_minVelocity; }

    float velocityTarget() const { return m_velocityTarget; }
    void setVelocityTarget(float velocityTarget) { m_velocityTarget = velocityTarget; }
    bool fullThrottle() const { return m_fullThrottle; }
    void setFullThrottle(bool fullThrottle) { m_fullThrottle = fullThrottle; }
    void setTurnVelocity(const Vector3D &turnVelocity) { m_turnVelocity = turnVelocity; }

public:
    bool update(float elapsedTime);
    void updateVelocity(float elapsedTime);
    void updateOrientation(float elapsedTime);

protected:
    glm::mat3 m_orientation;
    glm::mat3 m_orientationInverted;

    float m_velocityTarget;
    bool m_fullThrottle;

    float m_maxVelocity;
    float m_minVelocity;
    float m_maxOrientationAcceleration;
    float m_maxOrientationRetardation;
    Vector3D m_orientationFriction;

    Vector3D m_angleVelocity;
    Vector3D m_turnVelocity;

    Vector3D m_maxAngleVelocity;
    Vector3D m_angleFriction;
};


} // namespace fight


#endif // FIGHT_SIMPLEBOAT_H
