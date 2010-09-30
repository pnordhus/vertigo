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


Label::Label()
{

}


Label::Label(const gfx::Font &font)
{
    setFont(font);
}


void Label::setFont(const gfx::Font &font)
{
    m_font = font;
}


void Label::setPosition(float x, float y)
{
    m_position = QPointF(x, y);
}


void Label::setSize(float w, float h)
{
    m_size = QSizeF(w, h);
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
        m_rect = m_texture.draw(m_position);
    else
        m_rect = m_font.draw(m_text, m_position, m_size);
}


} // namespace ui
