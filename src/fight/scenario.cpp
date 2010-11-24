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
#include "turretbase.h"
#include "navpoint.h"
#include <QGLContext>
#include <QKeyEvent>
#include "math.h"


namespace fight {


Scenario::Scenario(const QString &name) :
    m_moduleManager(m_textureManager),
    m_left(0.0f),
    m_right(0.0f),
    m_up(0.0f),
    m_down(0.0f),
    m_forwards(0.0f),
    m_backwards(0.0f)
{
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

    float initialDir = 0.0f;
    foreach (const QString &section, m_file.sections().filter(QRegExp("^movable\\d*"))) {
        m_file.setSection(section);

        const int type = m_file.value("typ").toInt();
        switch (type) {
        case TypeBoat:
            {
                const int dType = m_file.value("dtyp").toInt();
                if (!types.contains(dType)) {
                    qDebug() << "Unhandled dtype" << dType;
                    continue;
                }

                Object *object = new Object(m_moduleManager, types.value(dType));
                QVector3D pos = getPosition();
                pos.setZ(pos.z() + 20.0f);
                object->setPosition(pos);
                m_objects << object;
                m_lightSources << object;
            }
            break;

        case TypeBomber:
            {
                const int dType = m_file.value("dtyp").toInt();
                if (!types.contains(dType)) {
                    qDebug() << "Unhandled dtype" << dType;
                    continue;
                }

                Object *object = new Object(m_moduleManager, types.value(dType));
                QVector3D pos = getPosition();
                pos.setZ(pos.z() + 20.0f);
                object->setPosition(pos);
                m_objects << object;
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

                Object *object = new TurretBase(m_moduleManager, types.value(dType));
                object->setPosition(getPosition());
                m_objects << object;
            }
            break;

        case TypeBuilding:
            {
                Object *object = new Building(m_moduleManager, QString("gp%1x%1_%2").arg(m_file.value("siz").toInt() + 1).arg(m_file.value("buityp").toInt()), m_file.value("siz").toInt(), m_file.value("card", 0).toInt() * 45.0f, m_surface, m_file.value("px").toInt(), m_file.value("py").toInt(), m_file.value("refx").toInt(), m_file.value("refy").toInt());
                object->setPosition(getPosition());
                m_objects << object;
            }
            break;

        case TypeMine:
            {
                const int dType = m_file.value("dtyp").toInt();
                if (!types.contains(dType)) {
                    qDebug() << "Unhandled dtype" << dType;
                    continue;
                }

                Object *object = new Mine(m_moduleManager, types.value(dType));
                object->setPosition(getPosition());
                m_objects << object;
            }
            break;

        case TypePlayer:
            m_position = getPosition();
            initialDir = 45.0f * m_file.value("card").toInt();
            //m_position.setZ(m_position.z() + 20.0f);
            break;

        case TypeCrawler:
            {
                Object *object = new Object(m_moduleManager, "gvehicle");
                object->setPosition(getPosition());
                m_objects << object;
            }
            break;

        case TypeNavPoint:
            {
                Object *object = new NavPoint(m_moduleManager, m_file.value("dtyp").toInt());
                object->setPosition(getPosition() + QVector3D(-0.5f*m_surface->scale().x(), -0.5f*m_surface->scale().y(), 12));
                m_objects << object;
            }
            break;

        case TypeActiveBuilding:
            {
                const int dType = m_file.value("dtyp").toInt();
                if (!types.contains(dType)) {
                    qDebug() << "Unhandled dtype" << dType;
                    continue;
                }

                Object *object = new Object(m_moduleManager, types.value(dType), 16);
                object->setPosition(getPosition());
                m_objects << object;
            }
            break;

        default:
            qDebug() << "Unhandled movable" << type << m_file.value("dtyp").toInt();;
        }
    }

    m_cameraMatrix.rotate(initialDir, 0, 1, 0);
    m_cameraMatrix.rotate(-90, 1, 0, 0);

    m_time.restart();
}


Scenario::~Scenario()
{
    delete m_surface;
}


void Scenario::draw()
{
    const float angleY = (m_right - m_left) * 2.0f;
    const float angleX = (m_up - m_down) * 2.0f;

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);

    m_cameraMatrix.rotate(angleX, m_cameraMatrix.row(0).toVector3D());
    m_cameraMatrix.rotate(angleY, QVector3D(0, 0, 1));
    m_position += m_cameraMatrix.row(2).toVector3D() * (m_backwards - m_forwards) * 5.0f;

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
    
    GLfloat light_position[] = { 0.0, 0.0, 0.0, 1.0 };
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
        
        GLfloat light_position[] = { object->position().x(), object->position().y(), object->position().z(), 1.0 };
        GLfloat spot_direction[] = { cos(0.005*m_time.elapsed()), sin(0.005*m_time.elapsed()), 0.0 };

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
        object->draw();
}


void Scenario::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Escape)
        emit success();

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


} // namespace fight
