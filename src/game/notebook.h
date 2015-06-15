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

#ifndef GAME_NOTEBOOK_H
#define GAME_NOTEBOOK_H

#include "gfx/font.h"
#include "txt/stringtable.h"
#include "ui/button.h"
#include "ui/list.h"

namespace game {

class Notebook : public ui::Label
{
public:
    Notebook(std::function<void()> funcClose);

private:
    void showMissions();
    void hideMissions();
    void showOptions();
    void hideOptions();
    void showMovies();
    void hideMovies();
    void showMoviePlayer();
    void hideMoviePlayer();
    void showMap();
    void hideMap();
    void updateMovies();

private:
    void createLabel(ui::Widget *parent, txt::String text, float posY);
    void createButton(std::function<void()> funcClick, ui::Widget *parent, txt::String text, float posY);
    void setupLabel(ui::Label &label, txt::String text, float posY);
    void setupButton(ui::Button &button, txt::String text, float posY);

private:
    bool mousePressEvent(const QPoint &pos, Qt::MouseButton button);

private:
    gfx::Font m_fontGreen;
    gfx::Font m_fontYellow;
    gfx::Texture m_background;
    ui::Label m_lblBackground;
    ui::Label m_lblMain;
    ui::Label m_lblMissions;
    ui::Label m_lblOptions;
    ui::Label m_lblMovies;
    ui::Label m_lblMoviePlayer;
    ui::Label m_lblMap;
    ui::Button m_btnMoviesAutopilot;
    ui::Button m_btnMoviesApproach;
    ui::Button m_btnMoviesHarbour;
    ui::Button m_btnMoviePlayerHide;
    ui::Button m_btnMissionsBack;
    ui::List m_listMissions;
    std::list<ui::Label> m_labels;
    std::list<ui::Button> m_buttons;
    std::list<ui::Button> m_movieButtons;
    std::function<void()> m_funcClose;
};

} // namespace game

#endif // GAME_NOTEBOOK_H
