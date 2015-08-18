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
#include "util/geometry2d.h"
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

    bool wide() const { return m_wide; }
    const RectF& rectHUD() const { return m_rectHUD; }
    const Point& center() const { return m_center; }
    ui::Widget* widget() { return m_integerScale ? &m_integerScaleWidget : &m_noScaleWidget; }
    gfx::Image getImage(const QString &name);
    gfx::Font& fontGreen() { return m_fontGreen; }
    gfx::Font& fontRed() { return m_fontRed; }
    gfx::Font& fontYellow() { return m_fontYellow; }

    const Matrix& hudProjectionMatrix() const { return m_integerScale ? m_integerScaleProjectionMatrix : m_noScaleProjectionMatrix; }
    const Matrix& hudProjectionMatrixInverted() const { return m_integerScale ? m_integerScaleProjectionMatrixInverted : m_noScaleProjectionMatrixInverted; }
    Point project(const Point &point);
    Rect projectCenter(const Rect &rect);

    void load(game::Boat *boat);
    void start(fight::Scenario *scenario);

protected:
    void setRect(const QRect &rect);
    void draw();
    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent(QKeyEvent *);

private:
    Rect readRect(const txt::DesFile &file);

private:
    util::event<> m_eventSuccess;

    game::Boat *m_boat;
    fight::Scenario *m_scenario;
    int m_lastTicks;
    int m_navPoint;

    bool m_wide;
    Rect m_rectGL;
    RectF m_rectHUD;
    Point m_center;
    bool m_integerScale;
    Matrix m_integerScaleProjectionMatrix;
    Matrix m_integerScaleProjectionMatrixInverted;
    Matrix m_noScaleProjectionMatrix;
    Matrix m_noScaleProjectionMatrixInverted;

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
