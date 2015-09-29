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
    m_offset(1)
{

}

Button::Button(util::event<> &&eventClicked, Widget *parent) :
    Button(parent)
{
    m_eventClicked = std::move(eventClicked);
}


void Button::setOffset(int offset)
{
    m_offset = offset;
}


void Button::setPressedTexture(const gfx::Texture &texture)
{
    m_pressedTexture = texture;
}


void Button::setDisabledTexture(const gfx::Texture &texture)
{
    m_disabledTexture = texture;
}


void Button::setDisabledFont(const gfx::Font &font)
{
    m_disabledFont = font;
}


void Button::draw()
{
    if (m_text.isEmpty()) {
        if (!isEnabled() && m_disabledTexture.isValid())
            m_drawRect = util::roundRect(m_disabledTexture.draw());
        else if (m_pressed && m_pressedTexture.isValid())
            m_drawRect = util::roundRect(m_pressedTexture.draw());
        else
            m_drawRect = util::roundRect(m_texture.draw());
    } else {
        Point offset(m_pressed ? m_offset : 0);

        if (!isEnabled() && m_disabledFont.height() != 0)
            m_drawRect = m_disabledFont.draw(m_text, Rect(offset, Size(width(), height())), m_alignment & AlignHCenter, m_alignment & AlignBottom);
        else
            m_drawRect = m_font.draw(m_text, Rect(offset, Size(width(), height())), m_alignment & AlignHCenter, m_alignment & AlignBottom);
        m_drawRect.setPos(m_drawRect.pos() - offset);
    }
}


bool Button::mousePressEvent(const QPoint &pos, Qt::MouseButton button)
{
    if (button == Qt::LeftButton && mapToGlobal(QRect(m_drawRect.x, m_drawRect.y, m_drawRect.width, m_drawRect.height)).contains(pos)) {
        m_pressed = true;
        return true;
    }

    return false;
}


void Button::mouseReleaseEvent(const QPoint &pos, Qt::MouseButton button)
{
    if (button == Qt::LeftButton) {
        if (m_pressed && mapToGlobal(QRect(m_drawRect.x, m_drawRect.y, m_drawRect.width, m_drawRect.height)).contains(pos))
            m_eventClicked();
        m_pressed = false;
    }
}


} // namespace ui
