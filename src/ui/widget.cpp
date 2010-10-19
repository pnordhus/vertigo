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

#include "widget.h"
#include <QGLContext>


namespace ui {


Widget::Widget(Widget *parent) :
    m_parent(parent),
    m_visible(true)
{
    setSize(640, 480);
    if (m_parent)
        m_parent->m_children.append(this);
}


Widget::~Widget()
{
    qDeleteAll(m_children);
    if (m_parent)
        m_parent->m_children.removeAll(this);
}


void Widget::setVisible(bool visible)
{
    m_visible = visible;
}


void Widget::hide()
{
    m_visible = false;
}


void Widget::show()
{
    m_visible = true;
}


void Widget::setPosition(int x, int y)
{
    m_rect.moveTopLeft(QPoint(x, y));
}


void Widget::setSize(const QSize &size)
{
    m_rect.setSize(size);
}


void Widget::setWidth(int w)
{
    m_rect.setWidth(w);
}


QRect Widget::mapToGlobal(QRect rect) const
{
    rect.moveTopLeft(rect.topLeft() + m_rect.topLeft());
    if (!m_parent)
        return rect;

    return m_parent->mapToGlobal(rect);
}


void Widget::doDraw()
{
    if (m_visible) {
        glPushMatrix();
        glTranslatef(m_rect.x(), m_rect.y(), 0.0f);

        draw();
        foreach (Widget *widget, m_children)
            widget->doDraw();

        glPopMatrix();
    }
}


bool Widget::doMousePressEvent(const QPoint &pos, Qt::MouseButton button)
{
    if (m_visible && pos.x() >= m_rect.x() && pos.y() >= m_rect.y()) {
        if (mousePressEvent(pos, button))
            return true;

        foreach (Widget *widget, m_children) {
            if (widget->doMousePressEvent(pos, button))
                return true;
        }
    }

    return false;
}


void Widget::doMouseReleaseEvent(const QPoint &pos, Qt::MouseButton button)
{
    if (m_visible) {
        mouseReleaseEvent(pos, button);
        foreach (Widget *widget, m_children)
            widget->doMouseReleaseEvent(pos, button);
    }
}


void Widget::doMouseMoveEvent(const QPoint &pos)
{
    if (m_visible) {
        mouseMoveEvent(pos);
        foreach (Widget *widget, m_children)
            widget->doMouseMoveEvent(pos);
    }
}


} // namespace ui
