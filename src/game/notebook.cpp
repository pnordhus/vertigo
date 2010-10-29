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
#include "notebook.h"
#include "gfx/colortable.h"
#include "gfx/image.h"


namespace game {


Notebook::Notebook()
{
    const gfx::ColorTable colorTable("gfx:pal/notebook/notebook.pal");
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
        connect(widget, SIGNAL(clicked()), SLOT(showMissions()));

        widget = createButton(m_lblMain, txt::Notebook_LoadSave, 110);

        widget = createButton(m_lblMain, txt::Notebook_Options, 130);
        connect(widget, SIGNAL(clicked()), SLOT(showOptions()));

        widget = createButton(m_lblMain, txt::Notebook_Map, 150);
        connect(widget, SIGNAL(clicked()), SLOT(showMap()));

        widget = createButton(m_lblMain, txt::Notebook_Back, 170);
        connect(widget, SIGNAL(clicked()), SIGNAL(close()));

        widget = createButton(m_lblMain, txt::Notebook_QuitGame, 210);
        connect(widget, SIGNAL(clicked()), Chapter::get(), SLOT(quit()));
    }

    {
        m_lblMissions = new ui::Label(labelBackground);
        m_lblMissions->setPosition(162, 73);
        m_lblMissions->setTexture(noteback);

        ui::Widget *widget;

        widget = createLabel(m_lblMissions, txt::Notebook_Missions_Title, 10);
        widget = createLabel(m_lblMissions, txt::Notebook_Missions_TitleLine, 20);

        widget = createLabel(m_lblMissions, txt::Notebook_Missions_TitleLine, 20);

        ui::Button *button = new ui::Button(m_lblMissions);
        button->setFont(m_fontGreen);
        button->setText(txt::StringTable::get(txt::Notebook_Back));
        button->setPosition(180, 270);
        connect(button, SIGNAL(clicked()), SLOT(hideMissions()));

        m_lblMissions->hide();
    }

    {
        m_lblOptions = new ui::Label(labelBackground);
        m_lblOptions->setPosition(162, 73);
        m_lblOptions->setTexture(noteback);

        ui::Widget *widget;

        widget = createLabel(m_lblOptions, txt::Notebook_Options_Title, 30);
        widget = createLabel(m_lblOptions, txt::Notebook_Options_TitleLine, 40);

        widget = createButton(m_lblOptions, txt::Notebook_Sound, 90);

        widget = createButton(m_lblOptions, txt::Notebook_Graphics, 110);

        widget = createButton(m_lblOptions, txt::Notebook_Movies, 130);

        widget = createButton(m_lblOptions, txt::Notebook_InputDevices, 150);

        widget = createButton(m_lblOptions, txt::Notebook_MoviePlayer, 170);

        widget = createButton(m_lblOptions, txt::Notebook_Back, 220);
        connect(widget, SIGNAL(clicked()), SLOT(hideOptions()));

        m_lblOptions->hide();
    }

    {
        m_lblMap = new ui::Label(labelBackground);
        m_lblMap->setPosition(164, 75);
        m_lblMap->setTexture(gfx::Image::load(QString("gfx:pic/notebook/%1.r16").arg(Chapter::get()->area()->map()), 304, 284));
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


void Notebook::showMissions()
{
    m_lblMain->hide();
    m_lblMissions->show();
}


void Notebook::hideMissions()
{
    m_lblMissions->hide();
    m_lblMain->show();
}


void Notebook::showOptions()
{
    m_lblMain->hide();
    m_lblOptions->show();
}


void Notebook::hideOptions()
{
    m_lblOptions->hide();
    m_lblMain->show();
}


void Notebook::showMap()
{
    m_lblMain->hide();
    m_lblMap->show();
}


void Notebook::hideMap()
{
    m_lblMap->hide();
    m_lblMain->show();
}


bool Notebook::mousePressEvent(const QPoint &pos, Qt::MouseButton button)
{
    if (m_lblMap->isVisible()) {
        hideMap();
        return true;
    }

    if (button == Qt::RightButton) {
        if (m_lblMissions->isVisible()) {
            hideMissions();
            return true;
        }
        if (m_lblOptions->isVisible()) {
            hideOptions();
            return true;
        }
        if (m_lblMain->isVisible()) {
            emit close();
            return true;
        }
    }

    return false;
}


} // namespace game
