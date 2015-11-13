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

#include "simpleboat.h"
#include "txt/desfile.h"
#include <glm/gtx/transform.hpp>


namespace fight {


SimpleBoat::SimpleBoat(Scenario *scenario, txt::DesFile &file, const ObjectInfo &info, float yaw) :
    SimpleObject(scenario, file, info),
    m_velocityTarget(0),
    m_fullThrottle(false),
    m_angleVelocity(0),
    m_turnVelocity(0)
{
    m_orientation = glm::mat3(glm::rotate(yaw, Vector3D(0, 0, 1)));
    m_orientationInverted = glm::transpose(m_orientation);

    file.setSection("kinematics");
    m_maxVelocity = file.value("MaxVelocity").toFloat() / 32.0f;
    m_minVelocity = file.value("MinVelocity").toFloat() / 32.0f;
    m_maxOrientationAcceleration = file.value("MaxOrientationAcceleration").toFloat() / 32.0f;
    m_maxOrientationRetardation = file.value("MaxOrientationRetardation").toFloat() / 32.0f;
    m_orientationFriction = Vector3D(-file.value("OrientationFrictionX").toFloat(), -file.value("OrientationFrictionY").toFloat(), -file.value("OrientationFrictionZ").toFloat());

    m_maxAngleVelocity = Vector3D(file.value("MaxAngleVelocityB").toFloat(), file.value("MaxAngleVelocityC").toFloat(), file.value("MaxAngleVelocityA").toFloat());
    m_angleFriction = Vector3D(-file.value("AngleFrictionB").toFloat(), -file.value("AngleFrictionC").toFloat(), -file.value("AngleFrictionA").toFloat());
}


bool SimpleBoat::update(float elapsedTime)
{
    updateOrientation(elapsedTime);
    updateVelocity(elapsedTime);
    m_position += m_velocity*elapsedTime;

    return false;
}


void SimpleBoat::updateVelocity(float elapsedTime)
{
    Vector3D velocity = m_orientation * m_velocity;
    Vector3D acceleration = velocity*m_orientationFriction;

    float orientationAcceleration = 0;
    if (m_velocityTarget > velocity.y + 0.1f)
        orientationAcceleration = m_maxOrientationAcceleration*(m_fullThrottle ? 2 : 1);
    if (m_velocityTarget < velocity.y - 0.1f)
        orientationAcceleration = m_maxOrientationRetardation*(m_fullThrottle ? 2 : 1);
    acceleration.y += orientationAcceleration;

    velocity += acceleration*elapsedTime;
    m_velocity = m_orientationInverted * velocity;
}


void SimpleBoat::updateOrientation(float elapsedTime)
{
    m_angleVelocity += m_angleVelocity*m_angleFriction*elapsedTime;
    if (glm::abs(m_angleVelocity.x) < m_turnVelocity.x*m_maxAngleVelocity.x)
        m_angleVelocity.x = m_turnVelocity.x*m_maxAngleVelocity.x;
    if (m_angleVelocity.z < m_turnVelocity.z*m_maxAngleVelocity.z)
        m_angleVelocity.z = m_turnVelocity.z*m_maxAngleVelocity.z;

    m_orientation *= glm::mat3(glm::rotate(m_angleVelocity.x*elapsedTime, right())) * glm::mat3(glm::rotate(m_angleVelocity.z*elapsedTime, Vector3D(0, 0, 1)));
    m_orientationInverted = glm::transpose(m_orientation);
}


} // namespace fight
