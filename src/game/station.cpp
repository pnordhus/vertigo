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

#include "station.h"
#include "txt/desfile.h"


namespace game {


Station::Station() :
    m_index(-1)
{

}


Station::Station(int index, const QString &name) :
    m_index(index),
    m_shortName(name)
{
    txt::DesFile file("dat:world/" + name + ".des");

    file.beginGroup("Station");
    m_name = file.value("Name").toString();
    file.endGroup();
}


} // namespace game
