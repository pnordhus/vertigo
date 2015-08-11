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

#include <glm/common.hpp>

namespace hud {


Master::Master(HUD *hud, util::Rect rect) :
    ui::Widget(hud->widget()),
    m_hud(hud),
    m_rect(rect),
    m_edgeBL(hud->getImage("hudedgbl"), false),
    m_edgeBR(hud->getImage("hudedgbr"), false),
    m_edgeTL(hud->getImage("hudedgtl"), false),
    m_edgeTR(hud->getImage("hudedgtr"), false),
    m_activeBlue(hud->getImage("hudscnab"), false),
    m_activeGreen(hud->getImage("hudscnag"), false),
    m_activeRed(hud->getImage("hudscnar"), false),
    m_passiveBlue(hud->getImage("hudscnpb"), false),
    m_passiveGreen(hud->getImage("hudscnpg"), false),
    m_passiveRed(hud->getImage("hudscnpr"), false)
{
}


void Master::draw()
{
    glm::ivec2 rectPos = m_hud->project(m_rect.pos());
    util::Rect rect(rectPos, m_hud->project(m_rect.pos() + m_rect.size()) - rectPos);
    m_clipRect.setRect(rect);

    util::RectangleInclusive<int> rectInc = rect;
    m_edgeBL.draw(rectInc.left(), rectInc.bottom() - m_edgeBL.height());
    m_edgeBR.draw(rectInc.right() - m_edgeBR.width(), rectInc.bottom() - m_edgeBR.height());
    m_edgeTL.draw(rectInc.left(), rectInc.top());
    m_edgeTR.draw(rectInc.right() - m_edgeTR.width(), rectInc.top());

    glm::mat4 m = m_hud->hudProjectionMatrixInverted() * m_hud->scenario()->projectionMatrix() * m_hud->scenario()->cameraMatrix();
    for (const auto &entry : m_hud->scenario()->sonar())
    {
        glm::vec4 point4 = m * glm::vec4(entry.object->center() - m_hud->scenario()->position(), 1);
        if (point4.z > 0)
            continue;
        glm::ivec2 point = glm::round(glm::vec2(point4)/point4.w);
        gfx::Texture *tex;
        if (entry.isPassive)
            tex = entry.isFriend ? &m_passiveGreen : &m_passiveRed;
        else
            tex = entry.isFriend ? &m_activeGreen : &m_activeRed;
        m_clipRect.draw(*tex, point.x - tex->width()/2, point.y - tex->height()/2);
    }
}


} // namespace hud
