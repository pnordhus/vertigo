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

#include "encom.h"
#include "gfx/colortable.h"
#include "gfx/image.h"
#include "txt/stringtable.h"


namespace game {


EnCom::EnCom(Dialog *dialog) :
    m_dialog(dialog)
{
    const gfx::ColorTable colorTableBorder("gfx:pal/gui/border.pal");
    const gfx::ColorTable colorTable("gfx:pal/notebook/notebook.pal");
    gfx::Texture noteback(gfx::Image::load("gfx:img/desktop/notebook/noteback.img", colorTable));
    gfx::Font font("gfx:fnt/dpsmamon.fnt", 0xffb0b800, 0xffb0b800, true);

    QImage image(640, 480, QImage::Format_Indexed8);
    image.fill(0);
    image.setColorTable(QVector<QRgb>() << qRgba(0, 0, 0, 128));
    setTexture(image);

    ui::Label *labelBackground = new ui::Label(this);
    labelBackground->setPosition(48, 48);
    labelBackground->setTexture(gfx::Image::loadPCX("gfx:pic/notebook/notebook.pcx"));

    ui::Label *labelMain = new ui::Label(labelBackground);
    labelMain->setPosition(162, 73);
    labelMain->setTexture(noteback);

    QImage line = gfx::Image::load("gfx:img/desktop/gui/horline2.img", colorTableBorder).copy(0, 0, 304, 3);

    ui::Label *label;

    label = new ui::Label(labelMain);
    label->setPosition(0, 24);
    label->setWidth(304);
    label->setAlignment(ui::Label::AlignHCenter);
    label->setFont(font);
    label->setText(txt::StringTable::get(txt::EnCom_Via));

    label = new ui::Label(labelMain);
    label->setPosition(0, 36);
    label->setWidth(304);
    label->setAlignment(ui::Label::AlignHCenter);
    label->setFont(font);
    label->setText(QString("%1 %2").arg(txt::StringTable::get(txt::EnCom_Person), m_dialog->name()));

    label = new ui::Label(labelMain);
    label->setPosition(0, 56);
    label->setTexture(line);

    label = new ui::Label(labelMain);
    label->setPosition(0, 111);
    label->setTexture(line);

    m_dialog->setParentWidget(labelMain);
    m_dialog->setPosition(16, 68);
    m_dialog->setWidth(304 - 32);
    connect(m_dialog, SIGNAL(close()), SIGNAL(close()));
}


} // namespace game
