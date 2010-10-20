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

    const QMap<QString, Station> &stations = Chapter::get()->stations();

    int y = 30;
    QMapIterator<QString, Station> it(stations);
    while (it.hasNext()) {
        it.next();

        if (it.key() == Chapter::get()->currentStation())
            continue;

        ui::Button *button = new ui::Button(lblMap);
        button->setFont(font);
        button->setText(it.value().name());
        button->setPosition(0, y);
        button->setWidth(180);
        button->setAlignment(ui::Label::AlignHCenter);
        button->setProperty("name", it.key());
        connect(button, SIGNAL(clicked()), SLOT(select()));

        y += font.height() + 4;
    }

    y += 8;

    ui::Button *button = new ui::Button(lblMap);
    button->setFont(font);
    button->setText(txt::StringTable::get(txt::Departure_Abort));
    button->setPosition(0, y);
    button->setWidth(180);
    button->setAlignment(ui::Label::AlignHCenter);
    connect(button, SIGNAL(clicked()), SIGNAL(close()));
}


bool Departure::mousePressEvent(const QPoint &pos, Qt::MouseButton button)
{

    if (button == Qt::RightButton) {
        emit close();
        return true;
    }

    return false;
}


void Departure::select()
{
    Chapter::get()->setStation(sender()->property("name").toString());
}


} // namespace game
