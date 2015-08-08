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

#include "activeobject.h"
#include "fight/scenario.h"


namespace fight {


ActiveObject::ActiveObject(Scenario *scenario) :
    Object(scenario)
{
}


ActiveObject::ActiveObject(Scenario *scenario, txt::DesFile &file, int iff, const QString &name, const QString &cargo) :
    Object(scenario, file),
    m_iff(iff),
    m_name(name),
    m_cargo(cargo)
{
    file.setSection("noise");
    m_noise = file.value("level").toFloat();

    file.setSection("defense");
}


void ActiveObject::destroy()
{
    disable();
    m_eventDestroy.complete();
}


} // namespace fight
