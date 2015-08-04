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

#ifndef HUD_ACTIVESONAR_H
#define HUD_ACTIVESONAR_H


#include "util/rect.hpp"
#include "ui/widget.h"
#include "gfx/texture.h"


namespace hud {


class HUD;


class ActiveSonar : public ui::Widget
{
public:
    ActiveSonar(HUD *hud, util::Rect rect);

protected:
    void draw();

private:
    HUD *m_hud;
    util::Rect m_rect;
    gfx::Texture m_actBo;
    gfx::Texture m_actOu;
};


} // namespace hud


#endif // HUD_ACTIVESONAR_H
