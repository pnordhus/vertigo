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

#ifndef GAME_VERTIGO_H
#define GAME_VERTIGO_H

#include "mainmenu.h"
#include "window.h"

#include "gfx/fontmanager.h"
#include "sfx/soundsystem.h"

namespace fight {

class Scenario;

}

namespace game {

class Chapter;
class Movie;

class Vertigo
{
public:
    Vertigo();
    ~Vertigo();

public:
    bool start(const QString &scenario);

private:
    void startGame(const QString &name);
    void loadGame(const QString &name);
    void endGame();
    void introFinished();
    void createMainMenu(bool skipToTitle);
    void createChapter();

private:
    Window m_window;
    sfx::SoundSystem m_soundSystem;
    gfx::FontManager m_fontManager;
    MainMenu m_mainMenu;
    Movie *m_intro;
    Chapter *m_chapter;
    QString m_name;
    fight::Scenario *m_scenario;
};

} // namespace game

#endif // GAME_VERTIGO_H
