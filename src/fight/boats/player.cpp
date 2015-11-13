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

#include "player.h"
#include "fight/scenario.h"
#include "game/boat.h"

#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/geometric.hpp>
#include <glm/gtx/norm.hpp>


namespace fight {


Player::Player(Scenario *scenario, game::Boat *boat, float yaw) :
    SimpleBoat(scenario, boat->boatFile(), ObjectInfo(), yaw),
    m_engineState(EngineRunning),
    m_speed(0),
    m_debugBoost(0),
    m_roll(0),
    m_currentTime(10.0f),
    m_currentNoise1(0),
    m_currentNoise2(0),
    m_firing(false),
    m_gun(scenario, boat->gun() - 3073)
{
    int i;
    txt::DesFile &file = boat->boatFile();

    file.setSection("kinematics");
    m_rollCenterVelocity = file.value("RollCenterVelocity").toFloat();
    m_bottomAlphaSupport = file.value("BottomAlphaSupport").toFloat();
    m_currentAmplitude = Vector3D(file.value("CurrentAmplitudeB").toFloat(), file.value("CurrentAmplitudeC").toFloat(), file.value("CurrentAmplitudeA").toFloat());
    m_current = file.value("Current").toFloat();

    file.setSection("defense");
    for (i = 0; i < 4; i++)
        m_shieldMax[i] = file.value(QString("kineticshield%1").arg(i)).toInt();

    int armor = 0;
    if (boat->armor() == 5121)
        armor = 2000;
    if (boat->armor() == 5122)
        armor = 3500;
    if (boat->armor() == 5123)
        armor = 6500;
    if (boat->armor() == 5124)
        armor = 7500;
    for (i = 0; i < 4; i++)
        m_shieldMax[i] += armor;

    for (i = 0; i < 4; i++)
        m_shield[i] = m_shieldMax[i];

    if (boat->silator() == 4101)
        m_noise -= 1;
    if (boat->nrskin() == 5126)
        m_noise -= 1;

    m_gun.addMuzzles(Vector3D(0, 3.0f, 0), boat->gun());
    m_gun.setDefect(boat->defects()[game::Boat::DefectGun]);
    if (boat->tur1() != 0)
    {
        m_tur1.reset(new Gun(m_scenario, boat->tur1() - 3073));
        m_tur1->addMuzzles(Vector3D(0, 3.0f, 0) + boat->getMounting("TUR1")->rel, boat->tur1());
        m_tur1->setDefect(boat->defects()[game::Boat::DefectTur1]);
    }
    if (boat->tur2() != 0)
    {
        m_tur2.reset(new Gun(m_scenario, boat->tur2() - 3073));
        m_tur2->addMuzzles(Vector3D(0, 3.0f, 0) + boat->getMounting("TUR2")->rel, boat->tur2());
        m_tur2->setDefect(boat->defects()[game::Boat::DefectTur2]);
    }

    sfx::SampleMap::get(sfx::Sample::Engine).playLoop();

    m_scenario->collisionManager().removeObject(this);
}


float Player::noise() const
{
    return (m_engineState != EngineOff ? ActiveObject::noise() : 0) + (m_scenario->sonar().isActive() || m_scenario->sonar().isActivating() ? 1 : 0);
}


float Player::range() const
{
    return m_gun.range();
}


void Player::damage(int kinetic, int shock, const Vector3D &position)
{
    Vector3D dirDamage = glm::normalize(position - m_position);
    float cosZ = glm::dot(dirDamage, dir());
    if (cosZ > 0.9)
        m_shield[0] -= kinetic;
    else if (cosZ < -0.9)
        m_shield[3] -= kinetic;
    else
    {
        float cosY = glm::dot(dirDamage, up());
        if (cosY > 0)
            m_shield[1] -= kinetic;
        else
            m_shield[2] -= kinetic;
    }
}


bool Player::update(float elapsedTime)
{
    m_roll = 0;
    if (glm::abs(dir().z) < 0.99999f)
        m_roll = glm::dot(glm::normalize(Vector3D(dir().y, -dir().x, 0)), up());

    m_angleVelocity.x += m_angleVelocity.x*m_angleFriction.x*64*elapsedTime;
    m_angleVelocity.x -= 0.64f*dir().z*m_maxAngleVelocity.x*elapsedTime;
    if (m_turnVelocity.x > 0 && m_angleVelocity.x < m_maxAngleVelocity.x)
        m_angleVelocity.x -= m_maxAngleVelocity.x*m_angleFriction.x*64*elapsedTime;
    else if (m_turnVelocity.x < 0 && m_angleVelocity.x > -m_maxAngleVelocity.x)
        m_angleVelocity.x += m_maxAngleVelocity.x*m_angleFriction.x*64*elapsedTime;

    m_angleVelocity.y += m_angleVelocity.y*m_angleFriction.y*64*elapsedTime;
    m_angleVelocity.y += m_roll*m_rollCenterVelocity*64*m_maxAngleVelocity.y*elapsedTime;
    if (m_turnVelocity.y > 0 && m_angleVelocity.y < m_maxAngleVelocity.y)
        m_angleVelocity.y -= m_maxAngleVelocity.y*m_angleFriction.y*64*elapsedTime;
    if (m_turnVelocity.y < 0 && m_angleVelocity.y > -m_maxAngleVelocity.y)
        m_angleVelocity.y += m_maxAngleVelocity.y*m_angleFriction.y*64*elapsedTime;

    m_angleVelocity.z = m_roll*m_bottomAlphaSupport*m_maxAngleVelocity.z;

    const float currentLen = 5.0f;
    m_currentTime += elapsedTime;
    if (m_currentTime > currentLen*0.75f)
    {
        m_currentTime = 0;
        m_currentNoise1 = m_currentNoise2;
        m_currentNoise2 = Vector3D(0.5f*qrand()/RAND_MAX, 0.5f*qrand()/RAND_MAX, 0.5f*qrand()/RAND_MAX);
        m_currentNoise2 *= m_currentNoise2;
        if (m_currentNoise1.z > 0)
            m_currentNoise2 = -m_currentNoise2;
    }
    float t = m_currentTime/currentLen;
    if (t < 0.25f)
        m_angleVelocity += m_currentAmplitude*m_currentNoise1*glm::sin(glm::pi<float>()*2*(t + 0.75f));
    m_angleVelocity += m_currentAmplitude*m_currentNoise2*glm::sin(glm::pi<float>()*2*t);

    m_orientation *= glm::mat3(glm::rotate(m_angleVelocity.z*elapsedTime, Vector3D(0, 0, 1))) * glm::mat3(glm::rotate(m_angleVelocity.y*elapsedTime, dir())) * glm::mat3(glm::rotate(m_angleVelocity.x*elapsedTime, right()));
    m_orientationInverted = glm::transpose(m_orientation);

    updateVelocity(elapsedTime);

    if (m_engineState == EngineStartup && m_engineTime < m_scenario->time())
    {
        m_engineState = EngineRunning;
        sfx::SampleMap::get(sfx::Sample::Engine).playLoop();
    }
    if (m_engineState == EngineShutdown && m_engineTime < m_scenario->time())
        m_engineState = EngineOff;
    if (m_engineState == EngineRunning)
    {
        float pitch = glm::length(m_velocity)*0.036f;
        if (pitch > 1.0f)
            pitch = 1.0f;
        if (pitch < -0.5f)
            pitch = -0.5f;
        sfx::SampleMap::get(sfx::Sample::Engine).setPitch(1.0f + pitch);
    }

    Vector3D vel = m_velocity + dir()*m_debugBoost;
    Vector3D delta = vel*elapsedTime;
    float path = 0;

    Vector3D posSurface, normSurface;
    bool colSurface = m_scenario->surface().testCollision(m_position, m_position + delta, 1.5f, posSurface, normSurface);

    Vector3D posObject, normObject;
    Object *colObject = m_scenario->collisionManager().testCollision(m_position, m_position + delta, 1.5f, posObject, normObject);

    if (colObject && (!colSurface || glm::length2(posObject - m_position) < glm::length2(posSurface - m_position)))
    {
        ActiveObject *activeObject = dynamic_cast<ActiveObject *>(colObject);
        if (activeObject)
        {
            activeObject->damage(m_kineticStrength, m_shockStrength, posObject - normObject*1.5f);
            damage(activeObject->kineticStrength(), activeObject->shockStrength(), posObject - normObject*1.5f);
        }
        sfx::SampleMap::get(sfx::Sample::Boat_Collision).playInstance();
        m_velocity -= 2*glm::dot(m_velocity, normObject)*normObject;
        delta = posObject - m_position;
    }
    else if (colSurface)
    {
        int count = 0;
        while (colSurface && glm::length2(delta) > 1e-3f && count < 5)
        {
            delta -= posSurface - m_position;
            path += glm::length(posSurface - m_position);
            m_position = posSurface;
            delta -= (glm::dot(delta, normSurface) - 1e-3f)*normSurface;
            colSurface = m_scenario->surface().testCollision(m_position, m_position + delta, 1.5f, posSurface, normSurface);
            count++;
        }
        if (colSurface)
            delta = Vector3D(0);
    }

    path += glm::length(delta);
    m_position += delta;
    m_speed = path/elapsedTime*3.6f;
    if (glm::dot(dir(), vel) < 0)
        m_speed = -m_speed;


    m_gun.update(elapsedTime);
    if (m_firing && m_gun.state() == Gun::StateReady)
        m_gun.fire(m_position, dir(), up(), right());
    if (m_tur1)
    {
        m_tur1->update(elapsedTime);
        if (m_firing && m_tur1->state() == Gun::StateReady)
            m_tur1->fire(m_position, dir(), up(), right());
    }
    if (m_tur2)
    {
        m_tur2->update(elapsedTime);
        if (m_firing && m_tur2->state() == Gun::StateReady)
            m_tur2->fire(m_position, dir(), up(), right());
    }

    return false;
}


void Player::engineToggle()
{
    if (m_engineState == EngineOff)
    {
        m_engineState = EngineStartup;
        m_engineTime = m_scenario->time() + 1;
        sfx::SampleMap::get(sfx::Sample::Engine_Startup).play();
    }
    if (m_engineState == EngineRunning)
    {
        m_engineState = EngineShutdown;
        m_engineTime = m_scenario->time() + 1;
        sfx::SampleMap::get(sfx::Sample::Engine_Shutdown).play();
        sfx::SampleMap::get(sfx::Sample::Engine).stop();
    }
}


void Player::fire()
{
    m_firing = true;
}


void Player::fireStop()
{
    m_firing = false;
}


} // namespace fight
