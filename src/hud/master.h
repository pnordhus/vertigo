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

#ifndef HUD_MASTER_H
#define HUD_MASTER_H


#include "util/rect.hpp"
#include "ui/widget.h"
#include "ui/label.h"
#include "gfx/cliprect.h"


namespace hud {


class HUD;


class Master : public ui::Widget
{
public:
    Master(HUD *hud, util::Rect rect);

public:
    const gfx::ClipRect& clipRect() const { return m_clipRect; }

protected:
    void draw();

private:
    HUD *m_hud;
    util::Rect m_rect;
    gfx::ClipRect m_clipRect;

    gfx::Texture m_edgeBL;
    gfx::Texture m_edgeBR;
    gfx::Texture m_edgeTL;
    gfx::Texture m_edgeTR;

    gfx::Texture m_activeBlue;
    gfx::Texture m_activeGreen;
    gfx::Texture m_activeRed;
    gfx::Texture m_passiveBlue;
    gfx::Texture m_passiveGreen;
    gfx::Texture m_passiveRed;

    gfx::Texture m_way;
};


} // namespace hud


#endif // HUD_MASTER_H
