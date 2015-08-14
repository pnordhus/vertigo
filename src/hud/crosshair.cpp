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

#include "crosshair.h"
#include "hud.h"
#include "fight/scenario.h"
#include "fight/objects/activeobject.h"

namespace hud {


Crosshair::Crosshair(HUD *hud, glm::ivec2 center, const gfx::ClipRect *clipRect) :
    ui::Widget(hud->widget()),
    m_hud(hud),
    m_center(center),
    m_clipRect(clipRect),
    m_targ(hud->getImage("hudtarg"), false),
    m_aim(hud->getImage("hudaimh"), false)
{
}


void Crosshair::draw()
{
    glm::ivec2 p = m_hud->project(m_center);
    m_targ.draw(p.x - (m_targ.width() + 1)/2, p.y - (m_targ.height() + 1)/2);

    fight::Target &target = m_hud->scenario()->target();
    if (target.locked() != nullptr)
    {
        glm::vec3 aimPoint = target.locked()->center();

        glm::mat4 m = m_hud->hudProjectionMatrixInverted() * m_hud->scenario()->projectionMatrix() * m_hud->scenario()->cameraMatrix();
        glm::vec4 point4 = m * glm::vec4(aimPoint - m_hud->scenario()->position(), 1);
        util::Point point = glm::round(glm::vec2(point4)/point4.w);
        util::Rect rect = m_clipRect->rect();

        if (point4.z > 0 || !rect.contains(point))
        {
            util::Point center = m_hud->project(m_hud->center());
            glm::vec2 dir = glm::vec2(point4) - glm::vec2(center)*point4.w;
            glm::vec2 dim;
            if (dir.x > 0)
                dim.x = rect.right() - center.x;
            else
                dim.x = rect.left() - center.x;
            if (dir.y > 0)
                dim.y = rect.bottom() - center.y;
            else
                dim.y = rect.top() - center.y;
            point = center + util::Point(dim.x, dim.x/dir.x*dir.y);
            if (!rect.contains(point))
                point = center + util::Point(dim.y/dir.y*dir.x, dim.y);
        }
        m_aim.draw(point.x - m_aim.width()/2, point.y - m_aim.height()/2, m_clipRect);
    }
}


} // namespace hud
