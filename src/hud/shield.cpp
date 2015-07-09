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

#include "shield.h"
#include "hud.h"


namespace hud {


Shield::Shield(HUD *hud, util::Rect rect) :
    ui::Widget(hud->widget()),
    m_hud(hud),
    m_rect(rect)
{
    for (int i = 1; i <= 9; i++)
        m_shield.emplace_back(m_hud->getImage(QString("hudshie%1").arg(i)), true);
}


void Shield::draw()
{
    util::Rect rect = m_hud->projectCenter(m_rect);
    m_shield[0].draw(rect.x + 3, rect.y + 3);

    // FRONT
    m_shield[2].draw(rect.x + 3, rect.y);

    // BACK
    m_shield[2].draw(rect.x + 3, rect.y + 14);

    // LEFT
    m_shield[1].draw(rect.x, rect.y + 3);

    // RIGHT
    m_shield[1].draw(rect.x + 14, rect.y + 3);
}


} // namespace hud
