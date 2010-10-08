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

#ifndef GAME_MAINMENU_H
#define GAME_MAINMENU_H


#include "renderer.h"
#include "gfx/texture.h"
#include "sfx/sound.h"
#include "ui/label.h"


namespace game {


class MainMenu : public Renderer
{
    Q_OBJECT

private:
    enum State { Invalid, Presents, Title };

public:
    MainMenu();

public:
    void draw();

signals:
    void startGame();

private:
    void activate();
    void deactivate();
    void changeState(State state);
    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);

private:
    State m_state;
    ui::Label m_presents;
    ui::Label m_title;
    ui::Widget* m_rootWidget;
    sfx::Sound m_backgroundSound;
};


} // namespace game


#endif // GAME_MAINMENU_H
