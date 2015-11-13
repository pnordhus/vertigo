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

#ifndef HUD_RADIOMESSAGE_H
#define HUD_RADIOMESSAGE_H


#include "util/geometry2d.h"
#include "ui/widget.h"
#include "ui/label.h"
#include <QString>


namespace hud {


class HUD;


class RadioMessage : public ui::Widget
{
public:
    RadioMessage(HUD *hud, Rect rect);

protected:
    void draw();

private:
    HUD *m_hud;
    Rect m_rect;

    ui::Label m_lblText;
    gfx::Texture m_black;
    gfx::Texture m_edgeBL;
    gfx::Texture m_edgeBR;
    gfx::Texture m_edgeTL;
    gfx::Texture m_edgeTR;
};


} // namespace hud


#endif // HUD_RADIOMESSAGE_H
