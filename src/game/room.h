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

#ifndef GAME_ROOM_H
#define GAME_ROOM_H


#include "minimovie.h"
#include "sfx/sound.h"
#include "ui/arrow.h"
#include "ui/frame.h"


namespace game {


class Dialog;


class Room : public ui::Frame
{
public:
    Room(int index, const QString &title, const QString &name,
         std::function<void()> funcClose,
         std::function<void(Dialog*)> funcStartDialog,
         std::function<void(Dialog*)> funcStartEnCom,
         std::function<void()> funcShowDeparture,
         std::function<void()> funcShowDepot,
         std::function<void()> funcHideCursor
    );
    ~Room();

public:
    const QString& name() const { return m_name; }
    void restart();

protected:
    void draw();
    bool mousePressEvent(const QPoint &pos, Qt::MouseButton button);

private:
    void showDock();
    void startDialog(int dialogId);
    void miniMovieFinished();

private:
    struct Person
    {
        template <typename... Args>
        Person(Args... args) : arrow(std::forward<Args>(args)...), female(false) {}
        ui::Arrow arrow;
        bool female;
    };

    int m_index;
    MiniMovie m_miniMovie;
    QString m_name;
    QImage m_backgroundImage;
    ui::Label *m_backgroundLabel;
    gfx::Texture m_background;
    sfx::Sound m_backgroundSound;
    sfx::Sound m_dockSound;
    QMap<int, Person*> m_persons;
    Person* m_dockMan;
    QString m_dockManName;
    std::function<void(Dialog*)> m_funcStartDialog;
    std::function<void(Dialog*)> m_funcStartEnCom;
    std::function<void()> m_funcShowDeparture;
    std::function<void()> m_funcShowDepot;
    std::function<void()> m_funcHideCursor;
    std::list<ui::Arrow> m_arrows;
};


} // namespace game


#endif // GAME_ROOM_H
