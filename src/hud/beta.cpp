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

#include "beta.h"
#include "hud.h"
#include "fight/scenario.h"
#include <glm/trigonometric.hpp>


namespace hud {


Beta::Beta(HUD *hud, util::Rect rect) :
    ui::Widget(hud->widget()),
    m_hud(hud),
    m_rect(rect),
    m_beta(hud->getImage("hudbeta"), false),
    m_point(hud->getImage("hudpoi3"), false)
{
}


void Beta::draw()
{
    int offset = 360 + static_cast<int>(glm::degrees(m_hud->scenario()->pitch())*2);
    util::Rect rect = m_hud->projectCenter(m_rect);
    m_beta.draw(rect.x + m_point.width() + 1, rect.y, util::RectF(0, offset, m_beta.width(), m_rect.height));
    m_point.draw(rect.x, rect.y + rect.height/2 - (m_point.height() + 1)/2);
}


} // namespace hud
