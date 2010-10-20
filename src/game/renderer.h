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

#ifndef GAME_RENDERER_H
#define GAME_RENDERER_H


#include <QMatrix4x4>
#include <QRect>


class QKeyEvent;
class QMouseEvent;


namespace game {


class Renderer : public QObject
{
    Q_OBJECT

public:
    Renderer();

public:
    virtual void draw() = 0;
    virtual void activate() = 0;
    virtual void deactivate() = 0;

public slots:
    void setCursorVisible(bool visible);
    void showCursor();
    void hideCursor();

public:
    virtual bool isCursorVisible() const { return m_cursorVisible; }
    void setRect(const QRect &rect);
    QRect rect() const { return m_rect; }
    int width() const { return m_rect.width(); }
    int height() const { return m_rect.height(); }
    QMatrix4x4 projection() const { return m_projection; }
    QPointF screenToImage(const QPointF &pos);

public:
    virtual void keyPressEvent(QKeyEvent *) {}
    virtual void keyReleaseEvent(QKeyEvent *) {}
    virtual void mousePressEvent(QMouseEvent *) = 0;
    virtual void mouseReleaseEvent(QMouseEvent *) {}
    virtual void mouseMoveEvent(QMouseEvent *) {}

protected:
    void setupOrthographicMatrix(float w, float h);

private:
    bool m_cursorVisible;
    QRect m_rect;
    QMatrix4x4 m_projection;
};


} // namespace game


#endif // GAME_RENDERER_H
