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

#include "label.h"


namespace ui {


Label::Label(Widget *parent, bool visible) :
    Widget(parent, visible),
    m_alignment(AlignNone)
{

}


Label::Label(const gfx::Font &font, Widget *parent) :
    Widget(parent),
    m_alignment(AlignNone)
{
    setFont(font);
}


void Label::setAlignment(Alignment alignment)
{
    m_alignment = alignment;
}


void Label::setFont(const gfx::Font &font)
{
    m_font = font;
}


void Label::setText(const QString &text)
{
    m_text = text;
}


void Label::setTexture(const gfx::Texture &texture)
{
    m_texture = texture;
}


void Label::draw()
{
    if (m_text.isEmpty())
        m_drawRect = util::roundRect(m_texture.draw());
    else
        m_drawRect = m_font.draw(m_text, Rect(0, 0, width(), height()), m_alignment & AlignHCenter, m_alignment & AlignBottom);
}


} // namespace ui
