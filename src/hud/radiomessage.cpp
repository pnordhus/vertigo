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

#include "radiomessage.h"
#include "hud.h"
#include "fight/scenario.h"
#include "sfx/samplemap.h"


namespace hud {


RadioMessage::RadioMessage(HUD *hud, util::Rect rect) :
    ui::Widget(hud->widget()),
    m_hud(hud),
    m_rect(rect),
    m_lblText(this, false),
    m_edgeBL(hud->getImage("hudedgbl"), false),
    m_edgeBR(hud->getImage("hudedgbr"), false),
    m_edgeTL(hud->getImage("hudedgtl"), false),
    m_edgeTR(hud->getImage("hudedgtr"), false)
{
    m_lblText.setFont(m_hud->fontGreen());
    m_lblText.setAlignment(ui::Label::AlignHCenter);
    m_black.createEmpty(rect.width, rect.height, gfx::Texture::Format::RGB, false);
}


void RadioMessage::draw()
{
    int i = m_hud->scenario()->radio().size() - 1;
    while (i >= 0 && m_hud->scenario()->radio()[i]->time() > m_hud->scenario()->time())
        i--;
    if (i < 0 || m_hud->scenario()->radio()[i]->time() < m_hud->scenario()->time() - 4500.0f)
    {
        m_lblText.setVisible(false);
        return;
    }
    if (!m_lblText.isVisible())
    {
        m_lblText.setVisible(true);
        sfx::SampleMap::get(sfx::Sample::Radio).play();
    }

    util::RectangleInclusive<int> rect = m_hud->projectCenter(m_rect);
    m_black.draw(QRectF(rect.x, rect.y, rect.width, rect.height));
    m_edgeBL.draw(rect.left(), rect.bottom() - m_edgeBL.height());
    m_edgeBR.draw(rect.right() - m_edgeBR.width(), rect.bottom() - m_edgeBR.height());
    m_edgeTL.draw(rect.left(), rect.top());
    m_edgeTR.draw(rect.right() - m_edgeTR.width(), rect.top());

    m_lblText.setPosition(rect.x, rect.y + 2);
    m_lblText.setSize(rect.width, rect.height - 5);
    QString text = m_hud->scenario()->radio()[i]->text();
    if (text[0] == '#')
    {
        text = text.mid(1);
        m_lblText.setFont(m_hud->fontRed());
    }
    else
        m_lblText.setFont(m_hud->fontGreen());
    m_lblText.setText(text);
}


} // namespace hud
