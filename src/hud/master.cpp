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

#include "master.h"
#include "hud.h"
#include "fight/scenario.h"
#include "fight/objects/navpoint.h"
#include "fight/objects/activeobject.h"
#include "sfx/samplemap.h"


namespace hud {


Master::Master(HUD *hud, util::Rect rect) :
    ui::Widget(hud->widget()),
    m_hud(hud),
    m_rect(rect),
    m_edgeBL(hud->getImage("hudedgbl"), false),
    m_edgeBR(hud->getImage("hudedgbr"), false),
    m_edgeTL(hud->getImage("hudedgtl"), false),
    m_edgeTR(hud->getImage("hudedgtr"), false),
    m_arrowD(hud->getImage("hudarrd"), false),
    m_arrowL(hud->getImage("hudarrl"), false),
    m_arrowR(hud->getImage("hudarrr"), false),
    m_arrowU(hud->getImage("hudarru"), false),
    m_activeBlue(hud->getImage("hudscnab"), false),
    m_activeGreen(hud->getImage("hudscnag"), false),
    m_activeRed(hud->getImage("hudscnar"), false),
    m_passiveBlue(hud->getImage("hudscnpb"), false),
    m_passiveGreen(hud->getImage("hudscnpg"), false),
    m_passiveRed(hud->getImage("hudscnpr"), false),
    m_lockActiveYellow(hud->getImage("hudlocaa"), false),
    m_lockActiveGreen(hud->getImage("hudlocag"), false),
    m_lockActiveRed(hud->getImage("hudlocar"), false),
    m_lockPassiveYellow(hud->getImage("hudlocpa"), false),
    m_lockPassiveGreen(hud->getImage("hudlocpg"), false),
    m_lockPassiveRed(hud->getImage("hudlocpr"), false),
    m_way(hud->getImage("hudway1"), false)
{
}


void Master::draw()
{
    util::Point rectPos = m_hud->project(m_rect.pos());
    util::Rect rect(rectPos, m_hud->project(m_rect.pos() + m_rect.size()) - rectPos);
    m_clipRect.setRect(rect);

    util::RectangleInclusive<int> rectInc = rect;
    m_edgeBL.draw(rectInc.left(), rectInc.bottom() - m_edgeBL.height());
    m_edgeBR.draw(rectInc.right() - m_edgeBR.width(), rectInc.bottom() - m_edgeBR.height());
    m_edgeTL.draw(rectInc.left(), rectInc.top());
    m_edgeTR.draw(rectInc.right() - m_edgeTR.width(), rectInc.top());

    glm::mat4 m = m_hud->hudProjectionMatrixInverted() * m_hud->scenario()->projectionMatrix() * m_hud->scenario()->cameraMatrix();
    fight::Target &target = m_hud->scenario()->target();
    bool lockNotLost = target.locked() == nullptr;

    for (const auto &entry : m_hud->scenario()->sonar())
    {
        lockNotLost |= target.locked() == entry.object;
        glm::vec4 point4 = m * glm::vec4(entry.object->center() - m_hud->scenario()->position(), 1);
        drawPoint(point4, rect, entry.object == target.locked(), entry.isFriend, entry.isPassive);
    }
    if (target.lockedNavPoint() != nullptr)
    {
        glm::vec4 point4 = m * glm::vec4(target.lockedNavPoint()->position() - m_hud->scenario()->position(), 1);
        drawPoint(point4, rect, true, true, true);
    }

    if (m_hud->navPoint() >= 0)
    {
        glm::vec4 point4 = m * glm::vec4(m_hud->scenario()->navPoints()[m_hud->navPoint()]->position() - m_hud->scenario()->position(), 1);
        if (point4.z > 0)
            return;
        util::Point point = glm::round(glm::vec2(point4)/point4.w);
        m_way.draw(point.x - (m_way.width() + 1)/2, point.y - (m_way.height() + 1)/2, &m_clipRect);

        m_hud->fontYellow().draw(QString("NAV %1").arg(static_cast<char>('A' + m_hud->scenario()->navPoints()[m_hud->navPoint()]->num())), point.x - 16, point.y - 17, &m_clipRect);
    }

    if (!lockNotLost)
    {
        target.lockReset();
        sfx::SampleMap::get(sfx::Sample::TargetLost).play();
    }
}


void Master::drawPoint(const glm::vec4 &point4, const util::Rect &rect, bool isLocked, bool isFriend, bool isPassive)
{
    gfx::Texture *tex;
    util::Point point = glm::round(glm::vec2(point4)/point4.w);

    if (isLocked && (point4.z > 0 || !rect.contains(point)))
    {
        util::Point center = m_hud->project(m_hud->center());
        glm::vec2 dir = glm::vec2(point4) - glm::vec2(center)*point4.w;
        glm::vec2 dim;
        if (dir.x > 0)
            dim.x = rect.right() - center.x;
        else
            dim.x = rect.left() - center.x;
        if (dir.y > 0)
            dim.y = rect.bottom() - center.y;
        else
            dim.y = rect.top() - center.y;
        util::Point p = center + util::Point(dim.x, dim.x/dir.x*dir.y);
        if (rect.contains(p))
        {
            if (p.x > center.x)
                m_arrowR.draw(p.x - m_arrowR.width() + 1, p.y - m_arrowR.height()/2);
            else
                m_arrowL.draw(p.x, p.y - m_arrowL.height()/2);
        }
        else
        {
            p = center + util::Point(dim.y/dir.y*dir.x, dim.y);
            if (p.y > center.y)
                m_arrowD.draw(p.x - m_arrowD.width()/2, p.y - m_arrowD.height() + 1);
            else
                m_arrowU.draw(p.x - m_arrowD.width()/2, p.y);
        }
    }
    if (point4.z > 0)
        return;
    
    if (isLocked)
    {
        if (isPassive)
            tex = isFriend && m_hud->scenario()->blink() ? &m_lockPassiveGreen : &m_lockPassiveRed;
        else
            tex = isFriend && m_hud->scenario()->blink() ? &m_lockActiveGreen : &m_lockActiveRed;
        tex->draw(point.x - tex->width()/2, point.y - tex->height()/2, &m_clipRect);
    }
    else
    {
        if (isPassive)
            tex = isFriend ? &m_passiveGreen : &m_passiveRed;
        else
            tex = isFriend ? &m_activeGreen : &m_activeRed;
        tex->draw(point.x - tex->width()/2, point.y - tex->height()/2, &m_clipRect);
    }
}


} // namespace hud
