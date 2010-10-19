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

#include "menu.h"
#include <QMouseEvent>


namespace game {


Menu::Menu() :
    m_rootWidget(NULL)
{

}


void Menu::draw()
{
    setupOrthographicMatrix(640, 480);

    if (m_rootWidget)
        m_rootWidget->doDraw();
}


void Menu::setRootWidget(ui::Widget *widget)
{
    m_rootWidget = widget;
}


void Menu::mousePressEvent(QMouseEvent *event)
{
    if (m_rootWidget)
        m_rootWidget->doMousePressEvent(screenToImage(event->pos()).toPoint(), event->button());
}


void Menu::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_rootWidget)
        m_rootWidget->doMouseReleaseEvent(screenToImage(event->pos()).toPoint(), event->button());
}


void Menu::mouseMoveEvent(QMouseEvent *event)
{
    if (m_rootWidget)
        m_rootWidget->doMouseMoveEvent(screenToImage(event->pos()).toPoint());
}


} // namespace game
