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

#include "sonar.h"
#include "scenario.h"
#include "sfx/samplemap.h"


namespace fight {


Sonar::Sonar(Scenario *scenario) :
    m_scenario(scenario),
    m_active(false),
    m_activatingDelay(0)
{
}


void Sonar::init(int sensor)
{
    if (sensor == 6145)
    {
        m_delay = 12000;
    }
    if (sensor == 6146)
    {
        m_delay = 10000;
    }
    if (sensor == 6147)
    {
        m_delay = 8000;
    }
    if (sensor == 6148)
    {
        m_delay = 6000;
    }
}


void Sonar::activate()
{
    m_activatingDelay = m_delay;
}


void Sonar::deactivate()
{
    m_active = false;
    m_activatingDelay = 0;
    sfx::SampleMap::get(sfx::Sample::SonarDeactivated).play();
}


void Sonar::toggle()
{
    if (!m_active && m_activatingDelay <= 0)
        activate();
    else
        deactivate();
}


void Sonar::update(float elapsedTime)
{
    if (!m_active && m_activatingDelay > 0)
    {
        m_activatingDelay -= elapsedTime;
        if (m_activatingDelay <= 0)
        {
            m_active = true;
            sfx::SampleMap::get(sfx::Sample::SonarActivated).play();
        }
    }
    if (!m_active)
        return;

    
}


} // namespace fight
