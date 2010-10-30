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

#include "list.h"
#include "gfx/font.h"


namespace ui {


List::List(Widget *parent) :
    Widget(parent)
{

}


void List::setText(const QStringList &text)
{
    m_text = text;
}


void List::setFont(const gfx::Font &font)
{
    m_font = font;
}


void List::draw()
{
    int y = 0;
    foreach (const QString &line, m_text) {
        QRect rect = m_font.draw(line, QPoint(0, y), QSize(width(), -1), true, false);
        y += rect.height();
    }
}


} // namespace ui
