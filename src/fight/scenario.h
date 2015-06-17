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


#include <QTime>
#include "txt/desfile.h"
#include "game/renderer.h"
#include "object.h"
#include "condition.h"

#include <functional>

namespace fight {


class Object;
class Surface;
class EffectManager;
class CollisionManager;


class Scenario : public game::Renderer
{
public:
    Scenario(const QString &name, std::function<void()> &&funcSuccess);
    ~Scenario();

public:
    const glm::vec3& position() const { return m_position; }
    const glm::mat4& cameraMatrixInverted() const { return m_cameraMatrixInverted; }
    Surface *surface() const { return m_surface; }
    gfx::TextureManager& textureManager() { return m_textureManager; }
    ModuleManager& moduleManager() { return m_moduleManager; }
    EffectManager* effectManager() { return m_effectManager; }
    CollisionManager* collisionManager() { return m_collisionManager; }

protected:
    void draw();
    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent(QKeyEvent *);

private:
    glm::vec3 getPosition() const;

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

    Surface *m_surface;
    glm::vec3 m_position;
    txt::DesFile m_file;
    gfx::TextureManager m_textureManager;
    ModuleManager m_moduleManager;
    EffectManager *m_effectManager;
    CollisionManager *m_collisionManager;
    QList<Object*> m_objects;
    QList<Object*> m_lightSources;

    QTime m_time;

    float m_left;
    float m_right;
    float m_up;
    float m_down;
    float m_forwards;
    float m_backwards;

    glm::mat4 m_cameraMatrix;
    glm::mat4 m_cameraMatrixInverted;

    struct ConditionEntry
    {
        Condition *condTrigger;
        int cond1;
        int dep1;
        int ref1;
        int op;
        int cond2;
        int dep2;
        int ref2;
        int del;
        ConditionEvent *condSignal;
        ConditionEvent *condAttacked;
        ConditionEvent *condIdentified;
        ConditionEvent *condParalyzed;
        ConditionEvent *condFinished;
        ConditionEvent *condBoarded;
    };
    ConditionAutopilot m_condAutopilot;
    ConditionFailure m_condFailure;
    QMap<int, Condition> m_condObjectives;
    QList<ConditionSpace *> m_condSpaces;
    std::function<void()> m_funcSuccess;

private:
    void initCondition(const QMap<int, ConditionEntry> &entries, int cond, int dep, int ref, Condition *condDepend);
};


} // namespace fight


#endif // FIGHT_SCENARIO_H
