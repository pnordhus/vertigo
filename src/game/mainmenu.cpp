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
#include "ui/button.h"
#include "util/colortable.h"
#include <QApplication>
#include <QKeyEvent>
#include <GL/gl.h>


namespace game {


MainMenu::MainMenu() :
    m_state(Invalid)
{
    util::ColorTable colorTable("gfx:pal/gui/border.pal");

    gfx::Texture texBar(gfx::Image::load("gfx:img/title/horline.img", colorTable));
    gfx::Font fontSmall("gfx:fnt/dpsmall.fnt", colorTable);
    gfx::Font fontMedium("gfx:fnt/dpmedium.fnt", colorTable);
    gfx::Font fontLarge("gfx:fnt/dplarge.fnt", colorTable);

    m_presents.setTexture(gfx::Image::loadPCX("gfx:pic/title/present.pcx"));
    m_title.setTexture(gfx::Image::loadPCX("gfx:pic/title/title.pcx"));

    ui::Label *label;

    label = new ui::Label;
    label->setFont(fontSmall);
    label->setPosition(2, 2);
    label->setText("Vertigo 0.1");
    m_title.addChild(label);

    label = new ui::Label;
    label->setFont(fontMedium);
    label->setPosition(145, 256 - fontMedium.height() - 2);
    label->setText("Hauptmenu");
    m_title.addChild(label);

    label = new ui::Label;
    label->setTexture(texBar);
    label->setPosition(140, 256);
    m_title.addChild(label);

    label = new ui::Label;
    label->setTexture(texBar);
    label->setPosition(140, 448);
    m_title.addChild(label);

    ui::Button *button;
    button = new ui::Button;
    button->setFont(fontLarge);
    button->setPosition(0, 280);
    button->setSize(640, -1);
    button->setText("Neues Spiel");
    connect(button, SIGNAL(clicked()), SIGNAL(startGame()));
    m_title.addChild(button);

    button = new ui::Button;
    button->setFont(fontLarge);
    button->setPosition(0, 300);
    button->setSize(640, -1);
    button->setText("Spielstand laden");
    m_title.addChild(button);

    button = new ui::Button;
    button->setFont(fontLarge);
    button->setPosition(0, 410);
    button->setSize(640, -1);
    button->setText("Spiel beenden");
    connect(button, SIGNAL(clicked()), qApp, SLOT(quit()));
    m_title.addChild(button);

    changeState(Presents);
}


void MainMenu::draw()
{
    setupOrthographicMatrix(640, 480);
    m_rootWidget->doDraw();
}


void MainMenu::changeState(State state)
{
    switch (state) {
    case Presents:
        m_rootWidget = &m_presents;
        m_state = state;
        break;

    case Title:
        m_rootWidget = &m_title;
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

    switch (m_state) {
    case Presents:
        changeState(Title);
        break;

    default:
        ; // nothing to do
    }
}


void MainMenu::mousePressEvent(QMouseEvent *event)
{
    m_rootWidget->doMousePressEvent(screenToImage(event->posF()), event->button());
    switch (m_state) {
    case Presents:
        changeState(Title);
        break;

    default:
        ; // nothing to do
    }
}


void MainMenu::mouseReleaseEvent(QMouseEvent *event)
{
    m_rootWidget->doMouseReleaseEvent(screenToImage(event->posF()), event->button());
}


void MainMenu::mouseMoveEvent(QMouseEvent *event)
{
    m_rootWidget->doMouseMoveEvent(screenToImage(event->posF()));
}


} // namespace game
