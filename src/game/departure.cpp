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

#include "chapter.h"
#include "departure.h"
#include "gfx/image.h"
#include "txt/stringtable.h"


namespace game {


Departure::Departure()
{
    const gfx::ColorTable colorTable("gfx:pal/notebook/notebook.pal");
    gfx::Font font("gfx:fnt/nfont1b.fnt", colorTable, true);

    QImage image(640, 480, QImage::Format_Indexed8);
    image.fill(0);
    image.setColorTable(QVector<QRgb>() << qRgba(0, 0, 0, 128));
    setTexture(image);

    ui::Label *label;
    label = new ui::Label(this);
    label->setTexture(gfx::Image::loadPCX("gfx:pic/apilot/apmon.pcx"));
    label->setPosition(186, 74);

    ui::Label *lblMap = new ui::Label(label);
    lblMap->setTexture(gfx::Image::loadPCX(QString("gfx:pic/apilot/%1.pcx").arg(Chapter::get()->area()->map())));
    lblMap->setPosition(46, 41);

    label = new ui::Label(lblMap);
    label->setFont(font);
    label->setText(txt::StringTable::get(txt::Departure_Title));
    label->setPosition(0, 10);
    label->setWidth(180);
    label->setAlignment(ui::Label::AlignHCenter);

    int y = 30;
    foreach (const Station &station, Chapter::get()->stations()) {
        if (station.index() == Chapter::get()->currentStation())
            continue;
        if (!station.isEnabled())
            continue;

        const int index = station.index();
        ui::Button *button = new ui::Button([this, index]() { Chapter::get()->setStation(index); }, lblMap);
        button->setFont(font);
        button->setText(station.name());
        button->setPosition(0, y);
        button->setWidth(180);
        button->setAlignment(ui::Label::AlignHCenter);

        y += font.height() + 4;
    }

    y += 4;

    foreach (Mission* mission, Chapter::get()->missions()) {
        if (mission->station() == -1) {
            const QString shortName = mission->shortName();
            ui::Button *button = new ui::Button([this, shortName]() { Chapter::get()->startMission(shortName); }, lblMap);
            button->setFont(font);
            button->setText(mission->name());
            button->setPosition(0, y);
            button->setWidth(180);
            button->setAlignment(ui::Label::AlignHCenter);

            y += font.height() + 4;
        }
    }

    y += 4;

    ui::Button *button = new ui::Button([this]() { emit close(); }, lblMap);
    button->setFont(font);
    button->setText(txt::StringTable::get(txt::Departure_Abort));
    button->setPosition(0, y);
    button->setWidth(180);
    button->setAlignment(ui::Label::AlignHCenter);
}


bool Departure::mousePressEvent(const QPoint &pos, Qt::MouseButton button)
{

    if (button == Qt::RightButton) {
        emit close();
        return true;
    }

    return false;
}

} // namespace game
