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


#include <QGLContext>
#include "fight/boundingbox.h"
#include "fight/condition.h"
#include <memory>


namespace txt { class DesFile; }


namespace fight {


class Scenario;


class Object
{
public:
    Object(Scenario *scenario);
    Object(Scenario *scenario, txt::DesFile &file, float scale = 1/32.0f);
    Object(const Object& x) = delete;
    virtual ~Object() { }

public:
    void setEnabled(bool);
    void enable();
    void disable();
    bool isEnabled() const { return m_enabled; }

    virtual void setPosition(const Vector3D &pos);
    const Vector3D& position() const { return m_position; }
    const BoundingBox& box() const { return m_box; }
    virtual Vector3D center() const { return (m_box.minPoint() + m_box.maxPoint())*0.5f; }
    bool isStatic() const { return m_static; }

    Condition* condEnable() { return &m_condEnable; }

public:
    virtual bool update(float elapsedTime);
    virtual void draw();
    virtual bool intersect(const Vector3D &start, const Vector3D &dir, float radius, float &distance, Vector3D &normal);

protected:
    Scenario *m_scenario;

    bool m_enabled;
    Vector3D m_position;

    BoundingBox m_box;
    bool m_static;

    ConditionEnable m_condEnable;
};


} // namespace fight


#endif // FIGHT_OBJECT_H
