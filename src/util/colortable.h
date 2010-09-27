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

#ifndef UTIL_COLORTABLE_H
#define UTIL_COLORTABLE_H


#include <QRgb>
#include <QVector>


namespace util {


class ColorTable : public QVector<QRgb>
{
public:
    ColorTable();
    ColorTable(const QString &filename);

public:
    bool loadFromFile(const QString &filename);
    void load(const QByteArray &data);
};


} // namespace util


#endif // UTIL_COLORTABLE_H
