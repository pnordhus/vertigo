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

#include "digiblock.h"
#include "hud.h"
#include "fight/scenario.h"
#include "fight/objects/navpoint.h"
#include <glm/common.hpp>
#include <sstream>


namespace hud {


DigiBlock::DigiBlock(HUD *hud, util::Rect rect) :
    ui::Widget(hud->widget()),
    m_hud(hud),
    m_rect(rect)
{
    for (int i = 0; i < 7; i++)
    {
        ui::Label *lbl = new ui::Label(this);
        m_children.emplace_back(lbl);
    }
}


void DigiBlock::draw()
{
    glm::ivec2 p = m_hud->project(m_rect.pos());
    for (int i = 0; i < 7; i++)
        m_children[i]->setPosition(p.x, p.y + 8*i);

    int depth = static_cast<int>(glm::round(m_hud->scenario()->depth()));
    m_children[0]->setFont(depth > 100 ? m_hud->fontGreen() : m_hud->fontRed());
    m_children[0]->setText(QString("%1M").arg(depth));
    
    int height = static_cast<int>(glm::round(m_hud->scenario()->height()));
    m_children[1]->setFont(height > 5 ? m_hud->fontGreen() : m_hud->fontRed());
    m_children[1]->setText(QString("%1M").arg(height));
    
    int speed = static_cast<int>(glm::round(m_hud->scenario()->speed()));
    m_children[2]->setFont(speed > -5 ? m_hud->fontGreen() : m_hud->fontRed());
    m_children[2]->setText(QString("%1KM/H").arg(speed));
    
    int noise = 1 + static_cast<int>(glm::round(m_hud->scenario()->noise()));
    m_children[3]->setFont(noise == 1 ? m_hud->fontGreen() : m_hud->fontRed());
    m_children[3]->setText(QString("NL %1").arg(noise));

    int time = static_cast<int>(m_hud->scenario()->time()/1000);
    std::stringstream st;
    if (time >= 3600)
    {
        st << time/3600 << ':';
        time %= 3600;
    }
    st.fill('0');
    st.width(2);
    st << time/60 << ':';
    st.fill('0');
    st.width(2);
    st << time%60;
    m_children[4]->setFont(m_hud->fontGreen());
    m_children[4]->setText(st.str().c_str());

    int nav = 0;
    m_children[5]->setFont(m_hud->fontGreen());
    m_children[5]->setText(m_hud->navPoint() < 0 ? QString("NAV NONE") : QString("NAV %1").arg(static_cast<char>('A' + m_hud->scenario()->navPoints()[m_hud->navPoint()]->num())));

    int buzzers = m_hud->scenario()->buzzers();
    m_children[6]->setFont(buzzers > 0 ? m_hud->fontGreen() : m_hud->fontRed());
    m_children[6]->setText(QString("BUZ %1").arg(buzzers));
}

} // namespace hud
