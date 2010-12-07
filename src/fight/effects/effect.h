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

#ifndef FIGHT_EFFECT_H
#define FIGHT_EFFECT_H


#include "../object.h"


namespace fight {


class Billboard;


class Effect : public Object
{
public:
    Effect(Scenario *scenario, Billboard *billboard, float angle);

public:
    void update();
    void draw();
    void setPermanent(bool permanent);

protected:
    Billboard *m_billboard;
    float m_angle;
    QTime m_time;
    int m_elapsedTime;
    bool m_permanent;
};


} // namespace fight


#endif // FIGHT_EFFECT_H
