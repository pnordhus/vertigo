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


#include <map>
#include <list>
#include "condition.h"


namespace fight {


class Scenario;


class ConditionManager
{
public:
    ConditionManager(Scenario *scenario);

public:
    struct ConditionEntry
    {
        Condition *condTrigger;
        int cond1;
        int dep1;
        int ref1;
        int op;
        int cond2;
        int dep2;
        int ref2;
        int del;
        ConditionEvent *condSignal;
        ConditionEvent *condAttacked;
        ConditionEvent *condIdentified;
        ConditionEvent *condParalyzed;
        ConditionEvent *condFinished;
        ConditionEvent *condBoarded;
    };

public:
    ConditionEntry& addEntry(int id);
    void updateObjective(int id, int index, int wccond);
    void buildDependencies();

    Condition* addCondition(int limit = 0);
    ConditionRadio* addCondRadio(const glm::vec3 &pos, const QString &text);
    ConditionSpace* addCondSpace(int x, int y, int dimx, int dimy, int minz, int maxz);
    void testSpace(float x, float y, float height);
    void delayComplete(Condition *cond, int delay);
    void update(float elapsedTime);

private:
    Scenario *m_scenario;
    std::map<int, ConditionEntry> condEntries;
    std::list<Condition> m_conditions;
    std::list<ConditionRadio> m_condRadio;
    std::list<ConditionSpace> m_condSpaces;

    ConditionAutopilot m_condAutopilot;
    ConditionFailure m_condFailure;
    std::map<int, Condition> m_condObjectives;

    struct DelayCompleteEntry
    {
        Condition *cond;
        float completeTime;
    };

    std::list<DelayCompleteEntry> m_delayEntries;

private:
    void initCondition(int cond, int dep, int ref, Condition *condDepend);
};


}


#endif // CONDITIONMANAGER_H
