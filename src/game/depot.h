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


namespace game {


class Depot : public ui::Frame
{
    Q_OBJECT

private:
    enum State { Flipping, Loading, Ready, Scrolling, Repair };
    enum LoadingState { Arrows, List1, List2 };

public:
    Depot();
    ~Depot();

private slots:
    void flip();

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
    ui::Button *m_btnFlip;
    ui::Button *m_btnRepair;
    ui::Button *m_btnLeft1;
    ui::Button *m_btnRight1;
    ui::Button *m_btnLeft2;
    ui::Button *m_btnRight2;
    ui::Button *m_btnBuy;
    ui::Button *m_btnSell;
    ui::Button *m_btnInfo;

    ui::Label *m_lblFlip;
    ui::Label *m_lblLeft1;
    ui::Label *m_lblRight1;
    ui::Label *m_lblLeft2;
    ui::Label *m_lblRight2;
    ui::Label *m_lblBuy;
    ui::Label *m_lblSell;

    QTime m_time;
    State m_state;
    int m_side;
    LoadingState m_loadingState;
    int m_loadingItem;
};


} // namespace game


#endif // GAME_DEPOT_H
