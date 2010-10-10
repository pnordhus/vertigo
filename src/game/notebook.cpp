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
#include "util/colortable.h"


namespace game {


Notebook::Notebook()
{
    const util::ColorTable colorTable("gfx:pal/notebook/notebook.pal");
    m_fontGreen.load("gfx:fnt/nfont1a.fnt", colorTable);
    m_fontYellow.load("gfx:fnt/nfont1b.fnt", colorTable);
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

    label = createLabel(txt::Notebook_Title, 30);
    addChild(label);

    label = createLabel(txt::Notebook_TitleLine, 40);
    addChild(label);

    button = createButton(txt::Notebook_Missions, 90);
    addChild(button);

    button = createButton(txt::Notebook_LoadSave, 110);
    addChild(button);

    button = createButton(txt::Notebook_Options, 130);
    addChild(button);

    button = createButton(txt::Notebook_Map, 150);
    addChild(button);

    button = createButton(txt::Notebook_Back, 170);
    connect(button, SIGNAL(clicked()), SIGNAL(close()));
    addChild(button);

    button = createButton(txt::Notebook_QuitGame, 210);
    addChild(button);
}


ui::Label* Notebook::createLabel(txt::String text, float posY)
{
    ui::Label *label = new ui::Label;
    label->setFont(m_fontYellow);
    label->setText(txt::StringTable::get(text));
    label->setPosition(210, 121 + posY);
    label->setSize(304, -1);
    return label;
}


ui::Button* Notebook::createButton(txt::String text, float posY)
{
    ui::Button *button = new ui::Button;
    button->setFont(m_fontGreen);
    button->setText(txt::StringTable::get(text));
    button->setPosition(210, 121 + posY);
    button->setSize(304, -1);
    return button;
}


void Notebook::mousePressEvent(const QPointF &pos, Qt::MouseButton button)
{
    if (button == Qt::RightButton)
        emit close();
}


} // namespace game
