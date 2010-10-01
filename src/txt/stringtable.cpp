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

#include "desfile.h"
#include "stringtable.h"
#include <QDebug>


namespace txt {


QStringList StringTable::m_table;


void StringTable::load()
{
    DesFile fileDes("txt:deeptext.des");
    fileDes.beginGroup("Text");

    for (int i = 0; ; i++) {
        const QString key = QString("Text%1").arg(i);
        if (!fileDes.contains(key))
            break;

        m_table.append(fileDes.value(key).toString());
    }
}


QString StringTable::get(String string)
{
    return m_table.at(string);
}


} // namespace txt
