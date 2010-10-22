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

#ifndef UI_WIDGET_H
#define UI_WIDGET_H


#include <QList>
#include <QObject>
#include <QRectF>


namespace ui {


class Widget : public QObject
{
    Q_OBJECT

public:
    Widget(Widget *parent = NULL);
    ~Widget();

public slots:
    void setEnabled(bool);
    void enable();
    void disable();
    void setVisible(bool);
    void hide();
    void show();

public:
    void setParentWidget(Widget *parent);
    bool isEnabled() const { return m_enabled; }
    bool isVisible() const { return m_visible; }
    void setPosition(int x, int y);
    void setPosition(const QPoint &pos);
    void setSize(int w, int h);
    void setSize(const QSize &size);
    void setWidth(int w);
    void setHeight(int h);
    const QRect& rect() const { return m_rect; }
    QSize size() const { return m_rect.size(); }
    float width() const { return m_rect.width(); }
    float height() const { return m_rect.height(); }
    QRect mapToGlobal(QRect rect) const;

    void doDraw();
    bool doMousePressEvent(const QPoint &pos, Qt::MouseButton button);
    void doMouseReleaseEvent(const QPoint &pos, Qt::MouseButton button);
    void doMouseMoveEvent(const QPoint &pos);

private:
    virtual void draw() = 0;
    virtual bool mousePressEvent(const QPoint &pos, Qt::MouseButton button) { return false; }
    virtual void mouseReleaseEvent(const QPoint &pos, Qt::MouseButton button) {}
    virtual void mouseMoveEvent(const QPoint &pos) {}

private:
    Widget *m_parent;
    bool m_enabled;
    bool m_visible;
    QList<Widget*> m_children;
    QRect m_rect;
};


inline void Widget::setSize(int w, int h)
{
    setSize(QSize(w, h));
}


} // namespace ui


#endif // UI_WIDGET_H
