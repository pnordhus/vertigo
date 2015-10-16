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
#include "fight/scenario.h"


namespace fight {


TurretBase::TurretBase(Scenario *scenario, txt::DesFile &file, int iff, const QString &name, const QString &cargo) :
    SimpleObject(scenario, file, iff, name, cargo)
{
    m_static = false;
    m_noise++;

    file.setSection("gunturret");
    if (file.contains("name"))
    {
        txt::DesFile gunDes(QString("vfx:sobjects/%1.des").arg(file.value("name").toString()));
        m_body.reset(new Turret(scenario, gunDes)); // TODO: TurretGun
        m_body->setPosition(Vector3D(file.value("RelativePositionX").toFloat(), file.value("RelativePositionY").toFloat(), file.value("RelativePositionZ").toFloat()));
        m_box.add(m_body->box().scale(m_scale));
    }
    file.setSection("torpedoturret");
    if (file.contains("name"))
    {
        txt::DesFile torpedoDes(QString("vfx:sobjects/%1.des").arg(file.value("name").toString()));
        m_body.reset(new Turret(scenario, torpedoDes)); // TODO: TurretTorpedo
        m_body->setPosition(Vector3D(file.value("RelativePositionX").toFloat(), file.value("RelativePositionY").toFloat(), file.value("RelativePositionZ").toFloat()));
        m_box.add(m_body->box().scale(m_scale));
    }
}


void TurretBase::draw()
{
    glPushMatrix();
    glTranslatef(m_position.x, m_position.y, m_position.z);
    glScalef(m_scale, m_scale, m_scale);
    m_base->draw();
    m_body->draw();
    glPopMatrix();
}


bool TurretBase::intersect(const Vector3D &start, const Vector3D &dir, float radius, float &distance, Vector3D &normal)
{
    bool collision = m_base->intersect((start - m_position)/m_scale, dir, radius/m_scale, distance, normal);
    if (collision)
        distance *= m_scale;

    float d;
    Vector3D norm;
    if (m_body->intersect((start - m_position)/m_scale, dir, radius/m_scale, d, norm))
    {
        d *= m_scale;
        if (!collision || distance > d)
        {
            distance = d;
            normal = norm;
            collision = true;
        }
    }

    return collision;
}


} // namespace fight
