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
#include <QFile>


namespace util {


ColorTable::ColorTable()
{

}


ColorTable::ColorTable(const QString &filename)
{
    loadFromFile(filename);
}


bool ColorTable::loadFromFile(const QString &filename)
{
    QFile file(filename);
    if (!file.open(QFile::ReadOnly))
        return false;
    file.seek(4);
    load(file.readAll());
    return true;
}


void ColorTable::load(const QByteArray &data)
{
    Q_ASSERT(data.size() == 256 * 3);

    clear();
    for (int i = 0; i < data.size(); i += 3)
        append(qRgb(data[i + 0], data[i + 1], data[i + 2]));
}


} // namespace util
