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

#include "heading.h"
#include "hud.h"
#include "fight/scenario.h"
#include "fight/objects/navpoint.h"
#include <glm/trigonometric.hpp>


namespace hud {


Heading::Heading(HUD *hud, util::Rect rect) :
    ui::Widget(hud->widget()),
    m_hud(hud),
    m_rect(rect),
    m_head(hud->getImage("hudhead"), false),
    m_point(hud->getImage("hudpoi4"), false),
    m_way(hud->getImage("hudway1"), false)
{
}


void Heading::draw()
{
    int offset = 360 + static_cast<int>(glm::degrees(m_hud->scenario()->yaw())*2);
    util::Rect rect = m_hud->projectCenter(m_rect);
    m_head.draw(rect.pos(), util::RectF(offset, 0, m_rect.width, m_head.height()));
    m_point.draw(rect.x + rect.width/2 - (m_point.width() + 1)/2, rect.y + m_head.height() + 1);

    if (m_hud->navPoint() >= 0 && (m_hud->scenario()->navPoints()[m_hud->navPoint()] != m_hud->scenario()->target().lockedNavPoint() || m_hud->scenario()->blink()))
    {
        glm::vec3 dir = m_hud->scenario()->position() - m_hud->scenario()->navPoints()[m_hud->navPoint()]->position();
        int navOffset = 360 + static_cast<int>(glm::degrees(glm::atan(dir.x, dir.y))*2) - offset;
        if (navOffset < -360)
            navOffset += 720;
        if (navOffset > 360)
            navOffset -= 720;
        int halfRect = rect.width/2;
        int halfWay = (m_way.width() + 1)/2;
        int left = 0;
        if (navOffset < -halfRect + halfWay)
        {
            left = halfWay - halfRect - navOffset;
            if (left > m_way.width()/2)
                left = m_way.width()/2;
            navOffset = -halfRect + halfWay;
        }
        int right = 0;
        if (navOffset > rect.width - m_way.width() - halfRect + halfWay)
        {
            right = navOffset - (rect.width - m_way.width() - halfRect + halfWay);
            if (right > m_way.width()/2)
                right = m_way.width()/2;
            navOffset = rect.width - m_way.width() - halfRect + halfWay;
        }
        m_way.draw(rect.x + halfRect - halfWay + navOffset + right, rect.y + 7, util::RectF(left, 0, m_way.width() - left - right, m_way.height()));
    }
}

} // namespace hud
