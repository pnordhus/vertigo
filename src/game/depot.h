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


#include "minimovie.h"
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

public:
    void restart();

protected:
    void draw();
    bool mousePressEvent(const QPoint &pos, Qt::MouseButton button);

private:
    QImage m_backgroundImage;
    ui::Label *m_backgroundLabel;
    gfx::Texture m_background;
    sfx::Sound m_backgroundSound;
};


} // namespace game


#endif // GAME_DEPOT_H
