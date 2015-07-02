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

#include "building.h"
#include "mine.h"
#include "module.h"
#include "scenario.h"
#include "surface/surface.h"
#include "effects/effect.h"
#include "effects/projectile.h"
#include "effects/trash.h"
#include "turretbase.h"
#include "navpoint.h"
#include "conditionmanager.h"
#include <QGLContext>
#include <QKeyEvent>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>


namespace fight {


Scenario::Scenario(const QString &name) :
    m_moduleManager(m_textureManager),
    m_effectManager(this),
    m_conditionManager(this),
    m_time(0),
    m_left(0.0f),
    m_right(0.0f),
    m_up(0.0f),
    m_down(0.0f),
    m_forwards(0.0f),
    m_backwards(0.0f),
    m_inverseUpDown(1.0f)
{
    qsrand(name.right(4).toInt());

    m_file.load(QString("vfx:scenario/%1.des").arg(name));

    m_file.setSection("surface");
    m_depth = m_file.value("depth").toInt();
    m_surface.load(m_file.value("name").toString().toLower(), m_file.value("maxheightscale").toInt(), m_file.value("patchcomb").toInt());

    std::map<int, QString> types;
    types[ 0] = "anscout2";
    types[ 1] = "anscout2";
    types[ 5] = "guntow0";
    types[ 7] = "guntow2";
    types[ 8] = "bioscout";
    types[16] = "anbombr1";
    types[10] = "russcout";
    types[12] = "atscout";
    types[27] = "mine0";
    types[36] = "tortow0";
    types[39] = "anscout1";
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
        switch (type) {
        case TypeBoat:
            {
                const int dType = m_file.value("dtyp").toInt();
                if (types.find(dType) == types.end()) {
                    qDebug() << "Unhandled dtype" << dType;
                    continue;
                }

                object = new Object(this, types[dType]);
                glm::vec3 pos = getPosition();
                pos.z += 20.0f;
                object->setPosition(pos);
            }
            break;

        case TypeBomber:
            {
                const int dType = m_file.value("dtyp").toInt();
                if (types.find(dType) == types.end()) {
                    qDebug() << "Unhandled dtype" << dType;
                    continue;
                }

                object = new Object(this, types[dType]);
                glm::vec3 pos = getPosition();
                pos.z += 20.0f;
                object->setPosition(pos);
            }
            break;

        case TypeTower:
        case TypeTorpedoTower:
            {
                const int dType = m_file.value("dtyp").toInt();
                if (types.find(dType) == types.end()) {
                    qDebug() << "Unhandled dtype" << dType;
                    continue;
                }

                object = new TurretBase(this, types[dType]);
                object->setPosition(getPosition());
            }
            break;

        case TypeBuilding:
            {
                object = new Building(this, QString("gp%1x%1_%2").arg(m_file.value("siz").toInt() + 1).arg(m_file.value("buityp").toInt()), m_file.value("siz").toInt(), m_file.value("card", 0).toInt() * 45.0f, m_file.value("px").toInt(), m_file.value("py").toInt(), m_file.value("refx").toInt(), m_file.value("refy").toInt());
                object->setPosition(getPosition());
            }
            break;

        case TypeMine:
            {
                const int dType = m_file.value("dtyp").toInt();
                if (types.find(dType) == types.end()) {
                    qDebug() << "Unhandled dtype" << dType;
                    continue;
                }

                object = new Mine(this, types[dType]);
                object->setPosition(getPosition());
            }
            break;

        case TypePlayer:
            m_position = getPosition();
            initialDir = 45.0f * m_file.value("card").toInt();
            //m_position.setZ(m_position.z() + 20.0f);

            {
                for (int i = 0; i < 27; i++)
                    m_effectManager.addEffect((Effects)(Explosion_0 + i), m_position + glm::vec3(i*5, 0, 0))->setPermanent(true);
                for (int i = 0; i < 9; i++)
                    m_effectManager.addEffect((Effects)(Shoot_0 + i), m_position + glm::vec3(i*5, -10, 0))->setPermanent(true);
                for (int i = 0; i < 23; i++)
                    m_effectManager.addEffect((Effects)(Debris_0 + i), m_position + glm::vec3(i*5, -20, 0))->setPermanent(true);
                for (int i = 0; i < 5; i++)
                    m_effectManager.addEffect((Effects)(Trash_0 + i), m_position + glm::vec3(i*5, -30, 0))->setPermanent(true);
                for (int i = 0; i < 3; i++)
                    m_effectManager.addEffect((Effects)(Bubble_0 + i), m_position + glm::vec3(i*5, -40, 0))->setPermanent(true);
            }
            break;

        case TypeCrawler:
            {
                object = new Object(this, "gvehicle");
                object->setPosition(getPosition());
            }
            break;

        case TypeNavPoint:
            {
                object = new NavPoint(this, m_file.value("dtyp").toInt());
                object->setPosition(getPosition() + glm::vec3(-0.5f*m_surface.scale().x, -0.5f*m_surface.scale().y, 12));
            }
            break;

        case TypeActiveBuilding:
            {
                const int dType = m_file.value("dtyp").toInt();
                if (types.find(dType) == types.end()) {
                    qDebug() << "Unhandled dtype" << dType;
                    continue;
                }

                object = new Object(this, types[dType], 16);
                object->setPosition(getPosition());
            }
            break;

        case TypeTrash:
            {
                glm::vec3 pos = getPosition();
                if (entry.cond1 != 0 || entry.del != 0)
                    entry.condTrigger = m_conditionManager.addCondition(0);
                entry.condSignal = m_conditionManager.addCondition(9);
                for (int i = 0; i < 9; i++)
                {
                    Trash *trash = new Trash(this, m_effectManager.getBillboard(Trash::trashCollection[i]), pos);
                    m_objects.emplace_back(trash);
                    if (entry.condTrigger != NULL)
                    {
                        trash->disable();
                        trash->condEnable()->setLimit(1);
                        entry.condTrigger->addDependency(trash->condEnable());
                    }
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
                if (entry.cond1 != 0 || entry.del != 0)
                    entry.condTrigger = space->condEnable();
                else
                    space->condEnable()->complete();
                entry.condSignal = space;
            }
            break;

        default:
            qDebug() << "Unhandled movable" << type << m_file.value("dtyp").toInt();;
        }

        if (object != NULL)
        {
            m_objects.emplace_back(object);
            if (entry.cond1 != 0 || entry.del != 0)
                object->disable();
            entry.condTrigger = object->condEnable();
            entry.condSignal = object->eventDestroy();
            entry.condAttacked = object->eventAttack();
            entry.condIdentified = object->eventIdentify();
            entry.condParalyzed = object->eventParalyze();
            entry.condFinished = object->eventFinish();
            entry.condBoarded = object->eventBoard();
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

    m_conditionManager.buildDependencies();


    m_cameraMatrix = glm::rotate(m_cameraMatrix, glm::radians(initialDir), glm::vec3(0, 1, 0));
    m_cameraMatrix = glm::rotate(m_cameraMatrix, glm::radians(-90.0f), glm::vec3(1, 0, 0));

    qsrand(QTime::currentTime().second() * 1000 + QTime::currentTime().msec());
}


void Scenario::setRect(const util::RectF &rect, const glm::vec2 &center)
{
    m_projectionMatrix = glm::translate(glm::vec3(center, 0)) * glm::perspective(glm::radians(60.0f), float(rect.width / rect.height), 0.1f, 10000.0f);
    m_projectionMatrixInverted = glm::inverse(m_projectionMatrix);
}


void Scenario::update(float elapsedTime)
{
    m_time += elapsedTime;

    const float angleY = (m_right - m_left) * elapsedTime * 0.09f;
    const float angleX = (m_up - m_down) * m_inverseUpDown * elapsedTime * 0.09f;

    m_cameraMatrix = glm::rotate(m_cameraMatrix, glm::radians(angleX), glm::vec3(glm::row(m_cameraMatrix, 0)));
    m_cameraMatrix = glm::rotate(m_cameraMatrix, glm::radians(angleY), glm::vec3(0, 0, 1));
    m_cameraMatrixInverted = glm::inverse(m_cameraMatrix);

    glm::vec3 prevPos = m_position;
    glm::vec3 dir = glm::vec3(glm::row(m_cameraMatrix, 2));
    m_position += dir * (m_backwards - m_forwards) * elapsedTime * 0.2f;
    m_yaw = glm::atan(dir.x, dir.y);
    m_pitch = dir.z < -1.0f ? -glm::half_pi<float>() : dir.z > 1.0f ? glm::half_pi<float>() : glm::asin(dir.z);

    glm::vec3 up = glm::vec3(glm::row(m_cameraMatrix, 1));
    if (up.z < 0)
    {
        m_cameraMatrix = glm::rotate(m_cameraMatrix, glm::pi<float>(), dir);
        m_inverseUpDown = -1.0f;
    }

    if (m_position != prevPos)
    {
        glm::vec3 pos, normal;
        if (m_surface.testCollision(prevPos, m_position, 1.5f, pos, normal))
            m_position = pos;
        float height = m_surface.heightAt(m_position.x, m_position.y);
        qDebug() << height << m_position.z;
        Object *collision = m_collisionManager.testCollision(prevPos, m_position, 1.5f, pos, normal);
        if (collision)
        {
            if (collision->type() == TrashObject)
                collision->destroy();
            m_position = pos;
        }
    }

    m_conditionManager.update(elapsedTime);

    float height = m_surface.heightAt(m_position.x, m_position.y);
    m_conditionManager.testSpace(m_position.x, m_position.y, m_position.z - height);

    for (const auto &object : m_objects)
        if (object->isEnabled())
            object->update(elapsedTime);

    m_effectManager.update(elapsedTime);
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
    glTranslatef(-m_position.x, -m_position.y, -m_position.z);

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
        glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, glm::value_ptr(glm::vec3(glm::cos(0.005f*m_time), glm::sin(0.005f*m_time), 0.0f)));

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

    glm::vec3 dir = -glm::vec3(glm::row(m_cameraMatrix, 2));
    m_surface.draw(m_position, dir);

    for (const auto &object : m_objects)
        if (object->isEnabled())
            object->draw();

    m_effectManager.draw();
}


void Scenario::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Left)
        m_left = 1.0f;
    if (e->key() == Qt::Key_Right)
        m_right = 1.0f;
    if (e->key() == Qt::Key_Up)
    {
        m_up = 1.0f;
        m_inverseUpDown = 1.0f;
    }
    if (e->key() == Qt::Key_Down)
    {
        m_down = 1.0f;
        m_inverseUpDown = 1.0f;
    }
    if (e->key() == Qt::Key_A)
        m_forwards = 1.0f;
    if (e->key() == Qt::Key_Z || e->key() == Qt::Key_Y)
        m_backwards = 1.0f;
    if (e->key() == Qt::Key_S)
        m_forwards = 0.2f;
    if (e->key() == Qt::Key_X)
        m_backwards = 0.2f;

    if (e->key() == Qt::Key_Space)
    {
        glm::vec3 dir = -glm::vec3(glm::row(m_cameraMatrix, 2));
        m_effectManager.addProjectile(Shoot_Vendetta, m_position, dir);
    }
}


void Scenario::keyReleaseEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Left)
        m_left = 0.0f;
    if (e->key() == Qt::Key_Right)
        m_right = 0.0f;
    if (e->key() == Qt::Key_Up)
        m_up = 0.0f;
    if (e->key() == Qt::Key_Down)
        m_down = 0.0f;
    if (e->key() == Qt::Key_A)
        m_forwards = 0.0f;
    if (e->key() == Qt::Key_Z || e->key() == Qt::Key_Y)
        m_backwards = 0.0f;
    if (e->key() == Qt::Key_S)
        m_forwards = 0.0f;
    if (e->key() == Qt::Key_X)
        m_backwards = 0.0f;
}


glm::vec3 Scenario::getPosition()
{
    glm::vec3 pos;
    pos.x = m_file.value("px").toInt() + 0.5f;
    pos.y = m_file.value("py").toInt() + 0.5f;
    pos.z = m_file.value("pz").toInt() + m_file.value("hei").toInt();

    pos *= m_surface.scale();
    m_surface.heightAt(pos.x, pos.y);
    pos.z += m_surface.heightAt(pos.x, pos.y);

    return pos;
}


} // namespace fight
