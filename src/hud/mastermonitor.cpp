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

#include "mastermonitor.h"
#include "hud.h"
#include "fight/scenario.h"
#include "fight/objects/activeobject.h"
#include "fight/objects/navpoint.h"
#include "txt/stringtable.h"
#include <glm/geometric.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>


namespace hud {


MasterMonitor::MasterMonitor(HUD *hud, Rect rect, Rect observationRect) :
    ui::Widget(hud->widget()),
    m_hud(hud),
    m_rect(rect),
    m_observationRect(observationRect)
{
}


void MasterMonitor::draw()
{
    Point pos;
    bool isFriend = true;
    QString txtName = txt::StringTable::get(txt::Target_NoTargetLocked);
    QString txtAction = "";
    QString txtCargo = "-------";
    QString txtDistance = "---";
    QString txtHeight = "---";
    QString txtVelocity = "---";
    QString txtNoise = "---";
    QString txtRange = "----";
    QString txtGun = "----";
    QString txtTorpedo = "----";

    fight::Target &target = m_hud->scenario()->target();
    fight::ActiveObject *object = target.locked();
    if (object != nullptr)
    {
        isFriend = m_hud->scenario()->isFriend(0, object->iff());
        txtName = object->name();
        if (m_hud->scenario()->sonar().detectActivity())
            txtAction = "activity not implemented";
        if (object->isIdentified())
            txtCargo = object->cargo();
        else
            txtCargo = txt::StringTable::get(txt::Target_NotIdentified);

        int distance = static_cast<int>(glm::round(target.distance()));
        txtDistance = QString("%1M").arg(distance);
        int height = static_cast<int>(glm::round(object->center().z - m_hud->scenario()->surface().heightAt(object->center().x, object->center().y)));
        txtHeight = QString("%1M").arg(height);

        txtVelocity = QString("%1KMH").arg(static_cast<int>(glm::round(glm::length(object->velocity()))));
        txtNoise = QString("%1").arg(object->noise());

        if (m_hud->scenario()->sonar().detectRange() && !target.isPassive() && object->sensor() > 0)
            txtRange = QString("%1M").arg(object->sensorRange());
        if (m_hud->scenario()->sonar().detectWeapons() && !target.isPassive())
        {
            txtGun = "NIMP";
            txtTorpedo = "NIMP";
        }
    }

    fight::NavPoint *navPoint = target.lockedNavPoint();
    if (navPoint != nullptr)
    {
        txtName = QString("NAV %1").arg(static_cast<char>('A' + navPoint->num()));
        txtDistance = QString("%1M").arg(static_cast<int>(glm::round(target.distance())));
        txtHeight = "10M";
        txtVelocity = "0KMH";
        txtNoise = "0";
    }

    if (target.isLocked())
    {
        Point observePos = m_hud->project(m_rect.pos() + m_observationRect.pos());
        Point observeCenter = m_hud->project(m_rect.pos() + m_observationRect.center());
        Matrix m(1);
        m = glm::translate(m, Vector3D(observeCenter, -0.5f));
        m = glm::scale(m, Vector3D(observeCenter.x - observePos.x, observePos.y - observeCenter.y, 0.5f));
        m *= m_hud->scenario()->cameraMatrix();
        m = glm::translate(m, m_hud->scenario()->position());

        fight::Object *obj = target.object();
        const fight::BoundingBox& box = obj->box();
        m = glm::scale(m, Vector3D(2/glm::length(box.dim())));
        m = glm::translate(m, -0.5f*(box.maxPoint() + box.minPoint()));

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glFrontFace(GL_CW);
        glMatrixMode(GL_MODELVIEW);
        glLoadMatrixf(glm::value_ptr(m));
        obj->draw();
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glDisable(GL_CULL_FACE);
        glFrontFace(GL_CCW);
        glDisable(GL_DEPTH_TEST);
    }

    pos = m_hud->project(m_rect.pos() + Point(m_rect.width/2, 4));
    (isFriend ? m_hud->fontGreen() : m_hud->fontRed()).draw(txtName, Rect(pos.x - 100, pos.y, 200, -1), true, false);

    pos.y += 12;
    m_hud->fontGreen().draw(txtAction, Rect(pos.x - 100, pos.y, 200, -1), true, false);

    pos = m_hud->project(m_rect.pos() + Point(m_rect.width/2, m_rect.height - 12));
    m_hud->fontGreen().draw(QString("%1%2").arg(txt::StringTable::get(txt::Target_Cargo)).arg(txtCargo), Rect(pos.x - 100, pos.y, 200, -1), true, false);

    pos = m_hud->project(m_rect.pos() + Point(0, 32));
    m_hud->fontGreen().draw(QString("D:%1").arg(txtDistance), pos);
    
    pos.y += 12;
    m_hud->fontGreen().draw(QString("H:%1").arg(txtHeight), pos);

    pos.y += 12;
    m_hud->fontGreen().draw(QString("V:%1").arg(txtVelocity), pos);

    pos.y += 12;
    m_hud->fontGreen().draw(QString("N:%1").arg(txtNoise), pos);

    pos = m_hud->project(m_rect.pos() + Point(m_rect.width, 32));
    pos.x -= 36;
    m_hud->fontGreen().draw(QString("R:%1").arg(txtRange), pos);

    pos.y += 12;
    m_hud->fontGreen().draw(QString("G:%1").arg(txtGun), pos);

    pos.y += 12;
    m_hud->fontGreen().draw(QString("T:%1").arg(txtTorpedo), pos);
}


} // namespace hud
