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

#ifndef CONDITIONMANAGER_H
#define CONDITIONMANAGER_H


#include "object.h"


namespace fight {


class ConditionManager
{
public:
    static void delayComplete(Condition *cond, int delay);
    static void update();

private:
    struct DelayCompleteEntry
    {
        Condition *cond;
        QTime completeTime;
    };

    static QList<DelayCompleteEntry> m_entries;
};


}


#endif // CONDITIONMANAGER_H
