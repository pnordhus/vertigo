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
#include "scenario.h"


namespace fight {


ConditionManager::ConditionManager(Scenario *scenario) :
    m_scenario(scenario),
    m_condAutopilot(scenario),
    m_condFailure(scenario)
{
}


Condition* ConditionManager::addCondition(int limit)
{
    m_conditions.emplace_back(m_scenario, limit);
    return &m_conditions.back();
}


ConditionSpace* ConditionManager::addCondSpace(int x, int y, int dimx, int dimy, int minz, int maxz)
{
    m_condSpaces.emplace_back(m_scenario, x, y, dimx, dimy, minz, maxz);
    return &m_condSpaces.back();
}


void ConditionManager::testSpace(float x, float y, float height)
{
    for (ConditionSpace &space : m_condSpaces)
        space.test(x, y, height);
}


void ConditionManager::delayComplete(Condition *cond, int delay)
{
    m_delayEntries.emplace_back();
    m_delayEntries.back().cond = cond;
    m_delayEntries.back().completeTime = m_scenario->time() + delay*1000;
}


void ConditionManager::update(float elapsedTime)
{
    m_delayEntries.remove_if([this](const DelayCompleteEntry &delayEntry) {
        if (delayEntry.completeTime <= m_scenario->time())
        {
            delayEntry.cond->complete();
            return true;
        }
        return false;
    });
}


ConditionManager::ConditionEntry& ConditionManager::addEntry(int id)
{
    return condEntries[id];
}


void ConditionManager::updateObjective(int id, int index, int wccond)
{
    const ConditionEntry &entry = condEntries.at(id);
    ConditionEvent *condSuccess = NULL;
    ConditionEvent *condFailure = NULL;
    switch (wccond)
    {
    case 1:
        condSuccess = entry.condTrigger;
        break;
    case 2:
        condSuccess = entry.condSignal;
        break;
    case 3:
        condSuccess = entry.condAttacked;
        break;
    case 4:
        condSuccess = entry.condIdentified;
        break;
    case 5:
        condFailure = entry.condSignal;
        break;
    case 6:
        condSuccess = entry.condParalyzed;
        condFailure = entry.condSignal;
        break;
    case 7:
        condSuccess = entry.condFinished;
        condFailure = entry.condSignal;
        break;
    case 8:
        condSuccess = entry.condBoarded;
        condFailure = entry.condSignal;
        break;
    case 9:
        condSuccess = entry.condSignal;
        condFailure = entry.condSignal;
        break;
    default:
        qDebug() << "Unhandled objective condition" << id << wccond;
    }
    if (condSuccess == NULL && condFailure == NULL)
        qDebug() << "Unachievable objective" << id << index;

    Condition *cond;
    if (condSuccess != NULL)
    {
        int objective = index*2;
        if (m_condObjectives.find(objective) == m_condObjectives.end())
        {
            cond = &m_condObjectives.emplace(std::piecewise_construct,
                std::forward_as_tuple(objective),
                std::forward_as_tuple(m_scenario, 1)).first->second;
            if (objective < 16)
            {
                cond->addDependency(&m_condAutopilot);
                m_condAutopilot.setLimit(m_condAutopilot.limit() + 1);
            }
        }
        else
        {
            cond = &m_condObjectives.at(objective);
            if (wccond == 2 || wccond == 9)
                cond->setLimit(cond->limit() + 1);
        }
        condSuccess->addDependency(cond);
    }
    if (condFailure != NULL)
    {
        int objective = index*2 + 1;
        if (m_condObjectives.find(objective) == m_condObjectives.end())
        {
            cond = &m_condObjectives.emplace(std::piecewise_construct,
                std::forward_as_tuple(objective),
                std::forward_as_tuple(m_scenario, 1)).first->second;
            if (objective < 16)
                cond->addDependency(&m_condFailure);
        }
        else
        {
            cond = &m_condObjectives.at(objective);
            if (wccond == 9)
                cond->setLimit(cond->limit() + 1);
        }
        condFailure->addDependency(cond);
        if (wccond == 6 || wccond == 8)
        {
            condSuccess->addDependency(cond, false);
            cond->setLimit(2);
        }
    }
}


void ConditionManager::buildDependencies()
{
    for (const auto &pair : condEntries)
    {
        const ConditionEntry &entry = pair.second;

        if (entry.cond1 == 0 && entry.cond2 != 0)
            qDebug() << "Abnormal condition" << pair.first;
        if (entry.cond1 == 0)
        {
            if (entry.del != 0)
                delayComplete(entry.condTrigger, entry.del);
            continue;
        }

        initCondition(entry.cond1, entry.dep1, entry.ref1, entry.condTrigger);
        if (entry.cond2 != 0)
            initCondition(entry.cond2, entry.dep2, entry.ref2, entry.condTrigger);
        entry.condTrigger->setLimit(entry.cond2 != 0 && entry.op == 0 ? 2 : 1);
        if (entry.op != 0 && entry.op != 1)
            qDebug() << "Unhandled condition operation" << entry.op;
        entry.condTrigger->setDelay(entry.del);
    }
}


void ConditionManager::initCondition(int cond, int dep, int ref, Condition *condDepend)
{
    if (cond == 1 || cond == 39)
    {
        if (dep != 1)
            qDebug() << "Unhandled condition dep" << cond << dep;
        if (condEntries.find(ref) == condEntries.end())
            qDebug() << "Condition reference not found" << ref;
        condEntries[ref].condSignal->addDependency(condDepend, cond == 1);
    }
    if (cond == 2 || cond == 40)
    {
        if (dep != 1)
            qDebug() << "Unhandled condition dep" << cond << dep;
        if (condEntries.find(ref) == condEntries.end())
            qDebug() << "Condition reference not found" << ref;
        condEntries[ref].condAttacked->addDependency(condDepend, cond == 2);
    }
    if (cond == 3 || cond == 41)
    {
        if (dep != 1)
            qDebug() << "Unhandled condition dep" << cond << dep;
        if (condEntries.find(ref) == condEntries.end())
            qDebug() << "Condition reference not found" << ref;
        condEntries[ref].condIdentified->addDependency(condDepend, cond == 3);
    }
    if (cond == 4)
    {
        if (dep != 0)
            qDebug() << "Unhandled condition dep" << cond << dep;
        if (ref != -1)
            qDebug() << "Unexpected reference" << cond << ref;
        m_condAutopilot.addDependency(condDepend);
    }
    if (cond == 5)
    {
        if (dep != 0)
            qDebug() << "Unhandled condition dep" << cond << dep;
        if (ref != -1)
            qDebug() << "Unexpected reference" << cond << ref;
        m_condFailure.addDependency(condDepend);
    }
    if (cond >= 6 && cond <= 37)
    {
        int objective = cond - 6;
        if (m_condObjectives.find(objective) == m_condObjectives.end())
            qDebug() << "Objective not found" << cond;
        if (dep != 0)
            qDebug() << "Unhandled condition dep" << cond << dep;
        if (ref != -1)
            qDebug() << "Unexpected reference" << cond << ref;
        m_condObjectives.at(objective).addDependency(condDepend);
    }
    if (cond == 38)
    {
        if (dep != 1)
            qDebug() << "Unhandled condition dep" << cond << dep;
        if (condEntries.find(ref) == condEntries.end())
            qDebug() << "Condition reference not found" << ref;
        condEntries[ref].condParalyzed->addDependency(condDepend);
    }
    if (cond == 43 || cond == 44)
    {
        if (dep != 1)
            qDebug() << "Unhandled condition dep" << cond << dep;
        if (condEntries.find(ref) == condEntries.end())
            qDebug() << "Condition reference not found" << ref;
        condEntries[ref].condFinished->addDependency(condDepend, cond == 43);
    }
    if (cond == 45 || cond == 46)
    {
        if (dep != 1)
            qDebug() << "Unhandled condition dep" << cond << dep;
        if (condEntries.find(ref) == condEntries.end())
            qDebug() << "Condition reference not found" << ref;
        condEntries[ref].condBoarded->addDependency(condDepend, cond == 45);
    }
}


} // namespace fight
