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


#include <QImage>
#include <QExplicitlySharedDataPointer>


namespace gfx {



class TexturePrivate;


class Texture
{
public:
    enum Format { RGB, RGBA };

public:
    Texture();

public:
    int width() const;
    int height() const;
    void bind();
    void fromImage(const QImage &image);
    void createEmpty(int w, int h, Texture::Format format);
    void update(int x, int y, QImage image);
    void draw();
    void draw(const QRectF &rect);

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
    int width() const { return m_width; }
    int height() const { return m_height; }
    void bind();
    void setFilter();
    void fromImage(const QImage &image);
    void createEmpty(int w, int h, Texture::Format format);
    void update(int x, int y, QImage image);
    void draw(const QRectF &rect, const QRectF &srcRect);

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


inline int Texture::width() const
{
    return d->width();
}


inline int Texture::height() const
{
    return d->height();
}


inline void Texture::bind()
{
    d->bind();
}


inline void Texture::fromImage(const QImage &image)
{
    d->fromImage(image);
}


inline void Texture::createEmpty(int w, int h, Texture::Format format)
{
    d->createEmpty(w, h, format);
}


inline void Texture::update(int x, int y, QImage image)
{
    d->update(x, y, image);
}


inline void Texture::draw()
{
    d->draw(QRectF(0, 0, width(), height()), QRectF(0, 0, width(), height()));
}


inline void Texture::draw(const QRectF &destRect)
{
    d->draw(destRect, QRectF(0, 0, width(), height()));
}


} // namespace gfx


#endif // GFX_TEXTURE_H
