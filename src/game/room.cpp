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

#include "room.h"
#include "gfx/colortable.h"
#include "gfx/font.h"
#include "gfx/image.h"
#include "ui/button.h"


namespace game {


Room::Room(const QString &title, const QString &name) :
    m_miniMovie("gfx:mvi/room"),
    m_name(name)
{
    txt::DesFile file("dat:world/" + name + ".des");
    file.beginGroup("Room");

    const QString background = file.value("BackGround").toString();
    const QString backgroundSound = file.value("Sound").toString();
    QImage image = gfx::Image::loadPCX("gfx:pic/room/" + background + ".pcx").toRgb565();
    m_miniMovie.setColorTable(image.colorTable());
    m_background.fromImage(image);
    m_backgroundSound.load("sfx:snd/room/" + backgroundSound + ".pcm");

    file.endGroup();

    setupFrame(m_background.size() + QSize(18, 24), title, true);

    ui::Label *label = new ui::Label(this);
    label->setPosition(9, 15);
    label->setTexture(m_background);

    m_miniMovie.load(file);
    m_miniMovie.start();
    m_backgroundSound.playLoop();
}


void Room::draw()
{
    m_miniMovie.update(m_background);
    ui::Label::draw();
}


bool Room::mousePressEvent(const QPoint &pos, Qt::MouseButton button)
{
    if (button == Qt::RightButton) {
        if (mapToGlobal(m_drawRect).contains(pos)) {
            emit close();
            return true;
        }
    }

    return false;
}


} // namespace game
