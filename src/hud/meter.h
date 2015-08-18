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

#ifndef HUD_METER_H
#define HUD_METER_H


#include "util/geometry2d.h"
#include "ui/widget.h"
#include "gfx/texture.h"


namespace hud {


class HUD;


class Meter : public ui::Widget
{
public:
    Meter(HUD *hud, Rect rect, glm::ivec2 barPos, int barHeight, bool k);

protected:
    void draw();

private:
    HUD *m_hud;
    Rect m_rect;
    glm::ivec2 m_barPos;
    int m_barHeight;
    bool m_k;
    gfx::Texture m_meter;
    gfx::Texture m_point;
    gfx::Texture m_pointRed;
};


} // namespace hud


#endif // HUD_METER_H
