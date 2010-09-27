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


#include <QImage>


namespace gfx {


class Image
{
public:
    enum Type { PaletteRLE = 0x0400, Palette = 0x0401, RGB565 = 0x0413 };

public:
    static QImage load(const QString &filename, const QVector<QRgb> &colorTable = QVector<QRgb>());
    static QImage load(QIODevice *device, Type type, const QVector<QRgb> &colorTable);
    static QImage loadPCX(const QString &filename);
    static QCursor loadCursor(const QString &filename, const QVector<QRgb> &colorTable);
};


} // namespace gfx


#endif // GFX_IMAGE_H
