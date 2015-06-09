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
#include "effects/effectmanager.h"
#include "effects/trash.h"
#include "collisionmanager.h"
#include "turretbase.h"
#include "navpoint.h"
#include "conditionmanager.h"
#include <QGLContext>
#include <QKeyEvent>
#include <cmath>

#ifndef GLU_VERSION
#include <GL/glu.h>
#endif // GLU_VERSION


namespace fight {


Scenario::Scenario(const QString &name, std::function<void()> &&funcSuccess) :
    m_moduleManager(m_textureManager),
    m_left(0.0f),
    m_right(0.0f),
    m_up(0.0f),
    m_down(0.0f),
    m_forwards(0.0f),
    m_backwards(0.0f),
    m_condAutopilot(this),
    m_condFailure(this),
    m_funcSuccess(std::move(funcSuccess))
{
    m_effectManager = new EffectManager(this);
    m_collisionManager = new CollisionManager();

    qsrand(name.right(4).toInt());

    hideCursor();
    m_file.load(QString("vfx:scenario/%1.des").arg(name));

    m_file.setSection("surface");
    m_surface = new Surface(m_file.value("name").toString().toLower(), m_file.value("maxheightscale").toInt(), m_file.value("patchcomb").toInt());

    QMap<int, QString> types;
    types.insert( 0, "anscout2");
    types.insert( 1, "anscout2");
    types.insert( 5, "guntow0");
    types.insert( 7, "guntow2");
    types.insert( 8, "bioscout");
    types.insert(16, "anbombr1");
    types.insert(10, "russcout");
    types.insert(12, "atscout");
    types.insert(27, "mine0");
    types.insert(36, "tortow0");
    types.insert(39, "anscout1");
    types.insert(41, "atbomber");
    types.insert(47, "build2");
    types.insert(48, "build3");
    types.insert(50, "build5");
    types.insert(67, "entrobot");

    QMap<int, ConditionEntry> condEntries;

    float initialDir = 0.0f;
    foreach (const QString &section, m_file.sections().filter(QRegExp("^movable\\d*"))) {
        m_file.setSection(section);

        const int id = m_file.value("id").toInt();
        ConditionEntry entry;
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
                if (!types.contains(dType)) {
                    qDebug() << "Unhandled dtype" << dType;
                    continue;
                }

                object = new Object(this, types.value(dType));
                QVector3D pos = getPosition();
                pos.setZ(pos.z() + 20.0f);
                object->setPosition(pos);
            }
            break;

        case TypeBomber:
            {
                const int dType = m_file.value("dtyp").toInt();
                if (!types.contains(dType)) {
                    qDebug() << "Unhandled dtype" << dType;
                    continue;
                }

                object = new Object(this, types.value(dType));
                QVector3D pos = getPosition();
                pos.setZ(pos.z() + 20.0f);
                object->setPosition(pos);
            }
            break;

        case TypeTower:
        case TypeTorpedoTower:
            {
                const int dType = m_file.value("dtyp").toInt();
                if (!types.contains(dType)) {
                    qDebug() << "Unhandled dtype" << dType;
                    continue;
                }

                object = new TurretBase(this, types.value(dType));
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
                if (!types.contains(dType)) {
                    qDebug() << "Unhandled dtype" << dType;
                    continue;
                }

                object = new Mine(this, types.value(dType));
                object->setPosition(getPosition());
            }
            break;

        case TypePlayer:
            m_position = getPosition();
            initialDir = 45.0f * m_file.value("card").toInt();
            //m_position.setZ(m_position.z() + 20.0f);

            {
                for (int i = 0; i < 27; i++)
                {
                    Effect *effect = m_effectManager->create((Effects)(Explosion_0 + i));
                    effect->setPosition(m_position + QVector3D(i*5, 0, 0));
                    effect->setPermanent(true);
                    m_objects << effect;
                }
                for (int i = 0; i < 9; i++)
                {
                    Effect *effect = m_effectManager->create((Effects)(Shoot_0 + i));
                    effect->setPosition(m_position + QVector3D(i*5, -10, 0));
                    effect->setPermanent(true);
                    m_objects << effect;
                }
                for (int i = 0; i < 23; i++)
                {
                    Effect *effect = m_effectManager->create((Effects)(Debris_0 + i));
                    effect->setPosition(m_position + QVector3D(i*5, -20, 0));
                    effect->setPermanent(true);
                    m_objects << effect;
                }
                for (int i = 0; i < 5; i++)
                {
                    Effect *effect = m_effectManager->create((Effects)(Trash_0 + i));
                    effect->setPosition(m_position + QVector3D(i*5, -30, 0));
                    effect->setPermanent(true);
                    m_objects << effect;
                }
                for (int i = 0; i < 3; i++)
                {
                    Effect *effect = m_effectManager->create((Effects)(Bubble_0 + i));
                    effect->setPosition(m_position + QVector3D(i*5, -40, 0));
                    effect->setPermanent(true);
                    m_objects << effect;
                }
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
                object->setPosition(getPosition() + QVector3D(-0.5f*m_surface->scale().x(), -0.5f*m_surface->scale().y(), 12));
            }
            break;

        case TypeActiveBuilding:
            {
                const int dType = m_file.value("dtyp").toInt();
                if (!types.contains(dType)) {
                    qDebug() << "Unhandled dtype" << dType;
                    continue;
                }

                object = new Object(this, types.value(dType), 16);
                object->setPosition(getPosition());
            }
            break;

        case TypeTrash:
            {
                QVector3D pos = getPosition();
                if (entry.cond1 != 0 || entry.del != 0)
                    entry.condTrigger = new Condition(0); // TODO: Fix memory leak
                entry.condSignal = new Condition(9); // TODO: Fix memory leak
                for (int i = 0; i < 9; i++)
                {
                    Object *trash = m_effectManager->createTrash(Trash::trashCollection[i], pos);
                    m_objects << trash;
                    if (entry.condTrigger != NULL)
                    {
                        trash->disable();
                        trash->condEnable()->setLimit(1);
                        entry.condTrigger->addDependency(trash->condEnable());
                    }
                    trash->eventDestroy()->addDependency((Condition *)entry.condSignal);
                }
            }
            break;

        case TypeSpace:
            {
                if (m_file.value("pz").toInt() != 0)
                    qDebug() << "Unexpected space parameter";
                ConditionSpace *space = new ConditionSpace(m_file.value("px").toInt()*m_surface->scale().x(),
                                                           m_file.value("py").toInt()*m_surface->scale().y(),
                                                           m_file.value("dimx").toInt()*m_surface->scale().x(),
                                                           m_file.value("dimy").toInt()*m_surface->scale().y(),
                                                           m_file.value("minz").toInt()*m_surface->scale().z(),
                                                           m_file.value("maxz").toInt()*m_surface->scale().z());
                if (entry.cond1 != 0 || entry.del != 0)
                    entry.condTrigger = space->condEnable();
                else
                    space->condEnable()->complete();
                entry.condSignal = space;
                m_condSpaces.append(space);
            }
            break;

        default:
            qDebug() << "Unhandled movable" << type << m_file.value("dtyp").toInt();;
        }

        if (object != NULL)
        {
            m_objects << object;
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
        condEntries.insert(id, entry);

        // Reading Objectives
        for (int i = 0; i < 16; i++)
        {
            const int wccond = m_file.value(QString("wccond%1").arg(i)).toInt();
            if (wccond == 0)
                continue;
            ConditionEvent *condSuccess = NULL;
            ConditionEvent *condFailure = NULL;
            switch (wccond)
            {
            case 1:
                condSuccess = entry.condTrigger;
                break;
            case 2:
                condSuccess = entry.condSignal;
                break;
            case 3:
                condSuccess = entry.condAttacked;
                break;
            case 4:
                condSuccess = entry.condIdentified;
                break;
            case 5:
                condFailure = entry.condSignal;
                break;
            case 6:
                condSuccess = entry.condParalyzed;
                condFailure = entry.condSignal;
                break;
            case 7:
                condSuccess = entry.condFinished;
                condFailure = entry.condSignal;
                break;
            case 8:
                condSuccess = entry.condBoarded;
                condFailure = entry.condSignal;
                break;
            case 9:
                condSuccess = entry.condSignal;
                condFailure = entry.condSignal;
                break;
            default:
                qDebug() << "Unhandled objective condition" << id << wccond;
            }
            if (condSuccess == NULL && condFailure == NULL)
                qDebug() << "Unachievable objective" << id << i;
            if (condSuccess != NULL)
            {
                int objective = i*2;
                if (!m_condObjectives.contains(objective))
                {
                    m_condObjectives.insert(objective, Condition(1));
                    if (objective < 16)
                    {
                        m_condObjectives[objective].addDependency(&m_condAutopilot);
                        m_condAutopilot.setLimit(m_condAutopilot.limit() + 1);
                    }
                }
                else if (wccond == 2 || wccond == 9)
                    m_condObjectives[objective].setLimit(m_condObjectives[objective].limit() + 1);
                condSuccess->addDependency(&m_condObjectives[objective]);
            }
            if (condFailure != NULL)
            {
                int objective = i*2 + 1;
                if (!m_condObjectives.contains(objective))
                {
                    m_condObjectives.insert(objective, Condition(1));
                    if (objective < 16)
                        m_condObjectives[objective].addDependency(&m_condFailure);
                }
                else if (wccond == 9)
                    m_condObjectives[objective].setLimit(m_condObjectives[objective].limit() + 1);
                condFailure->addDependency(&m_condObjectives[objective]);
                if (wccond == 6 || wccond == 8)
                {
                    condSuccess->addDependency(&m_condObjectives[objective], false);
                    m_condObjectives[objective].setLimit(2);
                }
            }
        }
    }

    // Building Condition dependencies
    QMapIterator<int, ConditionEntry> it(condEntries);
    while (it.hasNext())
    {
        it.next();

        if (it.value().cond1 == 0 && it.value().cond2 != 0)
            qDebug() << "Abnormal condition" << it.key();
        if (it.value().cond1 == 0)
        {
            if (it.value().del != 0)
                ConditionManager::delayComplete(it.value().condTrigger, it.value().del);
            continue;
        }

        initCondition(condEntries, it.value().cond1, it.value().dep1, it.value().ref1, it.value().condTrigger);
        if (it.value().cond2 != 0)
            initCondition(condEntries, it.value().cond2, it.value().dep2, it.value().ref2, it.value().condTrigger);
        it.value().condTrigger->setLimit(it.value().cond2 != 0 && it.value().op == 0 ? 2 : 1);
        if (it.value().op != 0 && it.value().op != 1)
            qDebug() << "Unhandled condition operation" << it.value().op;
        it.value().condTrigger->setDelay(it.value().del);
    }


    m_cameraMatrix.rotate(initialDir, 0, 1, 0);
    m_cameraMatrix.rotate(-90, 1, 0, 0);

    m_time.restart();
    qsrand(QTime::currentTime().second() * 1000 + QTime::currentTime().msec());
}


Scenario::~Scenario()
{
    delete m_surface;
    delete m_collisionManager;
    delete m_effectManager;
    qDeleteAll(m_objects);
    qDeleteAll(m_condSpaces);
}


void Scenario::draw()
{
    const float angleY = (m_right - m_left) * 2.0f;
    const float angleX = (m_up - m_down) * 2.0f;

    m_cameraMatrix.rotate(angleX, m_cameraMatrix.row(0).toVector3D());
    m_cameraMatrix.rotate(angleY, QVector3D(0, 0, 1));
    m_cameraMatrixInverted = m_cameraMatrix.inverted();

    QVector3D prevPos = m_position;
    m_position += m_cameraMatrix.row(2).toVector3D() * (m_backwards - m_forwards) * 5.0f;

    if (m_position != prevPos)
    {
        QVector3D pos, normal;
        if (m_surface->testCollision(prevPos, m_position, 1.5f, pos, normal))
            m_position = pos;
        Object *collision = m_collisionManager->testCollision(prevPos, m_position, 1.5f, pos, normal);
        if (collision)
        {
            if (collision->type() == TrashObject)
                collision->destroy();
            m_position = pos;
        }
    }

    ConditionManager::update();

    float height = m_surface->heightAt(m_position.x(), m_position.y());
    foreach (ConditionSpace *space, m_condSpaces)
        space->test(m_position.x(), m_position.y(), m_position.z() - height);

    foreach (Object *object, m_objects)
        if (object->isEnabled())
            object->update();

    m_effectManager->update();


    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);

    glClearColor(0.0, 0.0, 0.15, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, float(rect().width()) / rect().height(), 0.1f, 10000.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixd(m_cameraMatrix.data());
    glTranslatef(-m_position.x(), -m_position.y(), -m_position.z());

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

    if (m_lightSources.count() > 0)
    {
        Object *object = m_lightSources[0];

        glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
        glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
        
        const GLfloat light_position[] = { static_cast<float>(object->position().x()), static_cast<float>(object->position().y()), static_cast<float>(object->position().z()), 1.0f };
        const GLfloat spot_direction[] = { std::cos(0.005f*m_time.elapsed()), std::sin(0.005f*m_time.elapsed()), 0.0f };

        glLightfv(GL_LIGHT1, GL_POSITION, light_position);
        glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction);

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

    m_surface->draw(m_position, -m_cameraMatrix.row(2).toVector3D());

    foreach (Object *object, m_objects)
        if (object->isEnabled())
            object->draw();

    m_effectManager->draw();
}


void Scenario::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Escape)
        m_funcSuccess();

    if (e->key() == Qt::Key_Left)
        m_left = 1.0f;
    if (e->key() == Qt::Key_Right)
        m_right = 1.0f;
    if (e->key() == Qt::Key_Up)
        m_up = 1.0f;
    if (e->key() == Qt::Key_Down)
        m_down = 1.0f;
    if (e->key() == Qt::Key_A)
        m_forwards = 1.0f;
    if (e->key() == Qt::Key_Z || e->key() == Qt::Key_Y)
        m_backwards = 1.0f;
    if (e->key() == Qt::Key_S)
        m_forwards = 0.2f;
    if (e->key() == Qt::Key_X)
        m_backwards = 0.2f;

    if (e->key() == Qt::Key_Space)
        m_effectManager->addProjectile(Shoot_Vendetta, m_position, -m_cameraMatrix.row(2).toVector3D());
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


QVector3D Scenario::getPosition() const
{
    QVector3D pos;
    pos.setX(m_file.value("px").toInt() + 0.5f);
    pos.setY(m_file.value("py").toInt() + 0.5f);
    pos.setZ(m_file.value("pz").toInt() + m_file.value("hei").toInt());

    pos *= m_surface->scale();
    pos += QVector3D(0, 0, m_surface->heightAt(pos.x(), pos.y()));

    return pos;
}


void Scenario::initCondition(const QMap<int, ConditionEntry> &entries, int cond, int dep, int ref, Condition *condDepend)
{
    if (cond == 1 || cond == 39)
    {
        if (dep != 1)
            qDebug() << "Unhandled condition dep" << cond << dep;
        if (!entries.contains(ref))
            qDebug() << "Condition reference not found" << ref;
        entries[ref].condSignal->addDependency(condDepend, cond == 1);
    }
    if (cond == 2 || cond == 40)
    {
        if (dep != 1)
            qDebug() << "Unhandled condition dep" << cond << dep;
        if (!entries.contains(ref))
            qDebug() << "Condition reference not found" << ref;
        entries[ref].condAttacked->addDependency(condDepend, cond == 2);
    }
    if (cond == 3 || cond == 41)
    {
        if (dep != 1)
            qDebug() << "Unhandled condition dep" << cond << dep;
        if (!entries.contains(ref))
            qDebug() << "Condition reference not found" << ref;
        entries[ref].condIdentified->addDependency(condDepend, cond == 3);
    }
    if (cond == 4)
    {
        if (dep != 0)
            qDebug() << "Unhandled condition dep" << cond << dep;
        if (ref != -1)
            qDebug() << "Unexpected reference" << cond << ref;
        m_condAutopilot.addDependency(condDepend);
    }
    if (cond == 5)
    {
        if (dep != 0)
            qDebug() << "Unhandled condition dep" << cond << dep;
        if (ref != -1)
            qDebug() << "Unexpected reference" << cond << ref;
        m_condFailure.addDependency(condDepend);
    }
    if (cond >= 6 && cond <= 37)
    {
        int objective = cond - 6;
        if (!m_condObjectives.contains(objective))
            qDebug() << "Objective not found" << cond;
        if (dep != 0)
            qDebug() << "Unhandled condition dep" << cond << dep;
        if (ref != -1)
            qDebug() << "Unexpected reference" << cond << ref;
        m_condObjectives[objective].addDependency(condDepend);
    }
    if (cond == 38)
    {
        if (dep != 1)
            qDebug() << "Unhandled condition dep" << cond << dep;
        if (!entries.contains(ref))
            qDebug() << "Condition reference not found" << ref;
        entries[ref].condParalyzed->addDependency(condDepend);
    }
    if (cond == 43 || cond == 44)
    {
        if (dep != 1)
            qDebug() << "Unhandled condition dep" << cond << dep;
        if (!entries.contains(ref))
            qDebug() << "Condition reference not found" << ref;
        entries[ref].condFinished->addDependency(condDepend, cond == 43);
    }
    if (cond == 45 || cond == 46)
    {
        if (dep != 1)
            qDebug() << "Unhandled condition dep" << cond << dep;
        if (!entries.contains(ref))
            qDebug() << "Condition reference not found" << ref;
        entries[ref].condBoarded->addDependency(condDepend, cond == 45);
    }
}


} // namespace fight
