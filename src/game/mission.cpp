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

#include "mission.h"
#include "txt/desfile.h"


namespace game {


Mission::Mission(const QString &name, int station) :
    m_shortName(name),
    m_station(station)
{
    txt::DesFile file(QString("dat:mission/%1.des").arg(name));
    file.setSection("areamap");
    m_name = file.value("name").toString();
    m_pos = QPoint(file.value("X").toInt(), file.value("Y").toInt());

    file.setSection("mission");
    m_scenario = file.value("dramaturgy").toString();

    file.setSection("B");
    m_textB = file.value("{text}").toStringList();
    file.setSection("P");
    m_textP = file.value("{text}").toStringList();
    file.setSection("S");
    m_textS = file.value("{text}").toStringList();
}


} // namespace game
