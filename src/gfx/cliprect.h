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

#ifndef GFX_CLIPRECT_H
#define GFX_CLIPRECT_H


#include "util/rect.hpp"
#include "texture.h"


namespace gfx {


class ClipRect
{
public:
    ClipRect();

public:
    void setRect(const util::RectF &rect) { m_rect = rect; }
    void setRect(const util::Rect &rect);
    const util::RectF& rect() const { return m_rect; }

public:
    void draw(Texture& tex, float x, float y) const;
    void draw(Texture& tex, const glm::vec2 &dstPos) const;
    void draw(Texture& tex, const util::RectF &dstRect) const;
    void draw(Texture& tex, float x, float y, const util::RectF &srcRect) const;
    void draw(Texture& tex, const glm::vec2 &dstPos, const util::RectF &srcRect) const;
    void draw(Texture& tex, const util::RectF &dstRect, const util::RectF &srcRect) const;

private:
    util::RectF m_rect;
};


inline void ClipRect::draw(Texture& tex, float x, float y) const
{
    draw(tex, util::RectF(x, y, tex.width(), tex.height()));
}


inline void ClipRect::draw(Texture& tex, const glm::vec2 &dstPos) const
{
    draw(tex, util::RectF(dstPos, glm::vec2(tex.width(), tex.height())));
}


inline void ClipRect::draw(Texture& tex, const util::RectF &dstRect) const
{
    draw(tex, dstRect, util::RectF(0, 0, tex.width(), tex.height()));
}


inline void ClipRect::draw(Texture& tex, float x, float y, const util::RectF &srcRect) const
{
    draw(tex, util::RectF(x, y, tex.width(), tex.height()), srcRect);
}


inline void ClipRect::draw(Texture& tex, const glm::vec2 &dstPos, const util::RectF &srcRect) const
{
    draw(tex, util::RectF(dstPos, glm::vec2(tex.width(), tex.height())), srcRect);
}


} // namespace gfx


#endif // GFX_CLIPRECT_H
