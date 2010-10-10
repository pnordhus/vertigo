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
    m_fontGreen.load("gfx:fnt/nfont1a.fnt", colorTable, true);
    m_fontYellow.load("gfx:fnt/nfont1b.fnt", colorTable, true);
    gfx::Texture noteback(gfx::Image::load("gfx:img/desktop/notebook/noteback.img", colorTable));

    QImage image(640, 480, QImage::Format_Indexed8);
    image.fill(0);
    image.setColorTable(QVector<QRgb>() << qRgba(0, 0, 0, 128));
    setTexture(image);

    ui::Label *labelBackground = new ui::Label(this);
    labelBackground->setPosition(48, 48);
    labelBackground->setTexture(gfx::Image::loadPCX("gfx:pic/notebook/notebook.pcx"));

    {
        m_lblMain = new ui::Label(labelBackground);
        m_lblMain->setPosition(162, 73);
        m_lblMain->setTexture(noteback);

        ui::Widget *widget;

        widget = createLabel(m_lblMain, txt::Notebook_Title, 30);

        widget = createLabel(m_lblMain, txt::Notebook_TitleLine, 40);

        widget = createButton(m_lblMain, txt::Notebook_Missions, 90);

        widget = createButton(m_lblMain, txt::Notebook_LoadSave, 110);

        widget = createButton(m_lblMain, txt::Notebook_Options, 130);

        widget = createButton(m_lblMain, txt::Notebook_Map, 150);
        connect(widget, SIGNAL(clicked()), SLOT(showMap()));

        widget = createButton(m_lblMain, txt::Notebook_Back, 170);
        connect(widget, SIGNAL(clicked()), SIGNAL(close()));

        widget = createButton(m_lblMain, txt::Notebook_QuitGame, 210);
    }

    {
        m_lblMap = new ui::Label(labelBackground);
        m_lblMap->setPosition(164, 75);
        m_lblMap->setTexture(gfx::Image::load("gfx:pic/notebook/ar02.r16", 304, 284));
        m_lblMap->hide();
    }
}


ui::Label* Notebook::createLabel(ui::Widget *parent, txt::String text, float posY)
{
    ui::Label *label = new ui::Label(parent);
    label->setFont(m_fontYellow);
    label->setText(txt::StringTable::get(text));
    label->setPosition(0, posY);
    label->setWidth(304);
    label->setAlignment(ui::Label::AlignHCenter);
    return label;
}


ui::Button* Notebook::createButton(ui::Widget *parent, txt::String text, float posY)
{
    ui::Button *button = new ui::Button(parent);
    button->setFont(m_fontGreen);
    button->setText(txt::StringTable::get(text));
    button->setPosition(0, posY);
    button->setWidth(304);
    button->setAlignment(ui::Label::AlignHCenter);
    return button;
}


void Notebook::showMap()
{
    m_lblMain->hide();
    m_lblMap->show();
}


bool Notebook::mousePressEvent(const QPointF &pos, Qt::MouseButton button)
{
    if (m_lblMap->isVisible()) {
        m_lblMap->hide();
        m_lblMain->show();
        return true;
    }

    if (button == Qt::RightButton) {
        if (m_lblMain->isVisible()) {
            emit close();
            return true;
        }
    }

    return false;
}


} // namespace game
