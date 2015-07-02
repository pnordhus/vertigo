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
#include "SDL.h"
#include "fight/scenario.h"
#include "game/boat.h"
#include "gfx/image.h"
#include "gfx/colortable.h"

#include "heading.h"
#include "beta.h"


namespace hud {


HUD::HUD()
{

}


util::Rect HUD::readRect(const txt::DesFile &file)
{
    return util::Rect(file.value("X1").toInt(), file.value("Y1").toInt(), file.value("Width").toInt(), file.value("Height").toInt());
}


gfx::Image HUD::getImage(const QString &name)
{
    const gfx::ColorTable colorTable("gfx:pal/gui/border.pal");
    return gfx::Image::load(QString("gfx:img/cockpit/%1.img").arg(name), colorTable);
}


void HUD::load(game::Boat *boat)
{
    m_boat = boat;
    for (const auto &child : m_children)
        child->setParentWidget(nullptr);
    m_children.clear();

    m_wide = false;
    m_cockpit.setTexture(gfx::Image::load(QString("vfx:cockpit/%1/cockpit.r16").arg(boat->cockpit()), 640, 480, true));

    txt::DesFile file(QString("vfx:cockpit/%1.des").arg(boat->cockpit()));

    file.setSection("front");
    m_center = glm::ivec2(file.value("VFX_StartX").toInt() + file.value("VFX_FlightX").toInt(), file.value("VFX_StartY").toInt() + file.value("VFX_FlightY").toInt());
    ui::Label *lblTarget = new ui::Label(widget());
    lblTarget->setTexture(getImage("hudtarg"));
    lblTarget->setPosition(m_center.x - (lblTarget->texture().width() + 1)/2, m_center.y - (lblTarget->texture().height() + 1)/2);
    m_children.emplace_back(lblTarget);

    file.setSection("hudheading");
    Heading *heading = new Heading(this, readRect(file));
    m_children.emplace_back(heading);

    file.setSection("hudbeta");
    Beta *beta = new Beta(this, readRect(file));
    m_children.emplace_back(beta);
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
        m_rectHUD = util::RectF(rect.x(), rect.y() - rectOrtho().y()/rectOrtho().height()*rect.height(), rect.width(), rect.height()*480/rectOrtho().height());
    else
        m_rectHUD = util::RectF(rect.x() - rectOrtho().x()/rectOrtho().width()*rect.width(), rect.y() - rectOrtho().y()/rectOrtho().height()*rect.height(), rect.width()*640/rectOrtho().width(), rect.height()*480/rectOrtho().height());
    if (m_scenario)
        m_scenario->setRect(m_rectHUD, glm::vec2(m_projectionMatrix * glm::vec4(m_center, 0, 1)));
}


void HUD::draw()
{
    int ticks = SDL_GetTicks();
    float elapsedTime = ticks - m_lastTicks;
    m_lastTicks = ticks;
    m_scenario->update(elapsedTime);

    if (m_rectHUD.x > 0 || m_rectHUD.y > 0)
        glViewport(m_rectHUD.x, m_rectHUD.y, m_rectHUD.width, m_rectHUD.height);
    if (m_scenario)
        m_scenario->draw();
    if (m_rectHUD.x > 0 || m_rectHUD.y > 0)
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
