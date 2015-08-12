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

#ifndef GFX_FONT_H
#define GFX_FONT_H


#include "util/rect.hpp"
#include "cliprect.h"
#include "texture.h"
#include <QExplicitlySharedDataPointer>


namespace gfx {


class FontPrivate : public QSharedData
{
public:
    FontPrivate();

public:
    void load(const QString &filename, const QVector<QRgb> &colorTable, bool scale, bool smooth);
    util::Rect draw(const QString &text, const util::Rect &dstRect, bool alignHCenter = false, bool alignBottom = false, const ClipRect *clipRect = nullptr);
    int height() const { return m_height; }
    int height(int w, const QStringList &strings) const;
    int width(const QString &text) const;

private:
    struct Symbol
    {
        util::RectF rect;
        int width;
        int height;
    };

    Texture m_texture;
    QVector<Symbol> m_symbols;
    int m_height;
    bool m_alpha;
    QRgb m_colorNormal;
    QRgb m_colorHighlight;
};


class Font
{
public:
    enum Name
    {
        Small,
        Medium,
        Large,
        DialogTop,
        DialogBottom,
        DialogHighlight,
    };

public:
    Font();
    Font(Name name);
    Font(const QString &filename, const QVector<QRgb> &colorTable, bool scale = false, bool smooth = true);
    Font(const QString &filename, const QRgb &colorNormal, const QRgb &colorHighlight, bool scale = false, bool smooth = true);

public:
    void load(const QString &filename, const QVector<QRgb> &colorTable, bool scale = false, bool smooth = true);
    void load(const QString &filename, const QRgb &colorNormal, const QRgb &colorHighlight, bool scale = false, bool smooth = true);
    util::Rect draw(const QString &text, int x, int y, const ClipRect *clipRect = nullptr);
    util::Rect draw(const QString &text, const util::Point &dstPos, const ClipRect *clipRect = nullptr);
    util::Rect draw(const QString &text, const util::Rect &dstRect, const ClipRect *clipRect = nullptr);
    util::Rect draw(const QString &text, const util::Rect &dstRect, bool alignHCenter, bool alignBottom, const ClipRect *clipRect = nullptr);
    int height() const;
    int width(const QString &text) const;

private:
    QExplicitlySharedDataPointer<FontPrivate> d;
};


inline Font::Font() :
    d(new FontPrivate)
{

}


inline Font::Font(const QString &filename, const QVector<QRgb> &colorTable, bool scale, bool smooth) :
    d(new FontPrivate)
{
    load(filename, colorTable, scale, smooth);
}


inline Font::Font(const QString &filename, const QRgb &colorNormal, const QRgb &colorHighlight, bool scale, bool smooth) :
    d(new FontPrivate)
{
    load(filename, colorNormal, colorHighlight, scale, smooth);
}


inline void Font::load(const QString &filename, const QVector<QRgb> &colorTable, bool scale, bool smooth)
{
    d->load(filename, colorTable, scale, smooth);
}


inline void Font::load(const QString &filename, const QRgb &colorNormal, const QRgb &colorHighlight, bool scale, bool smooth)
{
    d->load(filename, QVector<QRgb>() << colorNormal << colorHighlight, scale, smooth);
}


inline util::Rect Font::draw(const QString &text, int x, int y, const ClipRect *clipRect)
{
    return d->draw(text, util::Rect(x, y, -1, -1), false, false, clipRect);
}


inline util::Rect Font::draw(const QString &text, const util::Point &dstPos, const ClipRect *clipRect)
{
    return draw(text, dstPos.x, dstPos.y, clipRect);
}


inline util::Rect Font::draw(const QString &text, const util::Rect &dstRect, const ClipRect *clipRect)
{
    return d->draw(text, dstRect, false, false, clipRect);
}


inline util::Rect Font::draw(const QString &text, const util::Rect &dstRect, bool alignHCenter, bool alignBottom, const ClipRect *clipRect)
{
    return d->draw(text, dstRect, alignHCenter, alignBottom, clipRect);
}


inline int Font::height() const
{
    return d->height();
}


inline int Font::width(const QString &text) const
{
    return d->width(text);
}


} // namespace gfx


#endif // GFX_FONT_H
