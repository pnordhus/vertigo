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

#include "arrow.h"
#include "gfx/colortable.h"
#include "gfx/image.h"

namespace ui {

Arrow::Arrow(const QString &dir, const QPoint &pos, bool large, std::function<void(int)> &&funcClicked, Widget *parent) :
    m_position(pos),
    m_value(0),
    m_large(large),
    m_funcClicked(std::move(funcClicked))
{
    const gfx::ColorTable colorTable("gfx:pal/gui/border.pal");

    m_left = dir.endsWith("Left");
    m_top = dir.startsWith("Top");

    char arrowX = 'l';
    char arrowY = 't';
    if (m_left)
        arrowX = 'r';
    if (m_top)
        arrowY = 'b';

    m_label = new Label(parent);
    m_label->setTexture(gfx::Image::load(QString("gfx:img/desktop/gui/arr%1%2%3.img").arg(arrowY).arg(arrowX).arg(large ? "med" : "sma"), colorTable));
    m_label->setPosition(pos);

    m_button = new Button([this]() { m_funcClicked(m_value); }, parent);
    m_button->setFont(m_large ? gfx::Font::Medium : gfx::Font::Small);
    m_button->setOffset(0);
}

Arrow::~Arrow()
{
    delete m_button;
    delete m_label;
}

void Arrow::hide()
{
    m_label->hide();
    m_button->hide();
}

void Arrow::show()
{
    m_label->show();
    m_button->show();
}

bool Arrow::isVisible()
{
    return m_label->isVisible();
}

void Arrow::setValue(int value)
{
    m_value = value;
}

void Arrow::setText(const QString &text)
{
    QPoint offset(9, 2);
    if (m_large)
        offset = QPoint(11, 3);
    QPoint offsetArrow;
    const int width = gfx::Font(m_large ? gfx::Font::Medium : gfx::Font::Small).width(text);

    if (m_top) {
        offsetArrow.setY(1 - offset.x());
        offset.setY(-offset.y());
    }

    if (m_left) {
        offsetArrow.setX(1 - offset.x());
        offset.setX(-1 - width);
    }

    m_button->setText(text);
    m_label->setPosition(m_position + offsetArrow);
    m_button->setPosition(m_position + offsetArrow + offset);
}

} // namespace ui
