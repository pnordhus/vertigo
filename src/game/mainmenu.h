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
#include "ui/button.h"
#include <QTime>

namespace game {

class MainMenu : public Menu
{
private:
    enum State { Invalid, Presents, Title };

public:
    MainMenu(bool skipToTitle, std::function<void(QString)> funcStartGame, std::function<void(QString)> funcLoadGame, std::function<void()> funcQuit);

private:
    void showMain();
    void showNew();
    void showLoad();

    void draw();
    void activate();
    void deactivate();
    void changeState(State state);
    void mousePressEvent(QMouseEvent *);
    void keyPressEvent(QKeyEvent *);

private:
    State m_state;
    ui::Label m_presents;
    ui::Label m_title;
    ui::Label m_lblVersion;
    ui::Label m_lblBar1;
    ui::Label m_lblBar2;
    ui::Label m_lblMain;
    ui::Label m_lblMainTitle;
    ui::Button m_btnMainNew;
    ui::Button m_btnMainLoad;
    ui::Button m_btnMainQuit;
    ui::Label m_lblNew;
    ui::Label m_lblNewTitle;
    ui::Button m_btnNewBack;
    ui::Label m_lblLoad;
    ui::Label m_lblLoadTitle;
    ui::Button m_btnLoadBack;
    std::list<ui::Label> m_lblLoadSave;
    std::list<ui::Button> m_btnLoadSave;
    sfx::Sound m_backgroundSound;
    QString m_name;
    bool m_cursor;
    QTime m_time;
    std::function<void(QString)> m_funcStartGame;
    std::function<void(QString)> m_funcLoadGame;
};

} // namespace game

#endif // GAME_MAINMENU_H
