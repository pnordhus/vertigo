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
#include <QFile>


namespace txt {


QString StringTable::m_language;
QStringList StringTable::m_tableDesktop;
QStringList StringTable::m_tableFight;


bool StringTable::load()
{
    if (QFile::exists("txt:german.lan"))
        m_language = "German";
    if (QFile::exists("txt:english.lan"))
        m_language = "English";

    DesFile fileDes("txt:deeptext.des");
    fileDes.setSection("Text");

    if (!fileDes.contains("Text190"))
        return false;

    for (int i = 0; ; i++) {
        const QString key = QString("Text%1").arg(i);
        if (!fileDes.contains(key))
            break;

        m_tableDesktop.append(fileDes.value(key).toString());
    }

    fileDes.load("txt:fightext.des");
    fileDes.setSection("Text");

    for (int i = 0; ; i++) {
        const QString key = QString("Text%1").arg(i);
        if (!fileDes.contains(key))
            break;

        m_tableFight.append(fileDes.value(key).toString());
    }

    return true;
}


QString StringTable::get(String string)
{
    return m_tableDesktop.at(string);
}

QString StringTable::get(StringFight string)
{
    return m_tableFight.at(string);
}


} // namespace txt
