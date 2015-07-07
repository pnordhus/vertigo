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
#include "scenario.h"
#include <glm/common.hpp>


namespace fight {


Condition::Condition(Scenario *scenario, int limit) :
    m_scenario(scenario),
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
            m_scenario->conditionManager().delayComplete(this, m_delay);
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
    Condition(scenario, 0)
{
}


void ConditionAutopilot::complete()
{
    Condition::complete();
    //m_scenario->
}


ConditionFailure::ConditionFailure(Scenario *scenario) :
    Condition(scenario, 1)
{
}


void ConditionFailure::complete()
{
    Condition::complete();
    //m_scenario->
}


ConditionEnable::ConditionEnable(Scenario *scenario, Object *object) :
    Condition(scenario, 0),
    m_object(object)
{
}


void ConditionEnable::complete()
{
    Condition::complete();
    m_object->enable();
}


ConditionSpace::ConditionSpace(Scenario *scenario, int x, int y, int dimx, int dimy, int minz, int maxz) :
    m_x(x),
    m_y(y),
    m_dimx(dimx),
    m_dimy(dimy),
    m_minz(minz),
    m_maxz(maxz),
    m_condEnable(scenario)
{
}


void ConditionSpace::test(float x, float y, float height)
{
    if (!m_condEnable.isCompleted() || isCompleted())
        return;
    if (glm::abs(x - m_x) <= m_dimx &&
        glm::abs(y - m_y) <= m_dimy &&
        height >= m_minz && height <= m_maxz)
        ConditionEvent::complete();
}


ConditionRadio::ConditionRadio(Scenario *scenario, const glm::vec3 &pos, const QString &text) :
    Condition(scenario, 0),
    m_pos(pos),
    m_text(text)
{
}


void ConditionRadio::complete()
{
    Condition::complete();
    m_time = m_scenario->time();
    if (m_scenario->radio().size() > 0 && m_time < m_scenario->radio().back()->time() + 5000.0f)
        m_time = m_scenario->radio().back()->time() + 5000.0f;
    m_scenario->radio().push_back(this);
}


} // namespace fight
