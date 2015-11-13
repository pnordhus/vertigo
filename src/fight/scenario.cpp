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

#include "scenario.h"
#include "module.h"
#include "surface/element.h"
#include "surface/betaspline.h"
#include "effects/billboard.h"
#include "effects/trash.h"
#include "game/boat.h"
#include "sfx/samplemap.h"

#include "objects/building.h"
#include "objects/mine.h"
#include "objects/turretbase.h"
#include "objects/navpoint.h"
#include "objects/simpleobject.h"

#include "boats/simpleboat.h"
#include "boats/player.h"

#include <QGLContext>
#include <QTime>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <algorithm>


namespace fight {


Scenario::Scenario(const QString &name) :
    m_moduleManager(m_textureManager),
    m_effectManager(this),
    m_conditionManager(this),
    m_sonar(this),
    m_target(this),
    m_yaw(0),
    m_pitch(0),
    m_speed(0),
    m_velocityTarget(0),
    m_time(0),
    m_left(0.0f),
    m_right(0.0f),
    m_up(0.0f),
    m_down(0.0f),
    m_forwards(0.0f),
    m_backwards(0.0f)
{
    qsrand(name.right(4).toInt());

    m_file.load(QString("vfx:scenario/%1.des").arg(name));

    m_file.setSection("surface");
    m_depth = m_file.value("depth").toInt();
    m_surface.load(m_file.value("name").toString().toLower(), m_file.value("maxheightscale").toInt(), m_file.value("patchcomb").toInt());

    std::map<int, QString> types;
    types[ 1] = "anscout2";
    types[ 5] = "guntow0";
    types[ 7] = "guntow2";
    types[ 8] = "bioscout";
    types[ 9] = "honglong";
    types[16] = "anbombr1";
    types[10] = "russcout";
    types[12] = "atscout";
    types[27] = "mine0";
    types[28] = "mine1";
    types[36] = "tortow0";
    types[39] = "anscout1";
    types[40] = "anbombr2";
    types[41] = "atbomber";
    types[47] = "build2";
    types[48] = "build3";
    types[50] = "build5";
    types[67] = "entrobot";

    float initialDir = 0.0f;
    foreach (const QString &section, m_file.sections().filter(QRegExp("^movable\\d*"))) {
        m_file.setSection(section);

        const int id = m_file.value("id").toInt();
        ConditionManager::ConditionEntry &entry = m_conditionManager.addEntry(id);
        entry.cond1 = m_file.value("bccond1").toInt();
        entry.dep1 = m_file.value("bcdep1").toInt();
        entry.ref1 = m_file.value("bcref1").toInt();
        entry.op = m_file.value("bcop").toInt();
        entry.cond2 = m_file.value("bccond2").toInt();
        entry.dep2 = m_file.value("bcdep2").toInt();
        entry.ref2 = m_file.value("bcref2").toInt();
        entry.del = m_file.value("bcdel").toInt();
        entry.condTrigger = NULL;
        entry.condSignal = NULL;
        entry.condAttacked = NULL;
        entry.condIdentified = NULL;
        entry.condParalyzed = NULL;
        entry.condFinished = NULL;
        entry.condBoarded = NULL;

        Object *object = NULL;
        const int type = m_file.value("typ").toInt();

        txt::DesFile objectDes;
        if (m_file.contains("dtyp") && type != TypeNavPoint && type != TypeCrawler)
        {
            const int dType = m_file.value("dtyp").toInt();
            if (types.find(dType) == types.end()) {
                qDebug() << "Unhandled dtype" << dType;
                continue;
            }
            objectDes.load(QString("vfx:sobjects/%1.des").arg(types[dType]));
        }

        ActiveObject::ObjectInfo info;
        if (m_file.contains("iff"))
            info.iff = m_file.value("iff").toInt();
        if (m_file.contains("inf"))
            info.name = m_file.valueText("inf");
        if (m_file.contains("carg"))
            info.cargo = m_file.valueText("carg");
        if (m_file.contains("sen"))
            info.sensor = m_file.value("sen").toInt();
        if (m_file.contains("senr"))
            info.sensorRange = m_file.value("senr").toInt();

        switch (type) {
        case TypeBoat:
            {
                object = new SimpleBoat(this, objectDes, info, 0);
                Vector3D pos = getPosition();
                pos.z += 20.0f;
                object->setPosition(pos);
            }
            break;

        case TypeBomber:
            {
                object = new SimpleBoat(this, objectDes, info, 0);
                Vector3D pos = getPosition();
                pos.z += 20.0f;
                object->setPosition(pos);
            }
            break;

        case TypeTower:
        case TypeTorpedoTower:
            {
                object = new TurretBase(this, objectDes, info);
                object->setPosition(getPosition());
            }
            break;

        case TypeBuilding:
            {
                objectDes.load(QString("vfx:sobjects/gp%1x%1_%2.des").arg(m_file.value("siz").toInt() + 1).arg(m_file.value("buityp").toInt()));
                object = new Building(this, objectDes, m_file.value("siz").toInt(), m_file.value("card", 0).toInt() * 45.0f, m_file.value("px").toInt(), m_file.value("py").toInt(), m_file.value("refx").toInt(), m_file.value("refy").toInt());
            }
            break;

        case TypeMine:
            {
                // TODO: dis num
                object = new Mine(this, objectDes, info);
                object->setPosition(getPosition());
            }
            break;

        case TypePlayer:
            m_position = getPosition();
            m_initialYaw = m_file.value("card").toInt()*glm::pi<float>()/4;
            //m_position.setZ(m_position.z() + 20.0f);
            m_height = m_position.z - m_surface.heightAt(m_position.x, m_position.y);
            entry.condTrigger = m_conditionManager.addCondition(0);

            {
                /*for (int i = 0; i < 27; i++)
                    m_effectManager.addEffect((Effects)((int)Effects::Explosion_0 + i), m_position + Vector3D(i*5, 0, 0))->setPermanent(true);
                for (int i = 0; i < 9; i++)
                    m_effectManager.addEffect((Effects)((int)Effects::Shoot_0 + i), m_position + Vector3D(i*5, -10, 0))->setPermanent(true);
                for (int i = 0; i < 23; i++)
                    m_effectManager.addEffect((Effects)((int)Effects::Debris_0 + i), m_position + Vector3D(i*5, -20, 0))->setPermanent(true);
                for (int i = 0; i < 5; i++)
                    m_effectManager.addEffect((Effects)((int)Effects::Trash_0 + i), m_position + Vector3D(i*5, -30, 0))->setPermanent(true);
                for (int i = 0; i < 3; i++)
                    m_effectManager.addEffect((Effects)((int)Effects::Bubble_0 + i), m_position + Vector3D(i*5, -40, 0))->setPermanent(true);*/
            }
            break;

        case TypeCrawler:
            {
                objectDes.load("vfx:sobjects/gvehicle.des");
                object = new SimpleObject(this, objectDes, info);
                object->setPosition(getPosition());
            }
            break;

        case TypeNavPoint:
            {
                object = new NavPoint(this, m_file.value("dtyp").toInt());
                object->setPosition(getPosition() + Vector3D(-0.5f*m_surface.scale().x, -0.5f*m_surface.scale().y, 10));
                m_navPoints.push_back(static_cast<NavPoint*>(object));
            }
            break;

        case TypeActiveBuilding:
            {
                object = new SimpleObject(this, objectDes, info, 16);
                object->setPosition(getPosition());
            }
            break;

        case TypeTrash:
            {
                Vector3D pos = getPosition();
                entry.condTrigger = m_conditionManager.addCondition(0);
                entry.condSignal = m_conditionManager.addCondition(9);
                for (int i = 0; i < 9; i++)
                {
                    Trash *trash = new Trash(this, m_effectManager.getBillboard(Trash::trashCollection[i]), pos);
                    m_objects.emplace_back(trash);
                    trash->disable();
                    trash->condEnable()->setLimit(1);
                    entry.condTrigger->addDependency(trash->condEnable());
                    trash->eventDestroy()->addDependency(static_cast<Condition *>(entry.condSignal));
                }
            }
            break;

        case TypeSpace:
            {
                if (m_file.value("pz").toInt() != 0)
                    qDebug() << "Unexpected space parameter";
                ConditionSpace *space = m_conditionManager.addCondSpace(m_file.value("px").toInt()*m_surface.scale().x,
                                                                        m_file.value("py").toInt()*m_surface.scale().y,
                                                                        m_file.value("dimx").toInt()*m_surface.scale().x,
                                                                        m_file.value("dimy").toInt()*m_surface.scale().y,
                                                                        m_file.value("minz").toInt()*m_surface.scale().z,
                                                                        m_file.value("maxz").toInt()*m_surface.scale().z);
                entry.condTrigger = space->condEnable();
                entry.condSignal = space;
            }
            break;

        default:
            qDebug() << "Unhandled movable" << type << m_file.value("dtyp").toInt();;
        }

        if (object != NULL)
        {
            m_objects.emplace_back(object);
            object->disable();
            entry.condTrigger = object->condEnable();
            ActiveObject *activeObject = dynamic_cast<ActiveObject *>(object);
            if (activeObject)
            {
                entry.condSignal = activeObject->eventDestroy();
                entry.condAttacked = activeObject->eventAttack();
                entry.condIdentified = activeObject->eventIdentify();
                entry.condParalyzed = activeObject->eventParalyze();
                entry.condFinished = activeObject->eventFinish();
                entry.condBoarded = activeObject->eventBoard();
            }
        }

        // Reading Objectives
        for (int i = 0; i < 16; i++)
        {
            int wccond = m_file.value(QString("wccond%1").arg(i)).toInt();
            if (wccond == 0)
                continue;
            m_conditionManager.updateObjective(id, i, wccond);
        }
    }

    foreach (const QString &section, m_file.sections().filter(QRegExp("^radio\\d*"))) {
        m_file.setSection(section);

        const int id = m_file.value("id").toInt();
        ConditionManager::ConditionEntry &entry = m_conditionManager.addEntry(id);
        entry.cond1 = m_file.value("bccond1").toInt();
        entry.dep1 = m_file.value("bcdep1").toInt();
        entry.ref1 = m_file.value("bcref1").toInt();
        entry.op = m_file.value("bcop").toInt();
        entry.cond2 = m_file.value("bccond2").toInt();
        entry.dep2 = m_file.value("bcdep2").toInt();
        entry.ref2 = m_file.value("bcref2").toInt();
        entry.del = m_file.value("bcdel").toInt();

        const int type = m_file.value("typ").toInt();
        if (type != 5120)
            qDebug() << "Unhandled radio type " << type;

        ConditionRadio *radio = m_conditionManager.addCondRadio(getPosition(), m_file.valueText("rtxt"));
        entry.condTrigger = radio;
        entry.condSignal = nullptr;
        entry.condAttacked = nullptr;
        entry.condIdentified = nullptr;
        entry.condParalyzed = nullptr;
        entry.condFinished = nullptr;
        entry.condBoarded = nullptr;
    }

    m_conditionManager.buildDependencies();

    std::sort(m_navPoints.begin(), m_navPoints.end(), [](NavPoint *a, NavPoint *b) { return a->num() < b->num(); });

    m_file.setSection("AttitudeMatrix");
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            m_attitudeMatrix[i][j] = m_file.value(QString("X%1Y%2").arg(j).arg(i)).toInt();

    m_file.setSection("End");
    m_endType = m_file.value("Type").toInt();



    qsrand(QTime::currentTime().second() * 1000 + QTime::currentTime().msec());

    sfx::SampleMap::load();
}


void Scenario::setBoat(game::Boat *boat)
{
    //if (boat->gun() == 3081)
    //    boat->buy(3083, "GUN"); // TODO: remove
    m_player.reset(new Player(this, boat, m_initialYaw));
    m_player->setPosition(m_position);

    m_buzzers = boat->buzzers().size();

    m_sonar.init(boat->sensor());
    m_sonar.activate();
}


void Scenario::setRect(const RectF &rect, const Vector2D &center)
{
    m_projectionMatrix = glm::translate(Vector3D(center, 0)) * glm::perspective(glm::radians(60.0f), float(rect.width / rect.height), 0.1f, 10000.0f);
    m_projectionMatrixInverted = glm::inverse(m_projectionMatrix);
}


void Scenario::update(float elapsedTime)
{
    m_time += elapsedTime;

    m_player->setTurnVelocity(Vector3D(m_up - m_down, m_left - m_right, 0));
    m_player->update(elapsedTime);

    Vector3D prevPos = m_position;
    Vector3D dir = m_player->dir();
    Vector3D vel = m_player->velocity();
    if (m_forwards == 100)
        vel += dir*100.0f;
    if (m_backwards == 100)
        vel -= dir*100.0f;
    m_yaw = glm::atan(-dir.x, -dir.y);
    m_pitch = dir.z < -1.0f ? -glm::half_pi<float>() : dir.z > 1.0f ? glm::half_pi<float>() : glm::asin(dir.z);
    m_position += vel*elapsedTime;
    m_speed = glm::length(vel)*3.6f;
    if (glm::dot(dir, vel) < 0)
        m_speed = -m_speed;

    /*Vector3D up = Vector3D(glm::row(m_cameraMatrix, 1));
    if (up.z < 0)
    {
        m_cameraMatrix = glm::rotate(m_cameraMatrix, glm::pi<float>(), dir);
        m_inverseUpDown = -1.0f;
    }*/

    if (m_position != prevPos)
    {
        Vector3D pos, normal;
        if (m_surface.testCollision(prevPos, m_position, 1.5f, pos, normal))
            m_position = pos;

        Object *collision = m_collisionManager.testCollision(prevPos, m_position, 1.5f, pos, normal);
        if (collision)
        {
            ActiveObject *activeObject = dynamic_cast<ActiveObject *>(collision);
            if (activeObject)
            {
                activeObject->damage(m_player->kineticStrength(), m_player->shockStrength(), pos - normal*1.5f);
                m_player->damage(activeObject->kineticStrength(), activeObject->shockStrength(), pos - normal*1.5f);
            }
            m_position = pos;
        }

        m_height = m_position.z - m_surface.heightAt(m_position.x, m_position.y);
        m_player->setPosition(m_position);
    }

    m_cameraMatrixInverted = Matrix(Vector4D(m_player->right(), 0), Vector4D(m_player->up(), 0), Vector4D(-m_player->dir(), 0), Vector4D(m_position, 1));
    m_cameraMatrix = glm::inverse(m_cameraMatrixInverted);

    m_conditionManager.update(elapsedTime);

    float height = m_surface.heightAt(m_position.x, m_position.y);
    m_conditionManager.testSpace(m_position.x, m_position.y, m_position.z - height);

    for (const auto &object : m_objects)
        if (object->isEnabled())
            object->update(elapsedTime);

    m_effectManager.update(elapsedTime);

    m_sonar.update(elapsedTime);
}


void Scenario::draw()
{
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);

    glClearColor(0.0, 0.0, 0.15, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(glm::value_ptr(m_projectionMatrix));

    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(glm::value_ptr(m_cameraMatrix));

    GLfloat global_ambient[] = { 0.5f, 0.5f, 1.0f, 1.0f };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);

    glEnable(GL_LIGHTING);

    GLfloat light_ambient[] = { 6.0, 6.0, 6.0, 1.0 };
    GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    
    GLfloat light_position[] = { 0.0, 0.0, 4.0, 1.0 };
    GLfloat spot_direction[] = { 0.0, 0.0, -1.0 };

    glPushMatrix();
    glLoadIdentity();
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);
    glPopMatrix();

    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 90.0);
    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.1);
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.0002);
    glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 4.0);

    glEnable(GL_LIGHT0);

    if (m_lightSources.size() > 0)
    {
        Object *object = m_lightSources[0];

        glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
        glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
        
        glLightfv(GL_LIGHT1, GL_POSITION, glm::value_ptr(object->position()));
        glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, glm::value_ptr(Vector3D(glm::cos(0.005f*m_time), glm::sin(0.005f*m_time), 0.0f)));

        glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 90.0);
        glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 0.1);
        glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.0002);
        glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 4.0);

        glEnable(GL_LIGHT1);
    }

    glEnable(GL_FOG);
    GLfloat fogColor[4] = {0.0, 0.0, 0.15, 1.0};
    glFogi(GL_FOG_MODE, GL_LINEAR);
    glFogfv(GL_FOG_COLOR, fogColor);
    glHint(GL_FOG_HINT, GL_FASTEST);
    glFogf(GL_FOG_START, 100.0);
    glFogf(GL_FOG_END, 200.0);

    m_surface.draw(m_position, m_player->dir());

    for (const auto &object : m_objects)
        if (object->isEnabled())
            object->draw();

    m_effectManager.draw();
}

static float pitch = 1.0f;
void Scenario::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Left || e->key() == Qt::Key_4)
        m_left = 1.0f;
    if (e->key() == Qt::Key_Right || e->key() == Qt::Key_6)
        m_right = 1.0f;
    if (e->key() == Qt::Key_Up || e->key() == Qt::Key_8)
        m_up = 1.0f;
    if (e->key() == Qt::Key_Down || e->key() == Qt::Key_5)
        m_down = 1.0f;
    if (e->key() == Qt::Key_Q)
        m_forwards = 100;
    if (e->key() == Qt::Key_W)
        m_backwards = 100;
    if (e->key() == Qt::Key_A)
    {
        m_forwards = 1;
        m_player->setVelocityTarget(m_player->maxVelocity());
        m_player->setFullThrottle(true);
    }
    if (e->key() == Qt::Key_Z)
    {
        m_backwards = 1;
        m_player->setVelocityTarget(m_player->minVelocity());
        m_player->setFullThrottle(true);
    }
    if (e->key() == Qt::Key_S)
    {
        m_velocityTarget += 2.778f;
        if (m_velocityTarget > m_player->maxVelocity())
            m_velocityTarget = m_player->maxVelocity();
        if (m_forwards != 1 && m_backwards != 1)
            m_player->setVelocityTarget(m_velocityTarget);
    }
    if (e->key() == Qt::Key_X)
    {
        m_velocityTarget -= 2.778f;
        if (m_velocityTarget < m_player->minVelocity())
            m_velocityTarget = m_player->minVelocity();
        if (m_forwards != 1 && m_backwards != 1)
            m_player->setVelocityTarget(m_velocityTarget);
    }
    if (e->key() == Qt::Key_C)
    {
        m_velocityTarget = 0;
        if (m_forwards != 1 && m_backwards != 1)
            m_player->setVelocityTarget(m_velocityTarget);
    }
    if (e->key() == Qt::Key_QuoteLeft)
        m_player->engineToggle();
    if (e->key() == Qt::Key_Space)
        m_player->fire();
}


void Scenario::keyReleaseEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Left || e->key() == Qt::Key_4)
        m_left = 0.0f;
    if (e->key() == Qt::Key_Right || e->key() == Qt::Key_6)
        m_right = 0.0f;
    if (e->key() == Qt::Key_Up || e->key() == Qt::Key_8)
        m_up = 0.0f;
    if (e->key() == Qt::Key_Down || e->key() == Qt::Key_5)
        m_down = 0.0f;
    if (e->key() == Qt::Key_Q)
        m_forwards = 0;
    if (e->key() == Qt::Key_W)
        m_backwards = 0;
    if (e->key() == Qt::Key_A)
    {
        m_forwards = 0;
        if (m_backwards > 0)
            m_player->setVelocityTarget(m_player->minVelocity());
        else
        {
            m_player->setVelocityTarget(m_velocityTarget);
            m_player->setFullThrottle(false);
        }
    }
    if (e->key() == Qt::Key_Z)
    {
        m_backwards = 0;
        if (m_forwards > 0)
            m_player->setVelocityTarget(m_player->maxVelocity());
        else
        {
            m_player->setVelocityTarget(m_velocityTarget);
            m_player->setFullThrottle(false);
        }
    }
    if (e->key() == Qt::Key_Space)
        m_player->fireStop();
}


Vector3D Scenario::getPosition()
{
    Vector3D pos;
    pos.x = m_file.value("px").toInt() + 0.5f;
    pos.y = m_file.value("py").toInt() + 0.5f;
    pos.z = m_file.value("pz").toInt() + m_file.value("hei").toInt();

    pos *= m_surface.scale();
    pos.z += m_surface.heightAt(pos.x, pos.y);

    return pos;
}


} // namespace fight
