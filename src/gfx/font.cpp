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

#include "fontmanager.h"
#include "image.h"
#include <QGLContext>

namespace gfx {

Font::Font(Name name)
{
    *this = FontManager::get()->font(name);
}

FontPrivate::FontPrivate() :
    m_height(0)
{

}

void FontPrivate::load(const QString &filename, const QVector<QRgb> &colorTable, bool scale)
{
    util::File file(QString(filename).replace(":", "/").toUtf8().data());

    int scaleFactor = scale ? 2 : 1;
    const float res = 256 * scaleFactor;

    quint8 type;
    quint32 numEntries;

    file >> type;
    file.skipBytes(5);
    file >> numEntries;
    file.skipBytes(8);

    if (colorTable.size() == 2) {
        m_texture.createEmpty(res, res, Texture::Alpha);
        m_colorNormal = colorTable[0];
        m_colorHighlight = colorTable[1];
        m_alpha = true;
    } else {
        m_texture.createEmpty(res, res, Texture::RGBA);
        m_alpha = false;
    }

    int maxHeight = 0;
    int x = 1;
    int y = 1;
    for (unsigned int i = 0; i < numEntries; i++) {
        QImage image;
        switch (type) {
        case 2:
            image = Image::load(file, Image::PaletteRLE, colorTable);
            break;

        case 4:
        case 18:
            image = Image::load(file, Image::Bitmap, colorTable);
            break;

        case 20:
            image = Image::load(file, Image::RGB565, colorTable);
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

QRect FontPrivate::draw(const QString &text, int x, int y, int w, int h, bool alignHCenter, bool alignBottom)
{
    m_texture.bind();

    glPushMatrix();

    if (alignHCenter) {
        const int totalW = width(text);
        if (w > totalW)
            x += (w - totalW) / 2;
    }

    QStringList strings = text.split(" ");
    if (alignBottom) {
        const int totalH = height(w, strings);
        if (h > totalH)
            y += (h - totalH);
    }

    glTranslatef(x, y, 0);

    bool highlight = false;
    int totalHeight = m_height;
    int totalWidth = 0.0f;
    int lineWidth = 0.0f;
    int current = 0;
    foreach (QString word, strings) {
        if (++current < strings.size())
            word += " ";
        const int wordWidth = width(word);
        if (w > 0 && lineWidth + wordWidth > w) {
            glTranslatef(-lineWidth, m_height + 1, 0);
            totalWidth = qMax(totalWidth, lineWidth);
            totalHeight += m_height + 1;
            lineWidth = 0;
        }

        const QByteArray latin1 = word.toLatin1();
        for (int i = 0; i < latin1.length(); i++) {
            quint8 c = latin1[i];
            if (c == '~') {
                i++;
                c = word[i].toLatin1();
                switch (c) {
                case '0':
                    highlight = false;
                    break;

                case '1':
                    highlight = true;
                    break;
                }
                continue;
            }

            c -= 32;

            Q_ASSERT(c < m_symbols.size());
            const Symbol &symbol = m_symbols[c];
            const QRectF &rect = symbol.rect;

            if (m_alpha) {
                if (highlight)
                    glColor4f(qRed(m_colorHighlight) / 255.0f, qGreen(m_colorHighlight) / 255.0f, qBlue(m_colorHighlight) / 255.0f, qAlpha(m_colorHighlight) / 255.0f);
                else
                    glColor4f(qRed(m_colorNormal) / 255.0f, qGreen(m_colorNormal) / 255.0f, qBlue(m_colorNormal) / 255.0f, qAlpha(m_colorNormal) / 255.0f);
            }

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
            lineWidth += symbol.width;
        }
    }
    totalWidth = qMax(totalWidth, lineWidth);

    glPopMatrix();
    glColor3f(1, 1, 1);

    QRect rect;
    rect.setLeft(x);
    rect.setTop(y);
    rect.setHeight(totalHeight);
    rect.setWidth(totalWidth);
    return rect;
}

int FontPrivate::height(int w, const QStringList &strings) const
{
    int totalHeight = m_height;
    int lineWidth = 0.0f;
    foreach (QString word, strings) {
        word += " ";
        const int wordWidth = width(word);
        if (w > 0 && lineWidth + wordWidth > w) {
            totalHeight += m_height + 1;
            lineWidth = 0;
        }

        lineWidth += wordWidth;
    }

    return totalHeight;
}

int FontPrivate::width(const QString &text) const
{
    int totalW = 0;
    const QByteArray latin1 = text.toLatin1();
    for (int i = 0; i < latin1.size(); i++) {
        quint8 c = latin1[i];
        if (c == '~') {
            i++;
            continue;
        }
        c -= 32;
        const Symbol &symbol = m_symbols[c];
        totalW += symbol.width;
    }

    return totalW;
}

} // namespace gfx
