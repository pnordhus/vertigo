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
    gfx::Texture m_flip;
    ui::Button *m_btnFlip;
    bool m_flipped;
};


} // namespace game


#endif // GAME_DEPOT_H