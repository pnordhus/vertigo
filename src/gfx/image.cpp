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
#include "rle.h"
#include <QBitmap>
#include <QCursor>
#include <QFile>
#include <QPainter>
#include <QPixmap>


namespace gfx {


QImage Image::load(const QString &filename, const QVector<QRgb> &colorTable)
{
    QFile file(filename);
    file.open(QFile::ReadOnly);

    QDataStream stream(&file);
    stream.setByteOrder(QDataStream::LittleEndian);

    quint16 type;
    stream >> type;

    return load(&file, Type(type), colorTable);
}


QImage Image::load(const QString &filename, int w, int h)
{
    QFile file(filename);
    file.open(QFile::ReadOnly);

    QDataStream stream(&file);
    stream.setByteOrder(QDataStream::LittleEndian);

    QImage image(w, h, QImage::Format_RGB888);

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            quint16 value;
            stream >> value;

            const quint8 r = (value >> 8) & 0xf8;
            const quint8 g = (value >> 3) & 0xfc;
            const quint8 b = (value << 3) & 0xf8;
            image.setPixel(x, y, qRgb(r, g, b));
        }
    }

    return image;
}


QImage Image::loadPCX(const QString &filename)
{
    QFile file(filename);
    file.open(QFile::ReadOnly);

    struct
    {
//        quint8 manufacturer;
//        quint8 version;
        quint8 encoding;
        quint8 bitsPerPixel;
        qint16 xMin, yMin, xMax, yMax;
//        qint16 hDpi, vDpi;
//        quint8 colormap[48];
//        quint8 reserved;
        quint8 nPlanes;
        qint16 bytesPerLine;
//        qint16 paletteInfo;
//        qint16 hScreenSize;
//        qint16 vScreenSize;
//        quint8 filler[54];
    } header;

    QDataStream stream(&file);
    stream.setByteOrder(QDataStream::LittleEndian);

    stream.skipRawData(2);
    stream >> header.encoding >> header.bitsPerPixel;
    stream >> header.xMin >> header.yMin >> header.xMax >> header.yMax;
    stream.skipRawData(53);
    stream >> header.nPlanes >> header.bytesPerLine;
    stream.skipRawData(60);

    const int width = (header.xMax - header.xMin) + 1;
    const int height = (header.yMax - header.yMin) + 1;

    Q_ASSERT(header.bitsPerPixel == 8);
    Q_ASSERT(header.nPlanes == 1);
    Q_ASSERT(header.encoding == 1);

    QImage image(width, height, QImage::Format_Indexed8);

    for (int y = 0; y < height; y++) {
        quint8 *scanLine = image.scanLine(y);

        int x = 0;
        while (x < header.bytesPerLine) {
            quint8 c;
            stream >> c;
            if ((c & 0xc0) == 0xc0) {
                const int count = c & 0x3f;
                stream >> c;
                for (int i = 0; i < count; i++)
                    scanLine[x++] = c;
            } else {
                scanLine[x++] = c;
            }
        }
    }

    quint8 c;
    do {
        stream >> c;
    } while (c != 12);

    gfx::ColorTable colorTable;
    colorTable.load(file.read(256 * 3));
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


QImage Image::load(QIODevice *device, Type type, const QVector<QRgb> &colorTable)
{
    QDataStream stream(device);
    stream.setByteOrder(QDataStream::LittleEndian);

    quint32 width;
    quint32 height;
    qint32 x;
    qint32 y;
    quint32 length;

    stream >> width >> height >> x >> y;

    QImage image;
    switch (type) {
    case PaletteRLE:
        stream >> length;
        image = RLE::decodeImage(device->read(length), width, height, colorTable);
        break;

    case Palette:
        image = QImage((const quint8*) (device->read(width * height)).constData(), width, height, width, QImage::Format_Indexed8).copy();
        image.setColorTable(colorTable);
        break;

    case RGB565:
        stream >> length;
        image = RLE::decodeImage(device->read(length), width, height);
        break;
    }

    return image;
}


} // namespace gfx
