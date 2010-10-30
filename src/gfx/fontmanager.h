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

#ifndef GFX_FONTMANAGER_H
#define GFX_FONTMANAGER_H


#include "font.h"
#include <QMap>


namespace gfx {


class FontManager
{
public:
    FontManager();
    ~FontManager();

public:
    Font font(Font::Name name) const;

public:
    static FontManager* get() { Q_ASSERT(m_singleton); return m_singleton; }

private:
    Q_DISABLE_COPY(FontManager);
    QMap<Font::Name, Font> m_fonts;

private:
    static FontManager *m_singleton;
};


} // namespace gfx


#endif // GFX_FONTMANAGER_H
