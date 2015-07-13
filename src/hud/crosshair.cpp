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


namespace hud {


Crosshair::Crosshair(HUD *hud, glm::ivec2 center) :
    ui::Widget(hud->widget()),
    m_hud(hud),
    m_center(center),
    m_targ(hud->getImage("hudtarg"), false)
{
}


void Crosshair::draw()
{
    glm::ivec2 p = m_hud->project(m_center);
    m_targ.draw(p.x - (m_targ.width() + 1)/2, p.y - (m_targ.height() + 1)/2);
}


} // namespace hud
