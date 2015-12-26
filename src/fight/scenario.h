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

#include <QKeyEvent>
#include <functional>
#include "util/geometry2d.h"
#include "util/geometry3d.h"
#include "util/deferreddeletable.h"
#include "gfx/texturemanager.h"
#include "txt/desfile.h"
#include "boats/player.h"
#include "surface/surface.h"
#include "effects/effectmanager.h"
#include "conditionmanager.h"
#include "collisionmanager.h"
#include "modulemanager.h"
#include "sonar.h"
#include "target.h"

namespace game { class Boat; }

namespace fight {

class Object;
class NavPoint;

class Scenario : public util::DeferredDeletable
{
public:
    Scenario(const QString &name);

public:
    const Matrix& projectionMatrix() const { return m_projectionMatrix; }
    const Matrix& projectionMatrixInverted() const { return m_projectionMatrixInverted; }
    const Matrix& cameraMatrix() const { return m_cameraMatrix; }
    const Matrix& cameraMatrixInverted() const { return m_cameraMatrixInverted; }
    const Vector3D& position() const { return m_position; }
    float yaw() const { return m_yaw; }
    float pitch() const { return m_pitch; }
    float height() const { return m_height; }
    float depth() const { return m_depth - m_position.z; }
    float time() const { return m_time; }
    int buzzers() const { return m_buzzers; }
    bool blink() const { return static_cast<int>(m_time/0.5f)%2 == 0; }
    int endType() const { return m_endType; }

    Player* player() const { return m_player.get(); }

    Surface& surface() { return m_surface; }
    gfx::TextureManager& textureManager() { return m_textureManager; }
    ModuleManager& moduleManager() { return m_moduleManager; }
    EffectManager& effectManager() { return m_effectManager; }
    CollisionManager& collisionManager() { return m_collisionManager; }
    ConditionManager& conditionManager() { return m_conditionManager; }
    std::vector<ConditionRadio*>& radio() { return m_radio; }
    Sonar& sonar() { return m_sonar; }
    Target& target() { return m_target; }

    std::vector<std::unique_ptr<Object>>::const_iterator begin() const { return m_objects.cbegin(); }
    std::vector<std::unique_ptr<Object>>::const_iterator end() const { return m_objects.cend(); }
    std::vector<NavPoint*>& navPoints() { return m_navPoints; }
    bool isFriend(int iffX, int iffY) const { return m_attitudeMatrix[iffY][iffX] == 0; }

public:
    void setBoat(game::Boat *boat);
    void setRect(const RectF &rect, const Vector2D &center);
    void update(float elapsedTime);
    void draw();
    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent(QKeyEvent *);

private:
    Vector3D getPosition();

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

    Vector3D m_position;
    float m_yaw;
    float m_pitch;
    float m_height;
    int m_buzzers;
    std::unique_ptr<Player> m_player;
    float m_velocityTarget;

    txt::DesFile m_file;
    int m_depth;

    Surface m_surface;
    gfx::TextureManager m_textureManager;
    ModuleManager m_moduleManager;
    EffectManager m_effectManager;
    CollisionManager m_collisionManager;
    ConditionManager m_conditionManager;
    std::vector<ConditionRadio*> m_radio;
    Sonar m_sonar;
    Target m_target;

    std::vector<std::unique_ptr<Object>> m_objects;
    std::vector<Object*> m_lightSources;
    std::vector<NavPoint*> m_navPoints;

    char m_attitudeMatrix[9][9];
    int m_endType;

    float m_time;

    float m_left;
    float m_right;
    float m_up;
    float m_down;
    int m_forwards;
    int m_backwards;

    Matrix m_projectionMatrix;
    Matrix m_projectionMatrixInverted;
    Matrix m_cameraMatrix;
    Matrix m_cameraMatrixInverted;
};

} // namespace fight

#endif // FIGHT_SCENARIO_H
