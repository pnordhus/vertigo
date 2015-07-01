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

#ifndef FIGHT_SCENARIO_H
#define FIGHT_SCENARIO_H


#include <QDebug>
#include <QTime>
#include "util/rect.hpp"
#include "util/deferreddeletable.h"
#include "object.h"
#include "surface/surface.h"
#include "effects/effectmanager.h"
#include "conditionmanager.h"

#include <functional>

namespace fight {


class Object;
class Surface;


class Scenario : public util::DeferredDeletable
{
public:
    Scenario(const QString &name);

public:
    const glm::mat4& cameraMatrix() const { return m_cameraMatrix; }
    const glm::mat4& cameraMatrixInverted() const { return m_cameraMatrixInverted; }
    const glm::vec3& position() const { return m_position; }
    const float yaw() const { return m_yaw; }
    const float time() const { return m_time; }

    Surface& surface() { return m_surface; }
    gfx::TextureManager& textureManager() { return m_textureManager; }
    ModuleManager& moduleManager() { return m_moduleManager; }
    EffectManager& effectManager() { return m_effectManager; }
    CollisionManager& collisionManager() { return m_collisionManager; }
    ConditionManager& conditionManager() { return m_conditionManager; }

public:
    void setRect(const util::RectF &rect, const glm::vec2 &center);
    void update(float elapsedTime);
    void draw();
    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent(QKeyEvent *);

private:
    glm::vec3 getPosition();

private:
    enum Type
    {
        TypeBoat           = 2049,
        TypeBomber         = 2050,
        TypeTank           = 2051,
        TypeTower          = 2052,
        TypeCrawler        = 2053,
        TypeTrash          = 2054,
        TypeBuilding       = 2056,
        TypePlayer         = 2057,
        TypeMine           = 2058,
        TypeTorpedoTower   = 2059,
        TypeSpace          = 2060,
        TypeNavPoint       = 2061,
        TypeActiveBuilding = 2062,
    };

    glm::vec3 m_position;
    float m_yaw;

    txt::DesFile m_file;

    Surface m_surface;
    gfx::TextureManager m_textureManager;
    ModuleManager m_moduleManager;
    EffectManager m_effectManager;
    CollisionManager m_collisionManager;
    ConditionManager m_conditionManager;

    std::vector<std::unique_ptr<Object>> m_objects;
    std::vector<Object*> m_lightSources;

    float m_time;

    float m_left;
    float m_right;
    float m_up;
    float m_down;
    float m_forwards;
    float m_backwards;

    glm::mat4 m_projectionMatrix;
    glm::mat4 m_projectionMatrixInverted;
    glm::mat4 m_cameraMatrix;
    glm::mat4 m_cameraMatrixInverted;
};


} // namespace fight


#endif // FIGHT_SCENARIO_H
