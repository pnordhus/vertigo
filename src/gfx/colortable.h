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

#ifndef GFX_COLORTABLE_H
#define GFX_COLORTABLE_H

#include "util/file.h"

#include <QImage>

namespace gfx {

class ColorTable : public QVector<QRgb>
{
public:
    ColorTable();
    ColorTable(const QVector<QRgb> &colorTable);
    ColorTable(const QString &filename);

public:
    bool loadFromFile(const QString &filename);
    void load(const QByteArray &data);
    void load(util::File &file);
    void toRgb565();
};

} // namespace gfx

#endif // GFX_COLORTABLE_H
