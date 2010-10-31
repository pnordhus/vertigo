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

#include "boat.h"
#include "txt/desfile.h"


namespace game {


Boat::Boat(int id)
{
    if (id == 0)
    {
        txt::DesFile file("vfx:sobjects/hiob.des");
        file.setSection("Ship");
        m_name = file.value("Name").toString();
        file.setSection("Cockpit");
        m_cockpit = file.value("Name").toString();
        m_flipMovie1 = "hiob1.mvi";
        m_flipMovie2 = "hiob2.mvi";
    }
}


} // namespace game
