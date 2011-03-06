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

#include "conditionmanager.h"


namespace fight {


QList<ConditionManager::DelayCompleteEntry> ConditionManager::m_entries;

void ConditionManager::delayComplete(Condition *cond, int delay)
{
    m_entries.append(DelayCompleteEntry());
    m_entries.last().cond = cond;
    m_entries.last().completeTime = QTime::currentTime().addSecs(delay);
}


void ConditionManager::update()
{
    QTime time = QTime::currentTime();
    for (int i = 0; i < m_entries.count(); i++)
        if (m_entries[i].completeTime <= time)
        {
            m_entries[i].cond->complete();
            m_entries.removeAt(i);
            i--;
        }
}


} // namespace fight
