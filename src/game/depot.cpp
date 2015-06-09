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
#include "depot.h"
#include "boat.h"
#include "items.h"
#include "gfx/colortable.h"
#include "gfx/image.h"
#include "ui/button.h"
#include "txt/stringtable.h"


namespace game {


Depot::Depot() :
    m_boat(Chapter::get()->boat()),
    m_state(Flipping),
    m_side(1)
{
    const gfx::ColorTable colorTable("gfx:pal/gui/border.pal");
    gfx::ColorTable colorTableDisabled;
    for (int i = 0; i < 256; i++)
        colorTableDisabled << qRgba(qRed(colorTable[i]), qGreen(colorTable[i]), qBlue(colorTable[i]), 128);

    gfx::Image backgroundImage = gfx::Image::load("gfx:img/desktop/depot/depoback.img", colorTable);
    setupFrame(backgroundImage.size() + QSize(18, 24), "DEPOT", true);

    m_backgroundLabel = new ui::Label(this);
    m_backgroundLabel->setPosition(9, 15);
    m_backgroundLabel->setTexture(backgroundImage);

    ui::Label *label;

    gfx::Image horline = gfx::Image::load("gfx:img/desktop/gui/horline.img", colorTable);

    label = new ui::Label(m_backgroundLabel);
    label->setTexture(horline);
    label->setPosition(8, 64);

    label = new ui::Label(m_backgroundLabel);
    label->setTexture(horline);
    label->setPosition(8, 283);

    label = new ui::Label(m_backgroundLabel);
    label->setTexture(gfx::Image::load("gfx:img/desktop/gui/verline.img", colorTable));
    label->setPosition(352, 75);

    m_boatName = new ui::Label(m_backgroundLabel);
    m_boatName->setFont(gfx::Font::Large);
    m_boatName->setPosition(16, 75);
    m_boatName->setText(m_boat->name());

    m_credits = new ui::Label(m_backgroundLabel);
    m_credits->setFont(gfx::Font::Large);
    m_credits->setPosition(16, 263);
    updateCredits();

    m_lblInfo = new ui::Label(m_backgroundLabel);
    m_lblInfo->hide();

    m_lblItemName = new ui::Label(m_lblInfo);
    m_lblItemName->setFont(gfx::Font::Medium);
    m_lblItemName->setPosition(371, 75);
    m_lblItemName->setSize(128, 16);
    m_lblItemName->setAlignment(Label::AlignHCenter);

    m_lblItemPrice = new ui::Label(m_lblInfo);
    m_lblItemPrice->setFont(gfx::Font::Medium);
    m_lblItemPrice->setPosition(371, 87);
    m_lblItemPrice->setSize(128, 16);
    m_lblItemPrice->setAlignment(Label::AlignHCenter);

    m_lblItemVideo = new ui::Label(m_lblInfo);
    m_lblItemVideo->setPosition(371, 75);
    m_lblItemVideo->setTexture(m_itemTexture);

    m_lblItemText = new ui::List(m_lblInfo);
    m_lblItemText->setFont(gfx::Font::Small);
    m_lblItemText->setPosition(371, 105);
    m_lblItemText->setSize(160, 192);
    m_lblItemText->setAlignCenter(false);


    m_videoFlip1.open(QString("gfx:mvi/sflip/%1").arg(Chapter::get()->boat()->flipMovie1()));
    m_videoFlip2.open(QString("gfx:mvi/sflip/%1").arg(Chapter::get()->boat()->flipMovie2()));
    m_videoFlip1.setFrameRate(25);
    m_videoFlip2.setFrameRate(25);
    m_videoFlip2.play();

    Q_ASSERT(m_videoFlip1.width() == m_videoFlip2.width());
    Q_ASSERT(m_videoFlip1.height() == m_videoFlip2.height());
    m_boatTexture.createEmpty(m_videoFlip1.width(), m_videoFlip1.height(), gfx::Texture::RGBA);

    m_boatLabel = new ui::Label(m_backgroundLabel);
    m_boatLabel->setTexture(m_boatTexture);
    m_boatLabel->setPosition(16, 75);


    m_btnFlip = new ui::Button([this]() { flip(); }, m_backgroundLabel);
    m_btnFlip->setTexture(gfx::Image::load("gfx:img/desktop/depot/gdfliu.img", colorTable));
    m_btnFlip->setPressedTexture(gfx::Image::load("gfx:img/desktop/depot/gdflid.img", colorTable));
    m_btnFlip->setDisabledTexture(gfx::Image::load("gfx:img/desktop/depot/gdfliu.img", colorTableDisabled));
    m_btnFlip->setPosition(304, 243);

    m_btnRepair = new ui::Button([this]() { repair(); }, m_backgroundLabel);
    m_btnRepair->setTexture(gfx::Image::load("gfx:img/desktop/depot/gdammou.img", colorTable));
    m_btnRepair->setPressedTexture(gfx::Image::load("gfx:img/desktop/depot/gdammod.img", colorTable));
    m_btnRepair->setPosition(304, 75);

    m_btnBuy = new ui::Button([this]() { buy(); }, m_backgroundLabel);
    m_btnBuy->setTexture(gfx::Image::load("gfx:img/desktop/depot/gdbuyu.img", colorTable));
    m_btnBuy->setPressedTexture(gfx::Image::load("gfx:img/desktop/depot/gdbuyd.img", colorTable));
    m_btnBuy->setDisabledTexture(gfx::Image::load("gfx:img/desktop/depot/gdbuyu.img", colorTableDisabled));
    m_btnBuy->setPosition(507, 243);

    m_btnSell = new ui::Button([this]() { sell(); }, m_backgroundLabel);
    m_btnSell->setTexture(gfx::Image::load("gfx:img/desktop/depot/gdselu.img", colorTable));
    m_btnSell->setPressedTexture(gfx::Image::load("gfx:img/desktop/depot/gdseld.img", colorTable));
    m_btnSell->setDisabledTexture(gfx::Image::load("gfx:img/desktop/depot/gdselu.img", colorTableDisabled));
    m_btnSell->setPosition(507, 75);

    m_btnInfo = new ui::Button([this]() { toggleInfo(); }, m_backgroundLabel);
    m_btnInfo->setTexture(gfx::Image::load("gfx:img/desktop/depot/gdtinfu.img", colorTable));
    m_btnInfo->setPressedTexture(gfx::Image::load("gfx:img/desktop/depot/gdtinfd.img", colorTable));
    m_btnInfo->setDisabledTexture(gfx::Image::load("gfx:img/desktop/depot/gdtinfu.img", colorTableDisabled));
    m_btnInfo->setPosition(507, 131);

    updateButtons();


    m_itemList1 = new ui::ItemList([this](int n) { itemListClicked1(n); }, m_backgroundLabel, true);
    m_itemList1->setPosition(8, 8);

    m_itemList2 = new ui::ItemList([this](int n) { itemListClicked2(n); }, m_backgroundLabel, false);
    m_itemList2->setPosition(8, 293);

    for (int i = 0; i < m_boat->mountings().count(); i++)
    {
        Boat::Mounting *mounting = m_boat->mountings().at(i);
        ui::Arrow *arrow = new ui::Arrow(mounting->dir, mounting->pos, true, [this](int n) { loadMounting(n); }, m_backgroundLabel);
        arrow->setText(mounting->name);
        arrow->setValue(i);
        arrow->hide();
        m_mountingArrows << arrow;
    }

    m_sndFlip.load("sfx:snd/desktop/shipdreh.pcm");
    m_sndWeapon.load("sfx:snd/desktop/weapchng.pcm");
    m_sndError.load("sfx:snd/desktop/error.pcm");
    m_sndButton.load("sfx:snd/desktop/button.pcm");
    m_sndNoop.load("sfx:snd/desktop/cursor1.pcm");
    m_sndSelect.load("sfx:snd/desktop/cursor2.pcm");
    m_sndButton.play();
    m_sndFlip.play();
}


Depot::~Depot()
{
}


void Depot::flip()
{
    m_sndButton.play();
    if (m_state == Ready && m_side == 1)
    {
        m_videoFlip1.play();
        m_state = Flipping;
        m_side = 2;
    }
    if (m_state == Ready && m_side == 2)
    {
        m_videoFlip2.play();
        m_state = Flipping;
        m_side = 1;
    }
    if (m_state == Flipping)
    {
        foreach (ui::Arrow *arrow, m_mountingArrows)
            arrow->hide();
        m_itemList1->clear();
        m_itemList2->clear();
        m_lblInfo->hide();
        updateButtons();
        m_sndFlip.play();
    }
}


void Depot::loadMounting(int index)
{
    if (m_mounting != index)
    {
        m_mounting = index;
        m_state = Loading;
        m_itemList1->clear();
        m_itemList2->clear();
        m_lblInfo->hide();
        updateButtons();

        m_loadingState = List2;
        m_loadingItem = 0;
        m_list2 = m_boat->getItems(m_boat->mountings().at(m_mounting)->name);
        m_selectedList = 2;
        m_selectedItem = -1;
    }
    else
        m_sndNoop.play();
}


void Depot::itemListClicked1(int index)
{
    if (m_selectedList == 2)
        m_itemList2->selectItem(-1);
    m_selectedList = 1;
    m_selectedItem = index;
    m_itemList1->selectItem(index);

    updateInfo();
    updateButtons();
    m_sndSelect.play();
}


void Depot::itemListClicked2(int index)
{
    if (m_selectedList == 1)
        m_itemList1->selectItem(-1);
    m_selectedList = 2;
    m_selectedItem = index;
    m_itemList2->selectItem(index);

    updateInfo();
    updateButtons();
    m_sndSelect.play();
}


void Depot::toggleInfo()
{
    m_sndButton.play();
    if (m_lblItemText->isVisible())
        m_lblItemText->hide();
    else
        m_lblItemText->show();
}


void Depot::repair()
{
    m_sndButton.play();
}


void Depot::updateInfo()
{
    Items::Item *item = NULL;
    if (m_selectedList == 1)
        item = Items::get(m_list1.at(m_selectedItem));
    if (m_selectedList == 2 && m_selectedItem >= 0)
        item = Items::get(m_list2.at(m_selectedItem));
    if (item == NULL)
    {
        m_lblInfo->hide();
        return;
    }

    m_lblItemName->setText(item->longname);
    m_lblItemText->setText(item->text);

    m_videoItem.open(QString("gfx:mvi/thing/%1.mvi").arg(item->imgname));
    m_videoItem.setFrameRate(13);
    m_videoItem.playLoop();
    m_itemTexture.createEmpty(m_videoItem.width(), m_videoItem.height(), gfx::Texture::RGBA);

    int price = Items::getDepotPrice(item->model);
    if (m_selectedList == 2) {
        if (m_boat->canSell(item->model, m_boat->mountings().at(m_mounting)->name)) {
            m_lblItemPrice->setText(txt::StringTable::get(txt::Depot_Bid) + QString("%1").arg(price));
        } else {
            m_lblItemPrice->setText(txt::StringTable::get(txt::Depot_Value) + QString("%1").arg(price));
        }
    }
    if (m_selectedList == 1)
    {
        int oldModel;
        m_boat->canBuy(item->model, m_boat->mountings().at(m_mounting)->name, &oldModel);
        if (oldModel > 0)
        {
            int oldPrice = Items::getDepotPrice(oldModel);
            if (oldPrice < price)
                m_lblItemPrice->setText(txt::StringTable::get(txt::Depot_Upgrade) + QString("%1").arg(price - oldPrice));
            else
                m_lblItemPrice->setText(txt::StringTable::get(txt::Depot_Degrade) + QString("%1").arg(price - oldPrice));
        }
        else
            m_lblItemPrice->setText(txt::StringTable::get(txt::Depot_Cost) + QString("%1").arg(price));
    }

    m_lblInfo->show();
}


void Depot::updateCredits()
{
    m_credits->setText(QString("CREDITS: $%1").arg(Chapter::get()->credits()));
}


void Depot::updateButtons()
{
    if (m_state == Flipping)
        m_btnFlip->disable();
    if (m_state == Ready)
        m_btnFlip->enable();

    if (m_state == Flipping || m_state == Loading || (m_state == Ready && m_selectedItem < 0))
    {
        m_btnBuy->disable();
        m_btnInfo->disable();
        m_btnSell->disable();
    }

    if (m_state == Ready && m_selectedItem >= 0)
    {
        m_btnInfo->enable();

        if (m_selectedList == 1 && m_boat->isCompatible(m_list1.at(m_selectedItem)))
            m_btnBuy->enable();
        else
            m_btnBuy->disable();

        if (m_selectedList == 2 && m_boat->canSell(m_list2.at(m_selectedItem), m_boat->mountings().at(m_mounting)->name))
            m_btnSell->enable();
        else
            m_btnSell->disable();
    }
}


void Depot::buy()
{
    const QString &mounting = m_boat->mountings().at(m_mounting)->name;
    if (m_selectedList != 1)
        return;
    Items::Item *item = Items::get(m_list1.at(m_selectedItem));
    if (!m_boat->isCompatible(item->model))
        return;
    int oldModel;
    if (!m_boat->canBuy(item->model, mounting, &oldModel))
    {
        m_sndError.play();
        return;
    }
    int price = Items::getDepotPrice(item->model);
    if (oldModel > 0)
        price -= Items::getDepotPrice(oldModel);
    if (price > Chapter::get()->credits())
    {
        m_sndError.play();
        return;
    }
    Chapter::get()->addCredit(-price);
    m_boat->buy(item->model, mounting);

    m_list2 = m_boat->getItems(mounting);
    m_itemList2->clear();
    foreach (int model, m_list2)
        m_itemList2->addItem(Items::get(model)->icon);
 
    updateCredits();
    m_sndWeapon.play();
}


void Depot::sell()
{
    const QString &mounting = m_boat->mountings().at(m_mounting)->name;
    if (m_selectedList != 2 || m_selectedItem >= m_list2.count())
        return;
    Items::Item *item = Items::get(m_list2.at(m_selectedItem));
    if (!m_boat->canSell(item->model, mounting))
        return;
    int price = Items::getDepotPrice(item->model);

    int index = 0;
    for (int i = 0; i < m_selectedItem; i++)
        if (Items::get(m_list2.at(i))->type == item->type)
            index++;
    m_boat->sell(item->model, index, mounting);
    Chapter::get()->addCredit(price);

    m_list2 = m_boat->getItems(mounting);
    m_itemList2->clear();
    foreach (int model, m_list2)
        m_itemList2->addItem(Items::get(model)->icon);
    if (m_selectedItem >= m_list2.count())
        m_selectedItem = m_list2.count() - 1;
    m_itemList2->selectItem(m_selectedItem);

    updateInfo();
    updateButtons();
    updateCredits();
    m_sndWeapon.play();
}


void Depot::draw()
{
    ui::Label::draw();

    if (m_state == Ready)
    {
        if (m_lblInfo->isVisible())
        {
            gfx::Image frame = m_videoItem.getFrame();
            if (!frame.isNull())
            {
                gfx::ColorTable colorTable = frame.colorTable();
                colorTable[0] = qRgba(0, 0, 0, 0);
                frame.setColorTable(colorTable);
                m_itemTexture.update(0, 0, frame);
            }
        }
    }
    if (m_state == Flipping)
    {
        gfx::Video *video;
        if (m_side == 1)
            video = &m_videoFlip2;
        if (m_side == 2)
            video = &m_videoFlip1;

        gfx::Image frame = video->getFrame();
        if (!frame.isNull())
        {
            gfx::ColorTable colorTable = frame.colorTable();
            colorTable[0] = qRgba(0, 0, 0, 0);
            frame.setColorTable(colorTable);
            m_boatTexture.update(0, 0, frame);
        }

        if (video->atEnd())
        {
            m_state = Loading;
            m_loadingState = Arrows;
            m_loadingItem = 0;
        }
    }

    switch (m_state)
    {
    case Flipping:
    case Ready:
    case Repair:
        return;
    case Loading:
        if (m_loadingState == Arrows && m_time.elapsed() < 95)
            return;
        if (m_loadingState != Arrows && m_time.elapsed() < 35)
            return;
        break;
    }

    m_time.restart();

    if (m_state == Loading)
    {
        if (m_loadingState == Arrows)
        {
            int i;
            int j = 0;
            for (i = 0; i < m_boat->mountings().count(); i++)
            {
                Boat::Mounting *mounting = m_boat->mountings().at(i);
                if (mounting->side + 1 == m_side)
                {
                    if (m_loadingItem == 0)
                        m_mounting = i;
                    if (j == m_loadingItem)
                    {
                        m_mountingArrows.at(i)->show();
                        m_loadingItem++;
                        break;
                    }
                    j++;
                }
            }
            if (i >= m_boat->mountings().count())
            {
                m_loadingState = List2;
                m_loadingItem = 0;
                m_list2 = m_boat->getItems(m_boat->mountings().at(m_mounting)->name);
                m_selectedList = 2;
                m_selectedItem = -1;
            }
        }
        if (m_loadingState == List2)
        {
            if (m_loadingItem < m_list2.count())
            {
                int model = m_list2.at(m_loadingItem);
                m_itemList2->addItem(Items::get(model)->icon);
                if (m_selectedItem < 0)
                {
                    m_selectedItem = m_loadingItem;
                    m_itemList2->selectItem(m_selectedItem);
                }
                m_loadingItem++;
                m_sndSelect.play();
            }
            else
            {
                m_loadingState = List1;
                m_loadingItem = -1;
                m_list1 = Items::getDepotItems(m_boat->mountings().at(m_mounting)->name);
            }
        }
        if (m_loadingState == List1)
        {
            if (m_loadingItem < 0)
                m_loadingItem++;
            else if (m_loadingItem < m_list1.count())
            {
                while (m_loadingItem < m_list1.count())
                {
                    int model = m_list1.at(m_loadingItem);
                    bool compatible = m_boat->isCompatible(model);
                    m_itemList1->addItem(Items::get(model)->icon, !compatible, compatible);
                    m_loadingItem++;
                    if (m_loadingItem <= 8)
                        break;
                }
                m_sndSelect.play();
            }
            else
            {
                m_state = Ready;
                updateInfo();
                updateButtons();
            }
        }
    }
}


bool Depot::mousePressEvent(const QPoint &pos, Qt::MouseButton button)
{
    if (button == Qt::RightButton) {
        closeFrame();
        return true;
    }

    return false;
}


} // namespace game
