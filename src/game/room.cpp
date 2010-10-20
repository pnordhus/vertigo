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
    m_backgroundImage = gfx::Image::loadPCX("gfx:pic/room/" + background + ".pcx").toRgb565();
    m_miniMovie.setColorTable(m_backgroundImage.colorTable());
    m_background.fromImage(m_backgroundImage);
    m_backgroundSound.load("sfx:snd/room/" + backgroundSound + ".pcm");

    file.endGroup();

    setupFrame(m_background.size() + QSize(18, 24), title, true);

    ui::Label *label = new ui::Label(this);
    label->setPosition(9, 15);
    label->setTexture(m_background);

    m_miniMovie.load(file);
    m_miniMovie.start();
    connect(&m_miniMovie, SIGNAL(videoFinished()), SIGNAL(showDeparture()));
    connect(&m_miniMovie, SIGNAL(videoFinished()), SLOT(enable()));

    m_backgroundSound.playLoop();

    if (file.contains("Dock/Name")) {
        file.beginGroup("Dock");

        const gfx::ColorTable colorTable("gfx:pal/gui/border.pal");
        gfx::Font fontSmall("gfx:fnt/dpsmall.fnt", colorTable);

        const QString name = file.value("Name").toString();
        const QString arrow = file.value("Arrow").toString();
        const QString sound = file.value("Sound").toString();
        const int rate = file.value("Rate", 0).toInt();
        const int width = fontSmall.width(name);

        m_dockSound.load("sfx:snd/room/" + sound + ".pcm", rate);

        const bool left = arrow.endsWith("Left");
        const bool top = arrow.startsWith("Top");

        char arrowX = 'l';
        char arrowY = 't';
        QPoint offset(9, 2);
        QPoint offsetArrow;

        if (left) {
            offsetArrow.setX(-10);
            offset.setX(-1 - width);
            arrowX = 'r';
        }

        if (top) {
            offsetArrow.setY(-10);
            offset.setY(-2);
            arrowY = 'b';
        }

        offset += offsetArrow;

        ui::Label *lblArrow = new ui::Label(this);
        lblArrow->setTexture(gfx::Image::load(QString("gfx:img/desktop/gui/arr%1%2sma.img").arg(arrowY).arg(arrowX), colorTable));
        lblArrow->setPosition(offsetArrow.x() + file.value("X").toInt(), offsetArrow.y() + file.value("Y").toInt());

        ui::Button *button = new ui::Button(this);
        button->setOffset(0);
        button->setFont(fontSmall);
        button->setText(name);
        button->setPosition(offset.x() + file.value("X").toInt(), offset.y() + file.value("Y").toInt());
        button->setProperty("name", name);
        button->setProperty("room", file.value("Room"));
        connect(button, SIGNAL(clicked()), SLOT(showDock()));

        file.endGroup();
    }
}


void Room::restart()
{
    m_background.fromImage(m_backgroundImage);
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


void Room::showDock()
{
    emit hideCursor();
    disable();
    m_dockSound.play();
    m_miniMovie.playOneshot();
}


} // namespace game
