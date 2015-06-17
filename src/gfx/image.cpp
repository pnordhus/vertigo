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
#include "image.h"
#include <QBitmap>
#include <QCursor>
#include <QPainter>
#include <QPixmap>

namespace gfx {

Image::Image()
{

}

Image::Image(const QImage &image) :
    QImage(image)
{

}

Image& Image::toRgb565()
{
    ColorTable c = colorTable();
    c.toRgb565();
    setColorTable(c);
    return *this;
}

Image Image::load(const QString &filename, const QVector<QRgb> &colorTable)
{
    util::File file(QString(filename).replace(":", "/").toUtf8().data());
    std::uint16_t type;
    file >> type;
    return load(file, Type(type), colorTable);
}

Image Image::load(const QString &filename, int w, int h, bool colorKey)
{
    util::File file(QString(filename).replace(":", "/").toUtf8().data());

    QImage image(w, h, colorKey ? QImage::Format_ARGB32 : QImage::Format_RGB888);

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            std::uint16_t value;
            file >> value;

            const std::uint8_t r = (value >> 8) & 0xf8;
            const std::uint8_t g = (value >> 3) & 0xfc;
            const std::uint8_t b = (value << 3) & 0xf8;
            if (colorKey && r == 0 && g == 0 && b == 0)
                image.setPixel(x, y, qRgba(0,0,0,0));
            else
                image.setPixel(x, y, qRgb(r, g, b));
        }
    }

    return image;
}

Image Image::loadPCX(const QString &filename)
{
    util::File file(QString(filename).replace(":", "/").toUtf8().data());

    struct
    {
//        std::uint8_t manufacturer;
//        std::uint8_t version;
        std::uint8_t encoding;
        std::uint8_t bitsPerPixel;
        std::int16_t xMin, yMin, xMax, yMax;
//        std::int16_t hDpi, vDpi;
//        std::uint8_t colormap[48];
//        std::uint8_t reserved;
        std::uint8_t nPlanes;
        std::int16_t bytesPerLine;
//        std::int16_t paletteInfo;
//        std::int16_t hScreenSize;
//        std::int16_t vScreenSize;
//        std::uint8_t filler[54];
    } header;

    file.skipBytes(2);
    file >> header.encoding >> header.bitsPerPixel;
    file >> header.xMin >> header.yMin >> header.xMax >> header.yMax;
    file.skipBytes(53);
    file >> header.nPlanes >> header.bytesPerLine;
    file.skipBytes(60);

    const int width = (header.xMax - header.xMin) + 1;
    const int height = (header.yMax - header.yMin) + 1;

    ASSERT(header.bitsPerPixel == 8);
    ASSERT(header.nPlanes == 1);
    ASSERT(header.encoding == 1);

    QImage image(width, height, QImage::Format_Indexed8);

    for (int y = 0; y < height; y++) {
        std::uint8_t *scanLine = image.scanLine(y);

        int x = 0;
        while (x < header.bytesPerLine) {
            std::uint8_t c;
            file >> c;
            if ((c & 0xc0) == 0xc0) {
                const int count = c & 0x3f;
                file >> c;
                for (int i = 0; i < count; i++)
                    scanLine[x++] = c;
            } else {
                scanLine[x++] = c;
            }
        }
    }

    std::uint8_t c;
    do {
        file >> c;
    } while (c != 12);

    gfx::ColorTable colorTable;
    colorTable.load(file);
    colorTable[0] = qRgba(0,0,0,0);

    image.setColorTable(colorTable);

    return image;
}

QCursor Image::loadCursor(const QString &filename, const QVector<QRgb> &colorTable)
{
    const QImage image = load(filename, colorTable);

    QPixmap pixmap = QPixmap::fromImage(image);
    {
        QBitmap mask(image.width(), image.height());
        mask.clear();
        QPainter painter(&mask);
        painter.setPen(Qt::color1);
        for (int y = 0; y < image.height(); y++) {
            for (int x = 0; x < image.width(); x++) {
                if (image.pixelIndex(x, y) != 0)
                    painter.drawPoint(x, y);
            }
        }
        painter.end();
        pixmap.setMask(mask);
    }

    return QCursor(pixmap, 3, 4);
}

Image Image::load(util::File &file, Type type, const QVector<QRgb> &colorTable)
{
    std::uint32_t width;
    std::uint32_t height;
    std::int32_t x;
    std::int32_t y;

    file >> width >> height >> x >> y;

    QImage image;
    switch (type) {
    case Bitmap:
        image = decodeImage(file, width, height, colorTable[0]);
        break;

    case PaletteRLE:
        image = decodeImage(file, width, height, colorTable);
        break;

    case Palette:
        image = QImage((const quint8*) file.read(width * height).data(), width, height, width, QImage::Format_Indexed8).copy();
        image.setColorTable(colorTable);
        break;

    case RGB565:
        image = decodeImage(file, width, height);
        break;
    }

    return image;
}

QImage Image::decodeImage(util::File &file, int width, int height, const QRgb &color)
{
    std::uint32_t length;
    file >> length;
    const std::size_t offset = file.position();

    QImage image(width, height, QImage::Format_ARGB32);

    for (int y = 0; y < height; y++) {
        int x = 0;
        while (x < width && !file.atEnd()) {
            std::uint16_t transparent = 0;
            std::uint16_t opaque = 0;
            file >> transparent >> opaque;

            for (uint i = 0; i < transparent; i++)
                image.setPixel(x++, y, qRgba(0, 0, 0, 0));

            for (uint i = 0; i < opaque; i++)
                image.setPixel(x++, y, color);
        }
    }
    file.seek(offset + length);
    return image;
}

QImage Image::decodeImage(util::File &file, int width, int height, const QVector<QRgb> &colorTable)
{
    std::uint32_t length;
    file >> length;
    const std::size_t offset = file.position();

    QImage image(width, height, QImage::Format_ARGB32);

    for (int y = 0; y < height; y++) {
        int x = 0;
        while (x < width && !file.atEnd()) {
            std::uint16_t transparent = 0;
            std::uint16_t opaque = 0;
            file >> transparent >> opaque;

            for (uint i = 0; i < transparent; i++)
                image.setPixel(x++, y, qRgba(0, 0, 0, 0));

            for (uint i = 0; i < opaque; i++) {
                std::uint8_t index;
                file >> index;
                image.setPixel(x++, y, colorTable[index]);
            }
        }
    }
    file.seek(offset + length);
    return image;
}

QImage Image::decodeImage(util::File &file, int width, int height)
{
    std::uint32_t length;
    file >> length;
    const std::size_t offset = file.position();

    QImage image(width, height, QImage::Format_ARGB32);

    for (int y = 0; y < height; y++) {
        int x = 0;
        while (x < width && !file.atEnd()) {
            std::uint16_t transparent = 0;
            std::uint16_t opaque = 0;
            file >> transparent >> opaque;

            for (uint i = 0; i < transparent; i++)
                image.setPixel(x++, y, qRgba(0, 0, 0, 0));

            for (uint i = 0; i < opaque; i++) {
                std::uint16_t value;
                file >> value;

                const std::uint8_t r = (value >> 8) & 0xf8;
                const std::uint8_t g = (value >> 3) & 0xfc;
                const std::uint8_t b = (value << 3) & 0xf8;

                image.setPixel(x++, y, qRgb(r, g, b));
            }
        }
    }
    file.seek(offset + length);
    return image;
}

} // namespace gfx
