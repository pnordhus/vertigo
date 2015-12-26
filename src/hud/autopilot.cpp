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

#include "autopilot.h"
#include "hud.h"
#include "fight/scenario.h"


namespace hud {


Autopilot::Autopilot(HUD *hud, Rect rect) :
    ui::Widget(hud->widget()),
    m_hud(hud),
    m_rect(rect),
    m_autopilot(hud->getImage("hudautop"), true)
{
}


void Autopilot::draw()
{
    Rect rect = m_hud->projectCenter(m_rect);
    if (m_hud->scenario()->conditionManager().autopilot() && m_hud->scenario()->blink())
        m_autopilot.draw(rect.x, rect.y + 1);
}


} // namespace hud
