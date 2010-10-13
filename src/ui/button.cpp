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


Button::Button(Widget *parent) :
    Label(parent),
    m_pressed(false),
    m_offset(1.0f)
{

}


void Button::setOffset(float offset)
{
    m_offset = offset;
}


void Button::draw()
{
    if (m_text.isEmpty()) {
        m_drawRect = m_texture.draw();
    } else {
        QPointF offset;
        if (m_pressed)
            offset += QPointF(m_offset, m_offset);

        m_drawRect = m_font.draw(m_text, offset, size(), m_alignment & AlignHCenter, m_alignment & AlignVCenter);
        m_drawRect.setTopLeft(m_drawRect.topLeft() - offset);
    }
}


bool Button::mousePressEvent(const QPointF &pos, Qt::MouseButton button)
{
    if (button == Qt::LeftButton && mapToGlobal(m_drawRect).contains(pos)) {
        m_pressed = true;
        return true;
    }

    return false;
}


void Button::mouseReleaseEvent(const QPointF &pos, Qt::MouseButton button)
{
    if (button == Qt::LeftButton) {
        if (m_pressed && mapToGlobal(m_drawRect).contains(pos))
            emit clicked();
        m_pressed = false;
    }
}


} // namespace ui
