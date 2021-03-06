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

#ifndef GFX_IMAGE_H
#define GFX_IMAGE_H

#include "util/file.h"

#include <QImage>

namespace gfx {

class Image : public QImage
{
public:
    enum Type { Bitmap = 0x0000, PaletteRLE = 0x0400, Palette = 0x0401, RGB565 = 0x0413 };

public:
    Image();
    Image(const QImage &image);

public:
    Image& toRgb565();

public:
    static Image load(const QString &filename, const QVector<QRgb> &colorTable = QVector<QRgb>());
    static Image load(const QString &filename, int w, int h, bool colorKey = false);
    static Image load(util::File& file, Type type, const QVector<QRgb> &colorTable);
    static Image loadPCX(const QString &filename);
    static QCursor loadCursor(const QString &filename, const QVector<QRgb> &colorTable);

private:
    static QImage decodeImage(util::File& file, int width, int height, const QRgb& color);
    static QImage decodeImage(util::File& file, int width, int height, const QVector<QRgb> &colorTable);
    static QImage decodeImage(util::File& file, int width, int height);
};

} // namespace gfx

#endif // GFX_IMAGE_H
