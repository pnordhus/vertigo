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

#include "texture.h"
#include <QGLWidget>


namespace gfx {


inline static quint32 nextPowerOfTwo(quint32 v)
{
    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    ++v;
    return v;
}


inline static GLenum formatToGL(Texture::Format format)
{
    switch (format) {
    case Texture::RGBA:
        return GL_RGBA;

    default:
        return GL_RGB;
    }
}


TexturePrivate::TexturePrivate() :
    m_texture(0),
    m_width(0),
    m_height(0),
    m_textureWidth(0),
    m_textureHeight(0)
{

}


TexturePrivate::~TexturePrivate()
{
    clear();
}


void TexturePrivate::clear()
{
    if (m_texture > 0) {
        glDeleteTextures(1, &m_texture);
        m_texture = 0;
        m_width = 0;
        m_height = 0;
        m_textureWidth = 0;
        m_textureHeight = 0;
    }
}


void TexturePrivate::setFilter()
{
    bind();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
}


void TexturePrivate::fromImage(const QImage &image)
{
    Texture::Format format = Texture::RGB;
    if (image.format() != QImage::Format_RGB888)
        format = Texture::RGBA;

    createEmpty(image.width(), image.height(), format);
    update(0, 0, image);
}


void TexturePrivate::createEmpty(int w, int h, Texture::Format format)
{
    clear();

    glGenTextures(1, &m_texture);
    bind();
    setFilter();

    m_format = format;
    m_width = w;
    m_height = h;
    m_textureWidth = nextPowerOfTwo(m_width);
    m_textureHeight = nextPowerOfTwo(m_height);

    QByteArray fill(4 * m_textureWidth * m_textureHeight, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, formatToGL(format), m_textureWidth, m_textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, fill.data());
}


void TexturePrivate::update(int x, int y, QImage image)
{
    GLenum format;

    switch (image.format()) {
    case QImage::Format_RGB888:
        format = GL_RGB;
        break;

    default:
        image = QGLWidget::convertToGLFormat(image.mirrored());
        format = GL_RGBA;
    }

    bind();
    glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, image.width(), image.height(), format, GL_UNSIGNED_BYTE, image.bits());
}


void TexturePrivate::bind()
{
    glBindTexture(GL_TEXTURE_2D, m_texture);
}


QRectF TexturePrivate::draw(const QRectF &destRect, const QRectF &srcRect)
{
    QRectF srcRectTex;
    srcRectTex.setLeft(srcRect.left() / m_textureWidth);
    srcRectTex.setRight(srcRect.right() / m_textureWidth);
    srcRectTex.setTop(srcRect.top() / m_textureHeight);
    srcRectTex.setBottom(srcRect.bottom() / m_textureHeight);

    glEnable(GL_TEXTURE_2D);
    bind();
    glBegin(GL_QUADS);
        glTexCoord2f(srcRectTex.left(), srcRectTex.top());
        glVertex2f(destRect.left(), destRect.top());

        glTexCoord2f(srcRectTex.left(), srcRectTex.bottom());
        glVertex2f(destRect.left(),  destRect.bottom());

        glTexCoord2f(srcRectTex.right(), srcRectTex.bottom());
        glVertex2f(destRect.right(),  destRect.bottom());

        glTexCoord2f(srcRectTex.right(), srcRectTex.top());
        glVertex2f(destRect.right(), destRect.top());
    glEnd();

    return destRect;
}


} // namespace gfx
