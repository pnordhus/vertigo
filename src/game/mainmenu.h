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


#include "menu.h"
#include "gfx/texture.h"
#include "sfx/sound.h"
#include "ui/label.h"


namespace game {


class MainMenu : public Menu
{
    Q_OBJECT

private:
    enum State { Invalid, Presents, Title };

public:
    MainMenu(bool skipToTitle);

signals:
    void startGame();
    void loadGame(int);
    void quit();

private slots:
    void showLoad();
    void hideLoad();
    void loadGame();

private:
    void activate();
    void deactivate();
    void changeState(State state);
    void mousePressEvent(QMouseEvent *);

private:
    State m_state;
    ui::Label m_presents;
    ui::Label m_title;
    ui::Label *m_lblMain;
    ui::Label *m_lblLoad;
    sfx::Sound m_backgroundSound;
};


} // namespace game


#endif // GAME_MAINMENU_H
