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

#include "font.h"
#include "image.h"
#include "rle.h"
#include <QFile>
#include <QGLContext>


namespace gfx {



FontPrivate::FontPrivate() :
    m_height(0)
{

}


void FontPrivate::load(const QString &filename, const QVector<QRgb> &colorTable, bool scale)
{
    QFile file(filename);
    if (!file.open(QFile::ReadOnly))
        return;

    int scaleFactor = scale ? 2 : 1;
    const float res = 256 * scaleFactor;

    QDataStream stream(&file);
    stream.setByteOrder(QDataStream::LittleEndian);

    quint8 type;
    quint32 numEntries;

    stream >> type;
    stream.skipRawData(5);
    stream >> numEntries;
    stream.skipRawData(8);

    m_texture.createEmpty(res, res, Texture::RGBA);

    int maxHeight = 0;
    int x = 1;
    int y = 1;
    for (unsigned int i = 0; i < numEntries; i++) {
        QImage image;
        switch (type) {
        case 2:
            image = Image::load(&file, Image::PaletteRLE, colorTable);
            break;

        case 4:
        case 18:
            image = Image::load(&file, Image::Bitmap, colorTable);
            break;

        case 20:
            image = Image::load(&file, Image::RGB565, colorTable);
            break;

        default:
            qFatal("Unknown font type");
        }

        image = image.scaled(image.width() * scaleFactor, image.height() * scaleFactor);

        if (x + image.width() + 1 >= res) {
            x = 1;
            y += maxHeight + 1;
        }

        m_texture.update(x, y, image);

        Symbol symbol;
        symbol.rect.setLeft(x / res);
        symbol.rect.setRight((x + image.width()) / res);
        symbol.rect.setTop(y / res);
        symbol.rect.setBottom((y + image.height()) / res);
        symbol.width = image.width() / scaleFactor;
        symbol.height = image.height() / scaleFactor;
        m_symbols.append(symbol);

        maxHeight = qMax(maxHeight, image.height());

        x += image.width() + 1;
    }

    m_height = maxHeight / scaleFactor;
}


QRect FontPrivate::draw(const QString &text, int x, int y, int w, int h, bool alignHCenter, bool alignVCenter)
{
    m_texture.bind();

    glPushMatrix();

    if (alignHCenter) {
        const int totalW = width(text);
        if (w > totalW)
            x += (w - totalW) / 2;
    }

    glTranslatef(x, y, 0);

    int width = 0.0f;
    for (int i = 0; i < text.size(); i++) {
        const quint8 c = text[i].toLatin1() - 32;

        Q_ASSERT(c < m_symbols.size());
        const Symbol &symbol = m_symbols[c];
        const QRectF &rect = symbol.rect;

        glBegin(GL_QUADS);
            glTexCoord2f(rect.left(), rect.top());
            glVertex2f(0, 0);
            glTexCoord2f(rect.left(), rect.bottom());
            glVertex2f(0, symbol.height);
            glTexCoord2f(rect.right(), rect.bottom());
            glVertex2f(symbol.width, symbol.height);
            glTexCoord2f(rect.right(), rect.top());
            glVertex2f(symbol.width, 0);
        glEnd();

        glTranslatef(symbol.width, 0, 0);
        width += symbol.width;
    }

    glPopMatrix();

    QRect rect;
    rect.setLeft(x);
    rect.setTop(y);
    rect.setHeight(m_height);
    rect.setWidth(width);
    return rect;
}


int FontPrivate::width(const QString &text) const
{
    int totalW = 0;
    for (int i = 0; i < text.size(); i++) {
        const quint8 c = text[i].toLatin1() - 32;
        const Symbol &symbol = m_symbols[c];
        totalW += symbol.width;
    }

    return totalW;
}


} // namespace gfx
