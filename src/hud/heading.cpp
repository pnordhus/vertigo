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
#include <glm/trigonometric.hpp>


namespace hud {


Heading::Heading(HUD *hud, util::Rect rect) :
    ui::Widget(hud->widget()),
    m_hud(hud),
    m_rect(rect),
    m_head(hud->getImage("hudhead"), false),
    m_point(hud->getImage("hudpoi4"), false)
{
}


void Heading::draw()
{
    int offset = 360 + static_cast<int>(glm::degrees(m_hud->scenario()->yaw())*2);
    //if (offset < 0)
    //    offset += 720;
    m_head.draw(m_rect.x, m_rect.y, QRectF(offset, 0, m_rect.width, m_head.height()));
    m_point.draw(m_rect.x + m_rect.width/2 - (m_point.width() + 1)/2, m_rect.y + m_head.height() + 1);
}

} // namespace hud
