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


#include "departure.h"
#include "dialogframe.h"
#include "encom.h"
#include "menu.h"
#include "minimovie.h"
#include "notebook.h"
#include "room.h"
#include "gfx/texture.h"
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
    const QString &approachMovie() const { return m_approachMovie; }

private slots:
    void showNotebook();
    void hideNotebook();
    void showRoom();
    void hideRoom();
    void showDialog(Dialog *dialog);
    void hideDialog();
    void showEnCom(Dialog *dialog);
    void hideEnCom();
    void showDeparture();
    void hideDeparture();

private:
    void activate();
    void deactivate();
    void checkEnCom();

private:
    gfx::Texture m_background;
    sfx::Sound m_backgroundSound;
    sfx::Sound m_nameSound;
    sfx::Sound m_notebookShowSound;
    sfx::Sound m_notebookShowEnComSound;
    sfx::Sound m_notebookBackgroundSound;
    sfx::Sound m_notebookHideSound;
    ui::Label m_lblBackground;
    ui::Widget *m_widgetRooms;
    ui::Button *m_btnNotebook;
    Notebook m_notebook;
    Room *m_room;
    DialogFrame *m_dialog;
    EnCom *m_enCom;
    Departure *m_departure;
    ui::Button *m_btnQuit;
    QString m_approachMovie;
    MiniMovie m_miniMovie;
};


} // namespace game


#endif // GAME_DESKTOP_H
