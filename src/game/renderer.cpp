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
#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>


namespace game {

void getMatrix(GLenum param, float *data)
{
    glGetFloatv(param, data);
}

void getMatrix(GLenum param, double *data)
{
    glGetDoublev(param, data);
}

Renderer::Renderer() :
    m_cursorVisible(true),
    m_window(NULL)
{

}


void Renderer::setWindow(QGLWidget *window)
{
    m_window = window;
}


void Renderer::showCursor()
{
    m_cursorVisible = true;
}


void Renderer::hideCursor()
{
    m_cursorVisible = false;
}


void Renderer::setRect(const QRect &rect)
{
    m_rect = rect;

    setupOrthographicMatrix(640, 480);
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
    }
    else {
        scale = ratioH;
        left = ((width() / scale) - w) / 2;
    }

    m_rectOrtho = QRectF(-left, -top, w + 2*left, h + 2*top);

    m_projectionMatrix = glm::ortho(m_rectOrtho.left(), m_rectOrtho.right(), m_rectOrtho.bottom(), m_rectOrtho.top());
    m_projectionMatrixInverted = glm::inverse(m_projectionMatrix);
}


void Renderer::setupGL(bool clear)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    if (clear)
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glDisable(GL_FOG);
    glDisable(GL_CULL_FACE);
    glFrontFace(GL_CCW);

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(glm::value_ptr(m_projectionMatrix));

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


QPointF Renderer::screenToImage(const QPointF &pos)
{
    Vector4D p = m_projectionMatrixInverted * Vector4D(pos.x() / width() * 2.0f - 1.0f, 1.0f - pos.y() / height() * 2.0f, 0, 1);
    return QPointF(p.x, p.y);
}


} // namespace game
