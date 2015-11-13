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

#ifndef SONAR_H
#define SONAR_H


#include <vector>


namespace fight {


class Scenario;
class ActiveObject;


class Sonar
{
public:
    Sonar(Scenario *scenario);

public:
    struct SonarEntry
    {
        ActiveObject *object;
        float distance;
        bool isFriend;
        bool isPassive;
    };

    std::vector<SonarEntry>::const_iterator begin() const { return m_sonarEntries.cbegin(); }
    std::vector<SonarEntry>::const_iterator end() const { return m_sonarEntries.cend(); }

public:
    void init(int sensor);
    void activate();
    void deactivate();
    void toggle();
    bool isActive() const { return m_active; }
    bool isActivating() const { return m_activatingDelay > 0; }
    bool detectArmor() const { return m_detectArmor; }
    bool detectRange() const { return m_detectRange; }
    bool detectWeapons() const { return m_detectWeapons; }
    bool detectActivity() const { return m_detectActivity; }

public:
    void update(float elapsedTime);

private:
    Scenario *m_scenario;
    float m_delay;
    float m_passiveRange;
    float m_passiveRangeMult;
    float m_activeRange;
    float m_activeRangeMult;
    bool m_iff;
    bool m_detectArmor;
    bool m_detectRange;
    bool m_detectWeapons;
    bool m_detectActivity;

    bool m_active;
    float m_activatingDelay;
    std::vector<SonarEntry> m_sonarEntries;
};


}


#endif // SONAR_H
