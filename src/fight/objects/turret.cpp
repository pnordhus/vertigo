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
#include "fight/scenario.h"


namespace fight {


Turret::Turret(Scenario *scenario, txt::DesFile &file) :
    Object(scenario, file),
    m_armLeft(nullptr),
    m_armRight(nullptr)
{
    file.setSection("cluster");
    m_body = scenario->moduleManager().get(file.value("body").toString());

    if (file.contains("arml") && file.value("arml") != "no")
    {
        m_armLeft = scenario->moduleManager().get(file.value("arml").toString());
        m_armLeftPosition.x = file.value("ArmLOffsetX").toFloat();
        m_armLeftPosition.y = file.value("ArmLOffsetY").toFloat();
        m_armLeftPosition.z = file.value("ArmLOffsetZ").toFloat();
    }

    if (file.contains("armr") && file.value("armr") != "no")
    {
        m_armRight = scenario->moduleManager().get(file.value("armr").toString());
        m_armRightPosition.x = file.value("ArmROffsetX").toFloat();
        m_armRightPosition.y = file.value("ArmROffsetY").toFloat();
        m_armRightPosition.z = file.value("ArmROffsetZ").toFloat();
    }
}


void Turret::draw()
{
    glTranslatef(m_position.x, m_position.y, m_position.z);
    //glScalef(m_scale, m_scale, m_scale);
    m_body->draw();

    if (m_armLeft != nullptr)
    {
        glPushMatrix();
        glTranslatef(m_armLeftPosition.x, m_armLeftPosition.y, m_armLeftPosition.z);
        m_armLeft->draw();
        glPopMatrix();
    }

    if (m_armRight != nullptr)
    {
        glPushMatrix();
        glTranslatef(m_armRightPosition.x, m_armRightPosition.y, m_armRightPosition.z);
        m_armRight->draw();
        glPopMatrix();
    }
}


} // namespace fight
