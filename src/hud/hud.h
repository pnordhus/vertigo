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

#ifndef HUD_HUD_H
#define HUD_HUD_H


#include "game/renderer.h"
#include "util/rect.hpp"
#include "util/event.hpp"
#include "ui/label.h"
#include "gfx/colortable.h"
#include "gfx/image.h"


namespace fight { class Scenario; class ActiveObject; }
namespace game { class Boat; }
namespace txt { class DesFile; }


namespace hud {


class HUD : public game::Renderer
{
public:
    HUD();

public:
    util::event<>& eventSuccess() { return m_eventSuccess; }

    game::Boat* boat() const { return m_boat; }
    fight::Scenario* scenario() const { return m_scenario; }
    int navPoint() const { return m_navPoint; }
    fight::ActiveObject* locked() const { return m_locked; }

    bool wide() const { return m_wide; }
    util::RectF rectHUD() const { return m_rectHUD; }
    ui::Widget* widget() { return m_integerScale ? &m_integerScaleWidget : &m_noScaleWidget; }
    gfx::Image getImage(const QString &name);
    gfx::Font& fontGreen() { return m_fontGreen; }
    gfx::Font& fontRed() { return m_fontRed; }
    gfx::Font& fontYellow() { return m_fontYellow; }

    const glm::mat4& hudProjectionMatrix() const { return m_integerScale ? m_integerScaleProjectionMatrix : m_noScaleProjectionMatrix; }
    const glm::mat4& hudProjectionMatrixInverted() const { return m_integerScale ? m_integerScaleProjectionMatrixInverted : m_noScaleProjectionMatrixInverted; }
    glm::ivec2 project(const glm::ivec2 &point);
    util::Rect projectCenter(const util::Rect &rect);

    void load(game::Boat *boat);
    void start(fight::Scenario *scenario);

    void lockReset();
    void lockReticle();

protected:
    void setRect(const QRect &rect);
    void draw();
    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent(QKeyEvent *);

private:
    util::Rect readRect(const txt::DesFile &file);

private:
    util::event<> m_eventSuccess;

    game::Boat *m_boat;
    fight::Scenario *m_scenario;
    int m_lastTicks;
    int m_navPoint;
    fight::ActiveObject *m_locked;

    bool m_wide;
    util::Rect m_rectGL;
    util::RectF m_rectHUD;
    glm::ivec2 m_center;
    bool m_integerScale;
    glm::mat4 m_integerScaleProjectionMatrix;
    glm::mat4 m_integerScaleProjectionMatrixInverted;
    glm::mat4 m_noScaleProjectionMatrix;
    glm::mat4 m_noScaleProjectionMatrixInverted;

    const gfx::ColorTable m_colorTable;
    gfx::Font m_fontGreen;
    gfx::Font m_fontRed;
    gfx::Font m_fontYellow;

    ui::Label m_cockpit;
    ui::Label m_integerScaleWidget;
    ui::Label m_noScaleWidget;
    std::vector<std::unique_ptr<ui::Widget>> m_children;
};


} // namespace hud


#endif // HUD_HUD_H
