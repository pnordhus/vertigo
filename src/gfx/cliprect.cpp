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

#include "cliprect.h"
#include <QRectF>


namespace gfx {


ClipRect::ClipRect()
{
}


bool ClipRect::clip(util::RectF &dstRect, util::RectF &srcRect) const
{
    if (dstRect.right() <= m_rect.left() || dstRect.left() >= m_rect.right())
        return false;
    if (dstRect.bottom() <= m_rect.top() || dstRect.top() >= m_rect.bottom())
        return false;
    util::RectF dst = dstRect;
    util::RectF src = srcRect;

    float clipLeft = m_rect.left() - dstRect.left();
    if (clipLeft > 0)
    {
        dst.x += clipLeft;
        dst.width -= clipLeft;
        clipLeft *= srcRect.width/dstRect.width;
        src.x += clipLeft;
        src.width -= clipLeft;
    }

    float clipRight = dstRect.right() - m_rect.right();
    if (clipRight > 0)
    {
        dst.width -= clipRight;
        clipRight *= srcRect.width/dstRect.width;
        src.width -= clipRight;
    }

    float clipTop = m_rect.top() - dstRect.top();
    if (clipTop > 0)
    {
        dst.y += clipTop;
        dst.height -= clipTop;
        clipTop *= srcRect.height/dstRect.height;
        src.y += clipTop;
        src.height -= clipTop;
    }

    float clipBottom = dstRect.bottom() - m_rect.bottom();
    if (clipBottom > 0)
    {
        dst.height -= clipBottom;
        clipBottom *= srcRect.height/dstRect.height;
        src.height -= clipBottom;
    }

    dstRect = dst;
    srcRect = src;
    return true;
}


} // namespace gfx
