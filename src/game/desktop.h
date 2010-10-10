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

#ifndef GAME_DESKTOP_H
#define GAME_DESKTOP_H


#include "menu.h"
#include "notebook.h"
#include "gfx/texture.h"
#include "gfx/video.h"
#include "sfx/sound.h"
#include "ui/button.h"


namespace game {


class Desktop : public Menu
{
    Q_OBJECT

public:
    Desktop(const QString &name);
    ~Desktop();

public:
    void draw();

private slots:
    void showNotebook();
    void hideNotebook();

private:
    void activate();
    void deactivate();

private:
    struct Video
    {
        gfx::Video video;
        int x;
        int y;
        int rndMax;
        int time;
    };

    gfx::Texture m_background;
    sfx::Sound m_backgroundSound;
    sfx::Sound m_nameSound;
    sfx::Sound m_notebookSound;
    ui::Label m_lblBackground;
    ui::Button *m_btnNotebook;
    Notebook m_notebook;
    ui::Button *m_btnQuit;
    QList<Video*> m_videos;
    QTime m_time;
};


} // namespace game


#endif // GAME_DESKTOP_H
