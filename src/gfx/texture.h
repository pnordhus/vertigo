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

#ifndef GFX_TEXTURE_H
#define GFX_TEXTURE_H


#include "cliprect.h"
#include <QImage>
#include <QExplicitlySharedDataPointer>


namespace gfx {



class TexturePrivate;


class Texture
{
public:
    enum Format { RGB, RGBA, Alpha };

public:
    Texture();
    Texture(const QImage &image, bool smooth = true);

public:
    bool isValid() const;
    int width() const;
    int height() const;
    QSize size() const;
    void bind();
    void fromImage(const QImage &image, bool smooth = true);
    void createEmpty(int w, int h, Texture::Format format, bool smooth = true);
    void createEmpty(const QSize &size, Texture::Format format, bool smooth = true);
    void update(int x, int y, QImage image);
    RectF draw(const ClipRect *clipRect = nullptr);
    RectF draw(float x, float y, const ClipRect *clipRect = nullptr);
    RectF draw(const PointF &dstPos, const ClipRect *clipRect = nullptr);
    RectF draw(const RectF &dstRect, const ClipRect *clipRect = nullptr);
    RectF draw(float x, float y, const RectF &srcRect, const ClipRect *clipRect = nullptr);
    RectF draw(const PointF &dstPos, const RectF &srcRect, const ClipRect *clipRect = nullptr);
    RectF draw(const RectF &dstRect, const RectF &srcRect, const ClipRect *clipRect = nullptr);

private:
    QExplicitlySharedDataPointer<TexturePrivate> d;
};


class TexturePrivate : public QSharedData
{
public:
    TexturePrivate();
    TexturePrivate(const TexturePrivate&);
    ~TexturePrivate();

public:
    bool isValid() const { return m_texture != 0; }
    int width() const { return m_width; }
    int height() const { return m_height; }
    void bind();
    void setFilter(bool smooth);
    void fromImage(const QImage &image, bool smooth);
    void createEmpty(int w, int h, Texture::Format format, bool smooth);
    void update(int x, int y, QImage image);
    RectF draw(const RectF &dstRect, const RectF &srcRect, const ClipRect *clipRect);

private:
    void clear();

private:
    unsigned int m_texture;
    Texture::Format m_format;
    quint32 m_width;
    quint32 m_height;
    quint32 m_textureWidth;
    quint32 m_textureHeight;
};


inline Texture::Texture() :
    d(new TexturePrivate)
{

}


inline Texture::Texture(const QImage &image, bool smooth) :
    d(new TexturePrivate)
{
    d->fromImage(image, smooth);
}


inline bool Texture::isValid() const
{
    return d->isValid();
}


inline int Texture::width() const
{
    return d->width();
}


inline int Texture::height() const
{
    return d->height();
}


inline QSize Texture::size() const
{
    return QSize(width(), height());
}


inline void Texture::bind()
{
    d->bind();
}


inline void Texture::fromImage(const QImage &image, bool smooth)
{
    d->fromImage(image, smooth);
}


inline void Texture::createEmpty(int w, int h, Texture::Format format, bool smooth)
{
    d->createEmpty(w, h, format, smooth);
}


inline void Texture::createEmpty(const QSize &size, Texture::Format format, bool smooth)
{
    d->createEmpty(size.width(), size.height(), format, smooth);
}


inline void Texture::update(int x, int y, QImage image)
{
    d->update(x, y, image);
}


inline RectF Texture::draw(const ClipRect *clipRect)
{
    return draw(RectF(0, 0, width(), height()), clipRect);
}


inline RectF Texture::draw(float x, float y, const ClipRect *clipRect)
{
    return draw(RectF(x, y, width(), height()), clipRect);
}


inline RectF Texture::draw(const PointF &dstPos, const ClipRect *clipRect)
{
    return draw(RectF(dstPos.x, dstPos.y, width(), height()), clipRect);
}


inline RectF Texture::draw(const RectF &dstRect, const ClipRect *clipRect)
{
    return draw(dstRect, RectF(0, 0, width(), height()), clipRect);
}


inline RectF Texture::draw(float x, float y, const RectF &srcRect, const ClipRect *clipRect)
{
    return draw(RectF(x, y, srcRect.width, srcRect.height), srcRect, clipRect);
}


inline RectF Texture::draw(const PointF &dstPos, const RectF &srcRect, const ClipRect *clipRect)
{
    return draw(RectF(dstPos.x, dstPos.y, srcRect.width, srcRect.height), srcRect, clipRect);
}


inline RectF Texture::draw(const RectF &dstRect, const RectF &srcRect, const ClipRect *clipRect)
{
    return d->draw(dstRect, srcRect, clipRect);
}


} // namespace gfx


#endif // GFX_TEXTURE_H
