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

#include "module.h"
#include "scenario.h"
#include "surface.h"
#include <QGLContext>
#include <QMouseEvent>


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
    m_surface = new Surface(m_file.value("name").toString().toLower(), m_file.value("patchcomb").toInt());

    foreach (const QString &section, m_file.sections().filter(QRegExp("^movable\\d*"))) {
        m_file.setSection(section);

        const int type = m_file.value("typ").toInt();
        switch (type) {
        case TypeBoat:
            {
                const int dType = m_file.value("dtyp").toInt();
                QString name;
                switch (dType) {
                case 1:
                    name = "anscout2";
                    break;

                case 8:
                    name = "bioscout";
                    break;

                case 10:
                    name = "russcout";
                    break;

                case 12:
                    name = "atscout";
                    break;

                case 39:
                    name = "anscout1";
                    break;

                case 67:
                    name = "entrobot";
                    break;
                }

                if (name.isEmpty()) {
                    qDebug() << "Unhandled dtype" << dType;
                    continue;
                }
                Object *object = new Object(m_moduleManager, name);
                object->setPosition(getPosition());
                m_objects << object;
            }
            break;

        case TypeBomber:
            {
                const int dType = m_file.value("dtyp").toInt();
                QString name;
                switch (dType) {
                case 16:
                    name = "anbombr1";
                    break;

                case 41:
                    name = "atbomber";
                    break;
                }

                if (name.isEmpty()) {
                    qDebug() << "Unhandled dtype" << dType;
                    continue;
                }
                Object *object = new Object(m_moduleManager, name);
                object->setPosition(getPosition());
                m_objects << object;
            }
            break;

        case TypePlayer:
            m_position = getPosition();
            break;

        default:
            qDebug() << "Unhandled movable" << type;
        }
    }

    m_cameraMatrix.rotate( 90, 0, 1, 0);
    m_cameraMatrix.rotate(-90, 1, 0, 0);
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
    m_cameraMatrix.rotate(angleY, m_cameraMatrix.row(1).toVector3D());
    m_position += m_cameraMatrix.row(2).toVector3D() * (m_backwards - m_forwards) * 5.0f;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, float(rect().width()) / rect().height(), 0.1f, 10000.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixd(m_cameraMatrix.data());
    glTranslatef(-m_position.x(), -m_position.y(), -m_position.z());

    static float lightPos[] = {100, 100, 200, 0 };
    static float lightColor[] = { 1, 1, 1, 1 };
   // glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_AMBIENT_AND_DIFFUSE, lightColor);

    m_surface->draw();

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
}


QVector3D Scenario::getPosition() const
{
    QVector3D pos;
    pos.setX(m_file.value("px").toInt() * 16);
    pos.setY(m_file.value("py").toInt() * 16);
    pos.setZ(m_file.value("pz").toInt() * 16);
    if (pos.z() == 0)
        pos.setZ(m_surface->heightAt(pos.x(), pos.y()) + 10);
    return pos;
}


} // namespace fight
