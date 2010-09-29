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

#include "mainmenu.h"
#include "gfx/image.h"
#include <QApplication>
#include <QKeyEvent>
#include <GL/gl.h>


namespace game {


MainMenu::MainMenu() :
    m_state(Invalid)
{
    m_presents.fromImage(gfx::Image::loadPCX("gfx:pic/title/present.pcx"));
    m_title.fromImage(gfx::Image::loadPCX("gfx:pic/title/title.pcx"));

    changeState(Presents);
}


void MainMenu::draw()
{
    setupOrthographicMatrix(640, 480);

    switch (m_state) {
    case Invalid:
        break;

    case Presents:
        m_presents.draw();
        break;

    case Title:
        m_title.draw();
        break;
    }
}


void MainMenu::changeState(State state)
{
    switch (state) {
    case Presents:
        qApp->setOverrideCursor(Qt::BlankCursor);
        m_state = state;
        break;

    case Title:
        qApp->restoreOverrideCursor();
        m_state = state;
        break;

    default:
        qFatal("Cannot change state!");
    }
}


void MainMenu::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape)
        qApp->quit();
}


void MainMenu::mousePressEvent(QMouseEvent *event)
{
    switch (m_state) {
    case Presents:
        changeState(Title);
        break;

    default:
        ; // nothing to do
    }
}


} // namespace game
