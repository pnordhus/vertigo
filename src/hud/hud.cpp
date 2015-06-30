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

#include "hud.h"
#include <QKeyEvent>
#include "fight/scenario.h"
#include "game/boat.h"
#include "gfx/image.h"
#include "SDL.h"


namespace hud {


HUD::HUD()
{

}


void HUD::load(game::Boat *boat)
{
    m_boat = boat;
    m_wide = false;
    m_cockpit.setTexture(gfx::Image::load(QString("vfx:cockpit/%1/cockpit.r16").arg(boat->cockpit()), 640, 480, true));
}


void HUD::start(fight::Scenario *scenario)
{
    m_scenario = scenario;
    hideCursor();
    m_lastTicks = SDL_GetTicks();
}


void HUD::setRect(const QRect &rect)
{
    Renderer::setRect(rect);

    if (m_wide)
        m_rectHUD = QRect(rect.x(), rect.y() - rectOrtho().y()/rectOrtho().height()*rect.height(), rect.width(), rect.height()*480/rectOrtho().height());
    else
        m_rectHUD = QRect(rect.x() - rectOrtho().x()/rectOrtho().width()*rect.width(), rect.y() - rectOrtho().y()/rectOrtho().height()*rect.height(), rect.width()*640/rectOrtho().width(), rect.height()*480/rectOrtho().height());
    if (m_scenario)
        m_scenario->setRect(m_rectHUD);
}


void HUD::draw()
{
    int ticks = SDL_GetTicks();
    float elapsedTime = ticks - m_lastTicks;
    m_lastTicks = ticks;
    m_scenario->update(elapsedTime);

    if (m_rectHUD.top() > 0 || m_rectHUD.left() > 0)
        glViewport(m_rectHUD.left(), m_rectHUD.top(), m_rectHUD.width(), m_rectHUD.height());
    if (m_scenario)
        m_scenario->draw();
    if (m_rectHUD.top() > 0 || m_rectHUD.left() > 0)
        glViewport(0, 0, rect().width(), rect().height());

    setupGL(false);

    m_cockpit.doDraw();
}


void HUD::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Escape)
        m_eventSuccess();

    if (m_scenario)
        m_scenario->keyPressEvent(e);
}


void HUD::keyReleaseEvent(QKeyEvent *e)
{
    if (m_scenario)
        m_scenario->keyReleaseEvent(e);
}


} // namespace hud
