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

#include "sonarmonitor.h"
#include "hud.h"
#include "fight/scenario.h"
#include "fight/objects/activeobject.h"
#include "fight/objects/navpoint.h"
#include <glm/geometric.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <QImage>


namespace hud {


void fill(int *data, int color)
{
    data[5] = color;
    data[6] = color;
    data[9] = color;
    data[10] = color;
}

SonarMonitor::SonarMonitor(HUD *hud, Point point, int radius, int scale) :
    ui::Widget(hud->widget()),
    m_hud(hud),
    m_point(point),
    m_radius(radius),
    m_scale(scale)
{
    int data[16] = {0};
    int colors[] = { 0x0084a8, 0x985c20, 0x388040, 0x0000a0, 0x888c88 };
    fill(data, 0xffa88400);
    m_center.fromImage(QImage((uchar*)data, 4, 4, QImage::Format::Format_ARGB32), true);
    fill(data, 0xff205c98);
    m_nav.fromImage(QImage((uchar*)data, 4, 4, QImage::Format::Format_ARGB32), true);
    fill(data, 0xff408038);
    m_friend.fromImage(QImage((uchar*)data, 4, 4, QImage::Format::Format_ARGB32), true);
    fill(data, 0xffa00000);
    m_enemy.fromImage(QImage((uchar*)data, 4, 4, QImage::Format::Format_ARGB32), true);
    fill(data, 0xff888c88);
    m_target.fromImage(QImage((uchar*)data, 4, 4, QImage::Format::Format_ARGB32), true);
}


void SonarMonitor::draw()
{
    Point point = m_hud->project(m_point);
    int radius = (m_hud->project(m_point + Point(0, m_radius)) - point).y;
    float scale = static_cast<float>(radius)/m_scale;
    PointF center = PointF(point) - PointF(1.5f);
    RectF rect = RectF(center, SizeF(4, 4));
    Matrix m(1);
    m = glm::scale(m, Vector3D(1, -1, 0));
    m = glm::rotate(m, m_hud->scenario()->yaw(), Vector3D(0, 0, 1));
    m = glm::translate(m, -m_hud->scenario()->position());

    m_hud->fontGreen().draw("T", point + Point(-2, -radius));
    m_hud->fontGreen().draw(QString("%1M").arg(m_scale), Rect(point + Point(-100, radius - 8), SizeF(200, -1)), true, false);
    m_center.draw(rect);

    for (fight::NavPoint *navPoint : m_hud->scenario()->navPoints())
        if (navPoint->isEnabled())
        {
            Vector2D dir = Vector2D(m * Vector4D(navPoint->position(), 1));
            float distance = glm::length(dir);
            if (distance < m_scale)
            {
                rect.setPos(center - dir*scale);
                m_nav.draw(rect);
            }
        }

    for (const auto &entry : m_hud->scenario()->sonar())
    {
        Vector2D dir = Vector2D(m * Vector4D(entry.object->position(), 1));
        float distance = glm::length(dir);
        if (distance < m_scale)
        {
            rect.setPos(center - dir*scale);
            (entry.isFriend ? m_friend : m_enemy).draw(rect);
        }
    }

    fight::Target &target = m_hud->scenario()->target();
    if (target.isLocked())
    {
        Vector2D dir = Vector2D(m * Vector4D(target.position(), 1));
        float distance = glm::length(dir);
        if (distance < m_scale)
        {
            rect.setPos(center - dir*scale);
            m_target.draw(rect);
        }
    }
}


} // namespace hud
