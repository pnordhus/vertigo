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
#include "surface.h"
#include "txt/desfile.h"
#include <QGLContext>
#include <QMouseEvent>


namespace fight {


Scenario::Scenario(const QString &name) :
    m_angleX(0),
    m_angleY(0)
{
    hideCursor();
    txt::DesFile file(QString("vfx:scenario/%1.des").arg(name));

    file.setSection("surface");
    m_surface = new Surface(file.value("name").toString().toLower(), file.value("patchcomb").toInt());
}


Scenario::~Scenario()
{
    delete m_surface;
}


void Scenario::draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, float(rect().width()) / rect().height(), 1.0f, 1000000.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glRotatef(m_angleX, 1, 0, 0);
    glRotatef(m_angleY, 0, 1, 0);
    glRotatef(-90, 1, 0, 0);
    glTranslatef(-131 * 512,-112 * 512,-5500);

    static float lightPos[] = {100, 100, 200, 0 };
    static float lightColor[] = { 1, 1, 1, 1 };
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_AMBIENT_AND_DIFFUSE, lightColor);

    m_surface->draw();
}


void Scenario::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Escape)
        emit success();
}


void Scenario::mouseMoveEvent(QMouseEvent *event)
{
    QPoint pos = event->pos();
    if (pos == rect().center())
        return;

    pos -= rect().center();

    m_angleY += pos.x() / 20.0f;
    m_angleX += pos.y() / 20.0f;

    if (m_angleX > 89.9)
        m_angleX = 89.9;
    if (m_angleX < -89.9)
        m_angleX = -89.9;

    emit centerMouse();
}


} // namespace fight
