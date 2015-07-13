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

#include "meter.h"
#include "hud.h"
#include "fight/scenario.h"
#include <glm/common.hpp>


namespace hud {


Meter::Meter(HUD *hud, util::Rect rect, glm::ivec2 barPos, int barHeight, bool k) :
    ui::Widget(hud->widget()),
    m_hud(hud),
    m_rect(rect),
    m_barPos(barPos),
    m_barHeight(barHeight),
    m_k(k),
    m_meter(hud->getImage(k ? "hudkmet" : "hudmet"), k ? true : false),
    m_point(hud->getImage("hudpoi1"), k ? true : false),
    m_pointRed(hud->getImage("hudpoi2"), false)
{
}


void Meter::draw()
{
    util::Rect rect = m_hud->projectCenter(m_rect);
    m_meter.draw(rect.x, rect.y);
    float depth = m_hud->scenario()->depth();
    if (m_k)
        m_point.draw(rect.x + m_barPos.x, rect.y + m_barPos.y + static_cast<int>(depth/1000*m_barHeight/12) - (m_point.height() + 1)/2);
    else
    {
        depth = glm::mod<float>(depth, 100);
        m_point.draw(rect.x + m_barPos.x, rect.y + m_barPos.y + static_cast<int>(depth*m_barHeight/100) - (m_point.height() + 1)/2);
        float height = m_hud->scenario()->height();
        if (height < 50)
        {
            height = glm::mod<float>(depth + height, 100);
            m_pointRed.draw(rect.x + m_barPos.x, rect.y + m_barPos.y + static_cast<int>(height*m_barHeight/100) - (m_pointRed.height() + 1)/2);
        }
    }
}

} // namespace hud
