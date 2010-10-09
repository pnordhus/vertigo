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

#include "notebook.h"
#include "gfx/image.h"
#include "txt/stringtable.h"
#include "ui/button.h"
#include "util/colortable.h"


namespace game {


Notebook::Notebook()
{
    const util::ColorTable colorTable("gfx:pal/notebook/notebook.pal");
    gfx::Font fontNotebook("gfx:fnt/nfont1a.fnt", colorTable);
    gfx::Texture noteback(gfx::Image::load("gfx:img/desktop/notebook/noteback.img", colorTable));

    QImage image(640, 480, QImage::Format_Indexed8);
    image.fill(0);
    image.setColorTable(QVector<QRgb>() << qRgba(0, 0, 0, 128));
    setTexture(image);

    ui::Label *label;
    ui::Button *button;

    label = new ui::Label;
    label->setPosition(48, 48);
    label->setTexture(gfx::Image::loadPCX("gfx:pic/notebook/notebook.pcx"));
    addChild(label);

    label = new ui::Label;
    label->setPosition(210, 121);
    label->setTexture(noteback);
    addChild(label);

    button = new ui::Button;
    button->setFont(fontNotebook);
    button->setText(txt::StringTable::get(txt::Notebook_QuitGame));
    button->setPosition(210, 331);
    button->setSize(304, -1);
    addChild(button);
}


} // namespace game
