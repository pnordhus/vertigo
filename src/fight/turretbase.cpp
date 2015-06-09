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
#include "turretbase.h"


namespace fight {


TurretBase::TurretBase(Scenario *scenario, const QString &name) :
    Object(scenario, name)
{
    txt::DesFile file(QString("vfx:sobjects/%1.des").arg(name));
    file.setSection("gunturret");
    if (!file.contains("name"))
        file.setSection("torpedoturret");
    m_turret = new Turret(scenario->moduleManager(), file.value("name").toString());

    m_turretPosition.setX(file.value("RelativePositionX").toFloat());
    m_turretPosition.setY(file.value("RelativePositionY").toFloat());
    m_turretPosition.setZ(file.value("RelativePositionZ").toFloat());
}


TurretBase::~TurretBase()
{
    delete m_turret;
}


void TurretBase::draw()
{
    glPushMatrix();
    glTranslatef(m_position.x(), m_position.y(), m_position.z());
    glScalef(m_scale, m_scale, m_scale);
    m_base.draw();

    glTranslatef(m_turretPosition.x(), m_turretPosition.y(), m_turretPosition.z());
    m_turret->draw();
    glPopMatrix();
}


} // namespace fight
