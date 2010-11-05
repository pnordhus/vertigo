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

#ifndef GAME_DEPOT_H
#define GAME_DEPOT_H


#include "gfx/video.h"
#include "sfx/sound.h"
#include "ui/arrow.h"
#include "ui/frame.h"
#include "ui/list.h"
#include "ui/itemlist.h"


namespace game {

class Boat;

class Depot : public ui::Frame
{
    Q_OBJECT

private:
    enum State { Flipping, Loading, Ready, Repair };
    enum LoadingState { Arrows, List1, List2 };

public:
    Depot();
    ~Depot();

private slots:
    void flip();
    void loadMounting(int index);
    void itemListClicked1(int index);
    void itemListClicked2(int index);
    void toggleInfo();
    void buy();
    void sell();

private:
    void updateInfo();
    void updateCredits();
    void updateButtons();

protected:
    void draw();
    bool mousePressEvent(const QPoint &pos, Qt::MouseButton button);

private:
    ui::Label *m_backgroundLabel;
    sfx::Sound m_backgroundSound;
    gfx::Video m_videoFlip1;
    gfx::Video m_videoFlip2;
    gfx::Texture m_boatTexture;

    ui::Label *m_boatLabel;
    ui::Label *m_boatName;
    ui::Label *m_credits;
    ui::Button *m_btnFlip;
    ui::Button *m_btnRepair;
    ui::Button *m_btnBuy;
    ui::Button *m_btnSell;
    ui::Button *m_btnInfo;

    ui::Label *m_lblInfo;
    ui::Label *m_lblItemName;
    ui::Label *m_lblItemPrice;
    ui::List *m_lblItemText;
    ui::Label *m_lblItemVideo;
    gfx::Video m_videoItem;
    gfx::Texture m_itemTexture;

    ui::ItemList *m_itemList1;
    ui::ItemList *m_itemList2;

    Boat *m_boat;
    int m_side;
    int m_mounting;
    QList<ui::Arrow*> m_mountingArrows;
    QList<int> m_list1;
    QList<int> m_list2;
    int m_selectedList;
    int m_selectedItem;

    QTime m_time;
    State m_state;
    LoadingState m_loadingState;
    int m_loadingItem;
};


} // namespace game


#endif // GAME_DEPOT_H
