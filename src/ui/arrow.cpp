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


Arrow::Arrow(const QString &dir, const QPoint &pos, bool large, Widget *parent) :
    m_position(pos),
    m_value(0)
{
    const gfx::ColorTable colorTable("gfx:pal/gui/border.pal");
    m_font.load("gfx:fnt/dpsmall.fnt", colorTable);

    m_left = dir.endsWith("Left");
    m_top = dir.startsWith("Top");

    char arrowX = 'l';
    char arrowY = 't';
    if (m_left)
        arrowX = 'r';
    if (m_top)
        arrowY = 'b';

    m_label = new Label(parent);
    m_label->setTexture(gfx::Image::load(QString("gfx:img/desktop/gui/arr%1%2sma.img").arg(arrowY).arg(arrowX), colorTable));
    m_label->setPosition(pos);

    m_button = new Button(parent);
    m_button->setFont(m_font);
    m_button->setOffset(0);
    connect(m_button, SIGNAL(clicked()), SLOT(clicked()));
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


void Arrow::setValue(int value)
{
    m_value = value;
}


void Arrow::setText(const QString &text)
{
    QPoint offset(9, 2);
    QPoint offsetArrow(9, 15);
    const int width = m_font.width(text);

    if (m_left) {
        offsetArrow.setX(1);
        offset.setX(-1 - width);
    }

    if (m_top) {
        offsetArrow.setY(7);
        offset.setY(-2);
    }

    m_button->setText(text);
    m_label->setPosition(m_position + offsetArrow);
    m_button->setPosition(m_position + offsetArrow + offset);
}


void Arrow::clicked()
{
    emit clicked(m_value);
}


} // namespace ui
