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
#include "window.h"
#include "gfx/colortable.h"
#include "gfx/image.h"
#include <QAction>
#include <QSettings>


namespace game {


Window::Window() :
    m_renderer(NULL)
{
    QAction *actionFullScreen = new QAction(this);
    actionFullScreen->setShortcuts(QList<QKeySequence>() << (Qt::CTRL + Qt::Key_F) << (Qt::ALT + Qt::Key_Return) << (Qt::ALT + Qt::Key_Enter));
    connect(actionFullScreen, SIGNAL(triggered()), SLOT(toggleFullScreen()));
    addAction(actionFullScreen);

    const gfx::ColorTable colorTable("gfx:pal/gui/cursor.pal");
    m_cursor = gfx::Image::loadCursor("gfx:img/desktop/gui/cur_norm.img", colorTable);
    setWindowTitle("Vertigo");
    setMouseTracking(true);
    makeCurrent();
    loadSettings();
}


void Window::setRenderer(Renderer *renderer)
{
    if (m_renderer) {
        m_renderer->deactivate();
        m_renderer->disconnect(this, SLOT(centerMouse()));
        m_renderer->setWindow(NULL);
    }

    m_renderer = renderer;

    if (m_renderer) {
        m_renderer->setWindow(this);
        connect(m_renderer, SIGNAL(centerMouse()), SLOT(centerMouse()));
        m_renderer->activate();
    }
}


void Window::centerMouse()
{
    QCursor::setPos(mapToGlobal(rect().center()));
}


void Window::toggleFullScreen()
{
    setWindowState(windowState() ^ Qt::WindowFullScreen);
}


void Window::initializeGL()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}


void Window::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}


void Window::paintGL()
{
    if (m_renderer) {
        if (m_renderer->isCursorVisible())
            setCursor(m_cursor);
        else
            setCursor(Qt::BlankCursor);

        m_renderer->setRect(rect());
        m_renderer->draw();
    }
}


void Window::keyPressEvent(QKeyEvent *event)
{
    if (m_renderer)
        m_renderer->keyPressEvent(event);
}


void Window::keyReleaseEvent(QKeyEvent *event)
{
    if (m_renderer)
        m_renderer->keyReleaseEvent(event);
}


void Window::mousePressEvent(QMouseEvent *event)
{
    if (m_renderer)
        m_renderer->mousePressEvent(event);
}


void Window::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_renderer)
        m_renderer->mouseReleaseEvent(event);
}


void Window::mouseMoveEvent(QMouseEvent *event)
{
    if (m_renderer)
        m_renderer->mouseMoveEvent(event);
}


void Window::closeEvent(QCloseEvent *event)
{
    saveSettings();
    QGLWidget::closeEvent(event);
}


void Window::saveSettings()
{
    QSettings settings;
    settings.setValue("MainWindow/geometry", saveGeometry());
}


void Window::loadSettings()
{
    QSettings settings;
    restoreGeometry(settings.value("MainWindow/geometry").toByteArray());
}


} // namespace game
