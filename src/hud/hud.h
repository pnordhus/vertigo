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


namespace fight { class Scenario; }
namespace game { class Boat; }


namespace hud {


class HUD : public game::Renderer
{
public:
    HUD();

public:
    util::event<>& eventSuccess() { return m_eventSuccess; }
    bool wide() const { return m_wide; }
    util::RectF rectHUD() const { return m_rectHUD; }
    game::Boat* boat() const { return m_boat; }
    fight::Scenario* scenario() const { return m_scenario; }

    void load(game::Boat *boat);
    void start(fight::Scenario *scenario);

protected:
    void setRect(const QRect &rect);
    void draw();
    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent(QKeyEvent *);

private:
    util::event<> m_eventSuccess;
    game::Boat *m_boat;
    bool m_wide;
    util::RectF m_rectHUD;
    glm::ivec2 m_center;
    fight::Scenario *m_scenario;
    int m_lastTicks;

    ui::Label m_cockpit;
    std::vector<std::unique_ptr<ui::Widget>> m_children;
};


} // namespace hud


#endif // HUD_HUD_H
