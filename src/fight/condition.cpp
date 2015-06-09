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

#include "object.h"
#include "conditionmanager.h"


namespace fight {


Condition::Condition(int limit) :
    m_current(0),
    m_limit(limit),
    m_delay(0)
{
}


void Condition::setLimit(int limit)
{
    m_limit = limit;
}


void Condition::inc()
{
    m_current++;
    if (m_current == m_limit)
    {
        if (m_delay != 0)
            ConditionManager::delayComplete(this, m_delay);
        else
            complete();
    }
}


void Condition::dec()
{
    m_current--;
}


void Condition::setDelay(int delay)
{
    m_delay = delay;
}


ConditionEvent::ConditionEvent() :
    m_completed(false)
{
}

void ConditionEvent::addDependency(Condition *cond, bool positive)
{
    if (positive)
        m_dependInc << cond;
    else
    {
        m_dependDec << cond;
        cond->m_current++;
    }
}


void ConditionEvent::complete()
{
    if (m_completed)
        return;
    m_completed = true;
    foreach (Condition *cond, m_dependInc)
        cond->inc();
    foreach (Condition *cond, m_dependDec)
        cond->dec();
}


ConditionAutopilot::ConditionAutopilot(Scenario *scenario) :
    Condition(0)
{
    m_scenario = scenario;
}


void ConditionAutopilot::complete()
{
    Condition::complete();
    //m_scenario->
}


ConditionFailure::ConditionFailure(Scenario *scenario) :
    Condition(1)
{
    m_scenario = scenario;
}


void ConditionFailure::complete()
{
    Condition::complete();
    //m_scenario->
}


ConditionEnable::ConditionEnable(Object *object) :
    Condition(0)
{
    m_object = object;
}


void ConditionEnable::complete()
{
    Condition::complete();
    m_object->enable();
}


ConditionSpace::ConditionSpace(int x, int y, int dimx, int dimy, int minz, int maxz) :
    m_x(x),
    m_y(y),
    m_dimx(dimx),
    m_dimy(dimy),
    m_minz(minz),
    m_maxz(maxz)
{
}


void ConditionSpace::test(float x, float y, float height)
{
    if (!m_condEnable.isCompleted() || isCompleted())
        return;
    if (abs(x - m_x) <= m_dimx &&
        abs(y - m_y) <= m_dimy &&
        height >= m_minz && height <= m_maxz)
        ConditionEvent::complete();
}


} // namespace fight
