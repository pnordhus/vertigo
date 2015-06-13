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

#ifndef FIGHT_OBJECT_H
#define FIGHT_OBJECT_H


#include <QVector3D>
#include <QVector2D>
#include <QMatrix4x4>
#include <QGLContext>
#include "modulemanager.h"
#include "scenario.h"
#include "boundingbox.h"
#include "condition.h"


namespace fight {


class Scenario;
class CollisionCache;

inline void loadMatrix(const float *data)
{
    glLoadMatrixf(data);
}

inline void loadMatrix(const double *data)
{
    glLoadMatrixd(data);
}

inline void multMatrix(const float *data)
{
    glMultMatrixf(data);
}

inline void multMatrix(const double *data)
{
    glMultMatrixd(data);
}

enum ObjectType
{
    UnknownObject,
    BuildingObject,
    TrashObject,
};


class Object
{
public:
    Object(Scenario *scenario);
    Object(Scenario *scenario, const QString &name, float scale = 1/32.0f);
    ~Object();

public:
    void setEnabled(bool);
    void enable();
    void disable();
    bool isEnabled() const { return m_enabled; }
    virtual void setPosition(const QVector3D &pos);
    QVector3D position() const { return m_position; }

    ObjectType type() const { return m_type; }
    const BoundingBox& box() const { return m_box; }
    bool isStatic() const { return m_static; }
    CollisionCache *collisionCache() const { return m_collisionCache; }
    void setCollisionCache(CollisionCache *cache);

    Condition* condEnable() { return &m_condEnable; }
    ConditionEvent* eventDestroy() { return &m_eventDestroy; }
    ConditionEvent* eventAttack() { return &m_eventAttack; }
    ConditionEvent* eventIdentify() { return &m_eventIdentify; }
    ConditionEvent* eventParalyze() { return &m_eventParalyze; }
    ConditionEvent* eventFinish() { return &m_eventFinish; }
    ConditionEvent* eventBoard() { return &m_eventBoard; }

public:
    virtual void update();
    virtual void draw();
    virtual bool intersect(const QVector3D &start, const QVector3D &dir, float radius, float &distance, QVector3D &normal);
    virtual void destroy();

protected:
    Scenario *m_scenario;

    bool m_enabled;
    Module m_base;
    float m_scale;
    QVector3D m_position;

    ObjectType m_type;
    BoundingBox m_box;
    bool m_static;
    CollisionCache *m_collisionCache;

    ConditionEnable m_condEnable;
    ConditionEvent m_eventDestroy;
    ConditionEvent m_eventAttack;
    ConditionEvent m_eventIdentify;
    ConditionEvent m_eventParalyze;
    ConditionEvent m_eventFinish;
    ConditionEvent m_eventBoard;
};


} // namespace fight


#endif // FIGHT_OBJECT_H
