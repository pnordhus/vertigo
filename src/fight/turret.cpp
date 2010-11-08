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

#include "turret.h"
#include "txt/desfile.h"
#include <QGLContext>


namespace fight {


Turret::Turret(ModuleManager &modMan, const QString &name)
{
    txt::DesFile file(QString("vfx:sobjects/%1.des").arg(name));
    file.setSection("cluster");
    m_body = modMan.get(file.value("body").toString());

    m_armLeft = modMan.get(file.value("arml").toString());
    m_armLeftPosition.setX(file.value("ArmLOffsetX").toFloat());
    m_armLeftPosition.setY(file.value("ArmLOffsetY").toFloat());
    m_armLeftPosition.setZ(file.value("ArmLOffsetZ").toFloat());

    m_armRight = modMan.get(file.value("armr").toString());
    m_armRightPosition.setX(file.value("ArmROffsetX").toFloat());
    m_armRightPosition.setY(file.value("ArmROffsetY").toFloat());
    m_armRightPosition.setZ(file.value("ArmROffsetZ").toFloat());

    file.setSection("size");
    m_scale = file.value("scale").toFloat() / 16;
}


void Turret::draw()
{
    glPushMatrix();
    m_body.draw();

    glPushMatrix();
    glTranslatef(m_armLeftPosition.x(), m_armLeftPosition.y(), m_armLeftPosition.z());
    m_armLeft.draw();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(m_armRightPosition.x(), m_armRightPosition.y(), m_armRightPosition.z());
    m_armRight.draw();
    glPopMatrix();

    glPopMatrix();
}


} // namespace fight
