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

#ifndef CONDITION_H
#define CONDITION_H


#include <vector>
#include "util/geometry3d.h"
#include <QString>


namespace fight {


class Object;
class Scenario;
class Condition;


class ConditionEvent
{
public:
    ConditionEvent();

public:
    void addDependency(Condition *cond, bool positive = true);

public:
    virtual void complete();
    bool isCompleted() const { return m_completed; }

protected:
    bool m_completed;
    std::vector<Condition *> m_dependInc;
    std::vector<Condition *> m_dependDec;
};


class Condition : public ConditionEvent
{
    friend class ConditionEvent;

public:
    Condition(Scenario *scenario, int limit = 0);

public:
    void setLimit(int limit);
    int limit() const { return m_limit; }
    void tryComplete();
    void inc();
    void dec();
    void setDelay(int delay);

public:

protected:
    Scenario *m_scenario;
    int m_current;
    int m_limit;
    int m_delay;
};


class ConditionAutopilot : public Condition
{
public:
    ConditionAutopilot(Scenario *scenario);

public:
    void complete();
};


class ConditionFailure : public Condition
{
public:
    ConditionFailure(Scenario *scenario);

public:
    void complete();
};


class ConditionEnable : public Condition
{
public:
    ConditionEnable(Scenario *scenario, Object *object);

public:
    void complete();

private:
    Object *m_object;
};


class ConditionSpace : public ConditionEvent
{
public:
    ConditionSpace(Scenario *scenario, int x, int y, int dimx, int dimy, int minz, int maxz);

public:
    Condition* condEnable() { return &m_condEnable; }
    void test(float x, float y, float height);

private:
    int m_x;
    int m_y;
    int m_dimx;
    int m_dimy;
    int m_minz;
    int m_maxz;
    Condition m_condEnable;
};


class ConditionRadio : public Condition
{
public:
    ConditionRadio(Scenario *scenario, const Vector3D &pos, const QString &text);

public:
    const Vector3D& pos() const { return m_pos; }
    const QString& text() const { return m_text; }
    float time() const { return m_time; }

public:
    void complete();

private:
    Vector3D m_pos;
    QString m_text;
    float m_time;
};


} // namespace fight


#endif // CONDITION_H
