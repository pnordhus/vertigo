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

#include "colortable.h"
#include "fontmanager.h"


namespace gfx {


FontManager *FontManager::m_singleton = NULL;


FontManager::FontManager()
{
    Q_ASSERT(m_singleton == NULL);
    m_singleton = this;

    const gfx::ColorTable colorTable("gfx:pal/gui/border.pal");

    m_fonts.insert(Font::Small, Font("gfx:fnt/dpsmall.fnt", colorTable));
    m_fonts.insert(Font::Medium, Font("gfx:fnt/dpmedium.fnt", colorTable));
    m_fonts.insert(Font::Large, Font("gfx:fnt/dplarge.fnt", colorTable));

    m_fonts.insert(Font::DialogTop, Font("gfx:fnt/dpsmamon.fnt", 0xffb89c00, 0xffc0c400, true));
    m_fonts.insert(Font::DialogBottom, Font("gfx:fnt/dpsmamon.fnt", 0xff00a8d0, 0xff00a8d0, true));
    m_fonts.insert(Font::DialogHighlight, Font("gfx:fnt/dpsmamon.fnt", 0xff00e4f8, 0xff00e4f8, true));
}


FontManager::~FontManager()
{
    m_singleton = NULL;
}


Font FontManager::font(Font::Name name) const
{
    Q_ASSERT(m_fonts.contains(name));
    return m_fonts.value(name);
}


} // namespace gfx
