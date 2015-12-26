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

#ifndef HUD_CROSSHAIR_H
#define HUD_CROSSHAIR_H


#include "util/geometry2d.h"
#include "ui/widget.h"
#include "gfx/texture.h"
#include "gfx/cliprect.h"


namespace hud {


class HUD;


class Crosshair : public ui::Widget
{
public:
    Crosshair(HUD *hud, glm::ivec2 center, const gfx::ClipRect *clipRect);

protected:
    void draw();

private:
    HUD *m_hud;
    glm::ivec2 m_center;
    const gfx::ClipRect *m_clipRect;
    gfx::Texture m_targ;
    gfx::Texture m_aim;
};


} // namespace hud


#endif // HUD_CROSSHAIR_H
