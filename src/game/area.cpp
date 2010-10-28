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

#include "area.h"
#include "desktop.h"
#include "txt/desfile.h"


namespace game {


Area::Area()
{

}


Area::Area(const QString &name)
{
    load(name);
}


void Area::load(const QString &name)
{
    txt::DesFile file(QString("dat:world/%1.des").arg(name));
    file.setSection("Area");
    m_name = file.value("Name").toString();
    m_code = file.value("Code").toInt();

    file.setSection("Map");
    m_map = file.value("Name").toString();

    foreach (const QString &section, file.sections()) {
        QRegExp reg("^station(\\d*)$");
        if (reg.indexIn(section) >= 0) {
            file.setSection(section);
            const int index = reg.cap(1).toUInt();
            const QString station = file.value("Name").toString();
            m_stations.insert(index, Station(index, station));
        }
    }
}


} // namespace game
