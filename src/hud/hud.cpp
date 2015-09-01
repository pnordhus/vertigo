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
#include "fight/objects/navpoint.h"
#include "fight/objects/activeobject.h"
#include "game/boat.h"
#include "gfx/image.h"

#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "crosshair.h"
#include "heading.h"
#include "beta.h"
#include "meter.h"
#include "digiblock.h"
#include "radiomessage.h"
#include "energybar.h"
#include "shield.h"
#include "activesonar.h"
#include "master.h"
#include "mastermonitor.h"
#include "sonarmonitor.h"
#include "autopilot.h"


namespace hud {


HUD::HUD() :
    m_scenario(nullptr),
    m_colorTable("gfx:pal/gui/border.pal"),
    m_fontGreen("gfx:fnt/cpit1ahc.fnt", m_colorTable, true, false),
    m_fontRed("gfx:fnt/cpit1bhc.fnt", m_colorTable, true, false),
    m_fontYellow("gfx:fnt/cpit1chc.fnt", m_colorTable, true, false)
{

}


Rect HUD::readRect(const txt::DesFile &file)
{
    return Rect(file.value("X1").toInt(), file.value("Y1").toInt(), file.value("Width").toInt(), file.value("Height").toInt());
}


gfx::Image HUD::getImage(const QString &name)
{
    return gfx::Image::load(QString("gfx:img/cockpit/%1.img").arg(name), m_colorTable);
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

    m_integerScale = true;

    file.setSection("hudshield");
    Shield *shield = new Shield(this, readRect(file));
    m_children.emplace_back(shield);

    file.setSection("hudactivesonarphase");
    ActiveSonar *activeSonar = new ActiveSonar(this, readRect(file));
    m_children.emplace_back(activeSonar);

    file.setSection("hudautopilot");
    Autopilot *autopilot = new Autopilot(this, readRect(file));
    m_children.emplace_back(autopilot);

    file.setSection("radiomessage");
    RadioMessage *radioMessage = new RadioMessage(this, Rect(file.value("X").toInt(), file.value("Y").toInt(), 640 - file.value("X").toInt()*2, 12));
    m_children.emplace_back(radioMessage);

    m_integerScale = true;

    file.setSection("hudheading");
    Heading *heading = new Heading(this, readRect(file));
    m_children.emplace_back(heading);

    file.setSection("hudbeta");
    Beta *beta = new Beta(this, readRect(file));
    m_children.emplace_back(beta);

    file.setSection("hudmeter");
    Meter *meter = new Meter(this, readRect(file), glm::ivec2(file.value("BarX1").toInt(), file.value("BarY1").toInt()), file.value("BarHeight").toInt(), false);
    m_children.emplace_back(meter);

    file.setSection("hudkmeter");
    Meter *kmeter = new Meter(this, readRect(file), glm::ivec2(file.value("BarX1").toInt(), file.value("BarY1").toInt()), file.value("BarHeight").toInt(), true);
    m_children.emplace_back(kmeter);

    file.setSection("huddigiblock1");
    DigiBlock *digiBlock = new DigiBlock(this, readRect(file));
    m_children.emplace_back(digiBlock);

    file.setSection("hudenergybar");
    EnergyBar *energyBar = new EnergyBar(this, readRect(file));
    m_children.emplace_back(energyBar);

    file.setSection("mastermonitor");
    MasterMonitor *masterMonitor = new MasterMonitor(this, readRect(file), Rect(file.value("ObservationX1").toInt(), file.value("ObservationY1").toInt(), file.value("ObservationWidth").toInt(), file.value("ObservationHeight").toInt()));
    m_children.emplace_back(masterMonitor);

    file.setSection("roundmonitor0");
    m_sonar1 = new SonarMonitor(this, Point(file.value("X1").toInt(), file.value("Y1").toInt()), file.value("Radius").toInt(), 360);
    m_children.emplace_back(m_sonar1);

    file.setSection("roundmonitor1");
    m_sonar2 = new SonarMonitor(this, Point(file.value("X1").toInt(), file.value("Y1").toInt()), file.value("Radius").toInt(), 180);
    m_children.emplace_back(m_sonar2);

    m_integerScale = false;

    file.setSection("masterhud");
    Master *master = new Master(this, readRect(file));
    m_children.emplace_back(master);

    Crosshair *crosshair = new Crosshair(this, m_center, &master->clipRect());
    m_children.emplace_back(crosshair);
}


void HUD::start(fight::Scenario *scenario)
{
    hideCursor();
    m_rectGL = Rect();
    m_scenario = scenario;
    m_scenario->setBoat(m_boat);
    m_lastTicks = SDL_GetTicks();
    m_navPoint = -1;
}


void HUD::setRect(const QRect &rect)
{
    if (m_rectGL.width == rect.width() && m_rectGL.height == rect.height())
        return;

    m_rectGL = Rect(rect.x(), rect.y(), rect.width(), rect.height());
    Renderer::setRect(rect);

    if (m_wide)
        m_rectHUD = RectF(rect.x(), rect.y() - rectOrtho().y()/rectOrtho().height()*rect.height(), rect.width(), rect.height()*480/rectOrtho().height());
    else
        m_rectHUD = RectF(rect.x() - rectOrtho().x()/rectOrtho().width()*rect.width(), rect.y() - rectOrtho().y()/rectOrtho().height()*rect.height(), rect.width()*640/rectOrtho().width(), rect.height()*480/rectOrtho().height());
    Renderer::setRect(QRect(glm::round(m_rectHUD.x), glm::round(m_rectHUD.y), glm::round(m_rectHUD.width), glm::round(m_rectHUD.height)));

    if (m_scenario)
        m_scenario->setRect(m_rectHUD, Vector2D(m_projectionMatrix * Vector4D(m_center, 0, 1)));

    int w = 640;
    int h = 480;
    while (w + 640 <= rect.width() && h + 480 <= rect.height())
    {
        w += 640;
        h += 480;
    }
    m_integerScaleProjectionMatrix = glm::ortho(0.0f, m_rectHUD.width*640/w, m_rectHUD.height*480/h, 0.0f);
    m_integerScaleProjectionMatrixInverted = glm::inverse(m_integerScaleProjectionMatrix);
    m_noScaleProjectionMatrix = glm::ortho(0.0f, m_rectHUD.width, m_rectHUD.height, 0.0f);
    m_noScaleProjectionMatrixInverted = glm::inverse(m_noScaleProjectionMatrix);
}


Point HUD::project(const Point &point)
{
    return glm::ivec2(glm::round(hudProjectionMatrixInverted() * (m_projectionMatrix * Vector4D(point, 0, 1))));
}


Rect HUD::projectCenter(const Rect &rect)
{
    return Rect(project(rect.center()) - glm::ivec2(rect.width/2, rect.height/2), rect.size());
}


void HUD::draw()
{
    if (!m_scenario)
    {
        setupGL(true);
        m_cockpit.doDraw();
        return;
    }

    int ticks = SDL_GetTicks();
    float elapsedTime = ticks - m_lastTicks;
    m_lastTicks = ticks;
    m_scenario->update(elapsedTime);

    if (rect().x() != m_rectGL.x || rect().y() != m_rectGL.y)
        glViewport(rect().x(), rect().y(), rect().width(), rect().height());

    m_scenario->draw();

    setupGL(false);
    m_cockpit.doDraw();

    m_integerScale = true;
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(glm::value_ptr(m_integerScaleProjectionMatrix));
    m_integerScaleWidget.doDraw();

    m_integerScale = false;
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(glm::value_ptr(m_noScaleProjectionMatrix));
    m_noScaleWidget.doDraw();

    if (rect().x() != m_rectGL.x || rect().y() != m_rectGL.y)
        glViewport(m_rectGL.x, m_rectGL.y, m_rectGL.width, m_rectGL.height);
}


void HUD::keyPressEvent(QKeyEvent *e)
{
    if (!m_scenario)
        return;

    if (e->key() == Qt::Key_Escape)
        m_eventSuccess();
    if (e->key() == Qt::Key_N)
    {
        if ((e->modifiers() & ~Qt::KeypadModifier) == Qt::ALT)
        {
            if (m_navPoint >= 0)
                m_scenario->target().lockNavPoint(m_scenario->navPoints()[m_navPoint]);
        }
        else
        {
            m_navPoint++;
            while (m_navPoint < m_scenario->navPoints().size() && !m_scenario->navPoints()[m_navPoint]->isEnabled())
                m_navPoint++;
            if (m_navPoint >= m_scenario->navPoints().size())
                m_navPoint = -1;
        }
    }
    if (e->key() == Qt::Key_R)
    {
        if ((e->modifiers() & ~Qt::KeypadModifier) == Qt::ALT)
        {
            int scale = m_sonar1->scale();
            if (scale == 90)
                scale = 360;
            else
                scale /= 2;
            m_sonar1->setScale(scale);
            m_sonar2->setScale(scale/2);
        }
        else
            m_scenario->sonar().toggle();
    }
    if (e->key() == Qt::Key_L)
        m_scenario->target().lockReticle();
    if (e->key() == Qt::Key_A)
    {
        if ((e->modifiers() & ~Qt::KeypadModifier) == Qt::ALT)
        {
            if (m_scenario->conditionManager().autopilot())
            {
                m_scenario = nullptr;
                m_eventSuccess.fire();
                return;
            }
        }
    }

    m_scenario->keyPressEvent(e);
}


void HUD::keyReleaseEvent(QKeyEvent *e)
{
    if (!m_scenario)
        return;

    m_scenario->keyReleaseEvent(e);
}


} // namespace hud
