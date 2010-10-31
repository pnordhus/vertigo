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

#include "task.h"
#include <QRegExp>


namespace game {


Task::Task(const QString &text)
{
    QRegExp reg("(\\w): (.*)");
    reg.indexIn(text);
    if (reg.cap(1) == "T")
        m_type = Tip;
    else if (reg.cap(1) == "A")
        m_type = Job;
    else if (reg.cap(1) == "M")
        m_type = Mission;
    m_text = reg.cap(2);
}


} // namespace game
