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

#include "button.h"


namespace ui {


Button::Button() :
    m_pressed(false)
{

}


void Button::draw()
{
    if (m_text.isEmpty()) {
        m_rect = m_texture.draw(m_position);
    } else {
        QPointF offset;
        if (m_pressed)
            offset += QPointF(1, 1);

        m_rect = m_font.draw(m_text, m_position + offset, m_size);
        m_rect.setTopLeft(m_rect.topLeft() - offset);
    }
}


void Button::mousePressEvent(const QPointF &pos, Qt::MouseButton button)
{
    m_pressed = true;
}


void Button::mouseReleaseEvent(const QPointF &pos, Qt::MouseButton button)
{
    if (m_pressed && m_rect.contains(pos))
        emit clicked();
    m_pressed = false;
}


} // namespace ui
