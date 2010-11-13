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

#include "renderer.h"
#include <QGLContext>


namespace game {


Renderer::Renderer() :
    m_cursorVisible(true),
    m_window(NULL)
{

}


void Renderer::setWindow(QWidget *window)
{
    m_window = window;
}


void Renderer::setRect(const QRect &rect)
{
    m_rect = rect;
}


void Renderer::setCursorVisible(bool visible)
{
    m_cursorVisible = visible;
}


void Renderer::showCursor()
{
    m_cursorVisible = true;
}


void Renderer::hideCursor()
{
    m_cursorVisible = false;
}


void Renderer::setupOrthographicMatrix(float w, float h)
{
    const float ratioW = width() / w;
    const float ratioH = height() / h;

    float left = 0.0f;
    float top = 0.0f;
    float scale;
    if (ratioW < ratioH) {
        scale = ratioW;
        top = ((height() / scale) - h) / 2;
    } else {
        scale = ratioH;
        left = ((width() / scale) - w) / 2;
    }

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glDisable(GL_FOG);
    glDisable(GL_CULL_FACE);
    glFrontFace(GL_CCW);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-left, w + left, h + top, -top, -1, 1);

    glGetDoublev(GL_PROJECTION_MATRIX, m_projection.data());
    m_projection.optimize();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


QPointF Renderer::screenToImage(const QPointF &pos)
{
    QPointF p;
    p.setX(pos.x() / width() * 2.0f - 1.0f);
    p.setY(1.0f - pos.y() / height() * 2.0f);
    return projection().inverted() * p;
}


} // namespace game
