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


Depot::Depot(std::function<void()> &&funcClose) :
    Frame(std::move(funcClose)),
    m_boat(Chapter::get()->boat()),
    m_side(1),
    m_state(Flipping),
    m_backgroundLabel(this),
    m_panelMountings(&m_backgroundLabel),
    m_panelRepair(&m_backgroundLabel, false),
    m_repairCost(&m_panelRepair),
    m_repairButton(&m_panelRepair)
{
    const gfx::ColorTable colorTable("gfx:pal/gui/border.pal");
    gfx::ColorTable colorTableDisabled;
    for (int i = 0; i < 256; i++)
        colorTableDisabled << qRgba(qRed(colorTable[i]), qGreen(colorTable[i]), qBlue(colorTable[i]), 128);

    gfx::Image backgroundImage = gfx::Image::load("gfx:img/desktop/depot/depoback.img", colorTable);
    setupFrame(backgroundImage.size() + QSize(18, 24), "DEPOT", true);

    m_backgroundLabel.setPosition(9, 15);
    m_backgroundLabel.setTexture(backgroundImage);

    ui::Label *label;

    gfx::Image horline = gfx::Image::load("gfx:img/desktop/gui/horline.img", colorTable);

    label = new ui::Label(&m_backgroundLabel);
    label->setTexture(horline);
    label->setPosition(8, 64);

    label = new ui::Label(&m_backgroundLabel);
    label->setTexture(horline);
    label->setPosition(8, 283);

    label = new ui::Label(&m_backgroundLabel);
    label->setTexture(gfx::Image::load("gfx:img/desktop/gui/verline.img", colorTable));
    label->setPosition(352, 75);

    m_boatName = new ui::Label(&m_panelMountings);
    m_boatName->setFont(gfx::Font::Large);
    m_boatName->setPosition(16, 75);
    m_boatName->setText(m_boat->name());

    m_credits = new ui::Label(&m_backgroundLabel);
    m_credits->setFont(gfx::Font::Large);
    m_credits->setPosition(16, 263);
    updateCredits();

    m_lblInfo = new ui::Label(&m_backgroundLabel);
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


    m_videoFlip1.open(QString("gfx:mvi/sflip/%011.mvi").arg(Chapter::get()->boat()->moviePrefix()));
    m_videoFlip2.open(QString("gfx:mvi/sflip/%012.mvi").arg(Chapter::get()->boat()->moviePrefix()));
    m_videoFlip1.setFrameRate(25);
    m_videoFlip2.setFrameRate(25);
    m_videoFlip2.play();

    Q_ASSERT(m_videoFlip1.width() == m_videoFlip2.width());
    Q_ASSERT(m_videoFlip1.height() == m_videoFlip2.height());
    m_boatTexture.createEmpty(m_videoFlip1.width(), m_videoFlip1.height(), gfx::Texture::RGBA);

    m_boatLabel = new ui::Label(&m_panelMountings);
    m_boatLabel->setTexture(m_boatTexture);
    m_boatLabel->setPosition(16, 75);


    m_btnFlip = new ui::Button([this]() { flip(); }, &m_backgroundLabel);
    m_btnFlip->setTexture(gfx::Image::load("gfx:img/desktop/depot/gdfliu.img", colorTable));
    m_btnFlip->setPressedTexture(gfx::Image::load("gfx:img/desktop/depot/gdflid.img", colorTable));
    m_btnFlip->setDisabledTexture(gfx::Image::load("gfx:img/desktop/depot/gdfliu.img", colorTableDisabled));
    m_btnFlip->setPosition(304, 243);

    m_btnRepair = new ui::Button([this]() { repair(); }, &m_backgroundLabel);
    m_btnRepair->setTexture(gfx::Image::load("gfx:img/desktop/depot/gdammou.img", colorTable));
    m_btnRepair->setPressedTexture(gfx::Image::load("gfx:img/desktop/depot/gdammod.img", colorTable));
    m_btnRepair->setPosition(304, 75);

    m_btnBuy = new ui::Button([this]() { buy(); }, &m_backgroundLabel);
    m_btnBuy->setTexture(gfx::Image::load("gfx:img/desktop/depot/gdbuyu.img", colorTable));
    m_btnBuy->setPressedTexture(gfx::Image::load("gfx:img/desktop/depot/gdbuyd.img", colorTable));
    m_btnBuy->setDisabledTexture(gfx::Image::load("gfx:img/desktop/depot/gdbuyu.img", colorTableDisabled));
    m_btnBuy->setPosition(507, 243);

    m_btnSell = new ui::Button([this]() { sell(); }, &m_backgroundLabel);
    m_btnSell->setTexture(gfx::Image::load("gfx:img/desktop/depot/gdselu.img", colorTable));
    m_btnSell->setPressedTexture(gfx::Image::load("gfx:img/desktop/depot/gdseld.img", colorTable));
    m_btnSell->setDisabledTexture(gfx::Image::load("gfx:img/desktop/depot/gdselu.img", colorTableDisabled));
    m_btnSell->setPosition(507, 75);

    m_btnInfo = new ui::Button([this]() { toggleInfo(); }, &m_backgroundLabel);
    m_btnInfo->setTexture(gfx::Image::load("gfx:img/desktop/depot/gdtinfu.img", colorTable));
    m_btnInfo->setPressedTexture(gfx::Image::load("gfx:img/desktop/depot/gdtinfd.img", colorTable));
    m_btnInfo->setDisabledTexture(gfx::Image::load("gfx:img/desktop/depot/gdtinfu.img", colorTableDisabled));
    m_btnInfo->setPosition(507, 131);

    updateButtons();


    m_itemList1 = new ui::ItemList([this](int n) { itemListClicked1(n); }, &m_backgroundLabel, true);
    m_itemList1->setPosition(8, 8);

    m_itemList2 = new ui::ItemList([this](int n) { itemListClicked2(n); }, &m_backgroundLabel, false);
    m_itemList2->setPosition(8, 293);

    int i;
    bool tur1 = false;
    bool tur2 = false;
    for (i = 0; i < m_boat->mountings().size(); i++)
    {
        const Boat::Mounting &mounting = m_boat->mountings()[i];
        ui::Arrow &arrow = m_mountingArrows.emplace(
            std::piecewise_construct,
            std::forward_as_tuple(i),
            std::forward_as_tuple(mounting.dir, mounting.pos, true, [this](int n) { loadMounting(n); }, &m_panelMountings)
        ).first->second;
        arrow.setText(mounting.name);
        arrow.setValue(i);
        arrow.hide();
        if (mounting.name == "TUR1")
            tur1 = true;
        if (mounting.name == "TUR2")
            tur2 = true;
    }


    m_repairLabels.emplace_back(&m_panelRepair);
    m_repairLabels.back().setFont(gfx::Font::Large);
    m_repairLabels.back().setPosition(16, 75);
    m_repairLabels.back().setText(txt::StringTable::get(txt::Repair));

    m_repairLabels.emplace_back(&m_panelRepair);
    m_repairLabels.back().setFont(gfx::Font::Medium);
    m_repairLabels.back().setPosition(42, 107);
    m_repairLabels.back().setText(txt::StringTable::get(txt::Repair_Type));

    m_repairLabels.emplace_back(&m_panelRepair);
    m_repairLabels.back().setFont(gfx::Font::Medium);
    m_repairLabels.back().setPosition(98, 107);
    m_repairLabels.back().setText(txt::StringTable::get(txt::Repair_State));

    m_repairLabels.emplace_back(&m_panelRepair);
    m_repairLabels.back().setFont(gfx::Font::Medium);
    m_repairLabels.back().setPosition(162, 107);
    m_repairLabels.back().setText(txt::StringTable::get(txt::Repair_Cost));

    m_repairItems.emplace_back(m_boat->gun(), "GUN", &m_panelRepair);
    m_repairItems.emplace_back(m_boat->magazine(), "TORP", &m_panelRepair);
    m_repairItems.emplace_back(m_boat->armor(), "DEFE", &m_panelRepair);
    if (tur1)
        m_repairItems.emplace_back(m_boat->tur1(), "TUR1", &m_panelRepair);
    if (tur2)
        m_repairItems.emplace_back(m_boat->tur2(), "TUR2", &m_panelRepair);

    gfx::Font fontMediumDisabled("gfx:fnt/dpmedium.fnt", colorTableDisabled);

    int total = 0;
    for (i = 0; i < m_repairItems.size(); i++)
    {
        RepairItem &item = m_repairItems[i];
        item.state = m_boat->repairState(item.model, item.mounting);
        item.cost = m_boat->repairCost(item.model, item.mounting);
        total += item.cost;

        m_repairLabels.emplace_back(&m_panelRepair);
        m_repairLabels.back().setFont(gfx::Font::Medium);
        m_repairLabels.back().setPosition(42, 131 + i*16);
        m_repairLabels.back().setText(i == 2 ? "ARMO" : item.mounting);

        item.lblState.setFont(gfx::Font::Medium);
        item.lblState.setPosition(98, 131 + i*16);
        item.lblState.setText(QString("%1%").arg(item.state));

        item.lblCost.setFont(gfx::Font::Medium);
        item.lblCost.setPosition(162, 131 + i*16);
        item.lblCost.setText(QString("$%1").arg(item.cost));

        item.btnRepair.setFont(gfx::Font::Medium);
        item.btnRepair.setDisabledFont(fontMediumDisabled);
        item.btnRepair.setPosition(218, 131 + i*16);
        item.btnRepair.setText(txt::StringTable::get(txt::Repair));
        if (item.cost == 0)
            item.btnRepair.disable();
        item.btnRepair.eventClicked() += [this, i] { m_sndButton.play(); repairItem(i); };
    }

    m_repairLabels.emplace_back(&m_panelRepair);
    m_repairLabels.back().setFont(gfx::Font::Medium);
    m_repairLabels.back().setPosition(162, 131 + i*16);
    m_repairLabels.back().setText("-----");
    i++;

    m_repairCost.setFont(gfx::Font::Medium);
    m_repairCost.setPosition(162, 131 + i*16);
    m_repairCost.setText(QString("$%1").arg(total));

    m_repairButton.setFont(gfx::Font::Medium);
    m_repairButton.setDisabledFont(fontMediumDisabled);
    m_repairButton.setPosition(218, 131 + i*16);
    m_repairButton.setText(txt::StringTable::get(txt::Repair));
    if (total == 0)
        m_repairButton.disable();
    m_repairButton.eventClicked() += [this] { m_sndButton.play(); repairAll(); };


    m_sndFlip.load("sfx:snd/desktop/shipdreh.pcm");
    m_sndWeapon.load("sfx:snd/desktop/weapchng.pcm");
    m_sndError.load("sfx:snd/desktop/error.pcm");
    m_sndButton.load("sfx:snd/desktop/button.pcm");
    m_sndNoop.load("sfx:snd/desktop/cursor1.pcm");
    m_sndSelect.load("sfx:snd/desktop/cursor2.pcm");
    m_sndButton.play();
    m_sndFlip.play();
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
        for (auto &arrow : m_mountingArrows) {
            arrow.second.hide();
        }
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
        m_list2 = m_boat->getItems(m_boat->mountings()[m_mounting].name);
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
    if (m_state == Repair)
        repair();
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
    if (m_state == Repair)
    {
        m_panelMountings.show();
        m_panelRepair.hide();
        m_btnFlip->enable();
        m_state = Ready;
    }
    else if (m_state == Ready)
    {
        m_panelMountings.hide();
        m_panelRepair.show();
        m_btnFlip->disable();
        m_state = Repair;
    }
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
        if (m_boat->repairCost(item->model, m_boat->mountings()[m_mounting].name) > 0)
            m_lblItemPrice->setText(txt::StringTable::get(txt::Repair_Defective));
        else if (m_boat->canSell(item->model, m_boat->mountings()[m_mounting].name))
            m_lblItemPrice->setText(txt::StringTable::get(txt::Depot_Bid) + QString("%1").arg(price));
        else
            m_lblItemPrice->setText(txt::StringTable::get(txt::Depot_Value) + QString("%1").arg(price));
    }
    if (m_selectedList == 1)
    {
        int oldModel;
        m_boat->canBuy(item->model, m_boat->mountings()[m_mounting].name, &oldModel);
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

    if ((m_state == Ready && m_selectedItem >= 0) || m_state == Repair)
    {
        m_btnInfo->enable();

        if (m_selectedList == 1 && m_boat->isCompatible(m_list1.at(m_selectedItem)))
            m_btnBuy->enable();
        else
            m_btnBuy->disable();

        if (m_selectedList == 2 && m_boat->canSell(m_list2.at(m_selectedItem), m_boat->mountings()[m_mounting].name))
            m_btnSell->enable();
        else
            m_btnSell->disable();
    }
}


void Depot::buy()
{
    const QString &mounting = m_boat->mountings()[m_mounting].name;
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
    const QString &mounting = m_boat->mountings()[m_mounting].name;
    if (m_selectedList != 2 || m_selectedItem >= m_list2.size())
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
    if (m_selectedItem >= m_list2.size())
        m_selectedItem = m_list2.size() - 1;
    m_itemList2->selectItem(m_selectedItem);

    updateInfo();
    updateButtons();
    updateCredits();
    m_sndWeapon.play();
}


void Depot::draw()
{
    ui::Label::draw();

    if (m_state == Ready || m_state == Repair)
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
            for (i = 0; i < m_boat->mountings().size(); i++)
            {
                const Boat::Mounting &mounting = m_boat->mountings()[i];
                if (mounting.side + 1 == m_side)
                {
                    if (m_loadingItem == 0)
                        m_mounting = i;
                    if (j == m_loadingItem)
                    {
                        m_mountingArrows.at(i).show();
                        m_loadingItem++;
                        break;
                    }
                    j++;
                }
            }
            if (i >= m_boat->mountings().size())
            {
                m_loadingState = List2;
                m_loadingItem = 0;
                m_list2 = m_boat->getItems(m_boat->mountings()[m_mounting].name);
                m_selectedList = 2;
                m_selectedItem = -1;
            }
        }
        if (m_loadingState == List2)
        {
            if (m_loadingItem < m_list2.size())
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
                m_list1 = Items::getDepotItems(m_boat->mountings()[m_mounting].name);
            }
        }
        if (m_loadingState == List1)
        {
            if (m_loadingItem < 0)
                m_loadingItem++;
            else if (m_loadingItem < m_list1.size())
            {
                while (m_loadingItem < m_list1.size())
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


void Depot::repairItem(int index)
{
    if (Chapter::get()->credits() == 0)
    {
        m_sndError.play();
        return;
    }

    RepairItem &item = m_repairItems[index];
    float amount = 1.0f;
    int cost = item.cost;
    if (cost > Chapter::get()->credits())
    {
        cost = Chapter::get()->credits();
        amount = static_cast<float>(cost)/item.cost;
    }
    Chapter::get()->addCredit(-cost);
    m_boat->repair(item.model, item.mounting, amount);
    item.state = m_boat->repairState(item.model, item.mounting);
    item.cost = m_boat->repairCost(item.model, item.mounting);

    item.lblState.setText(QString("%1%").arg(item.state));
    item.lblCost.setText(QString("$%1").arg(item.cost));
    if (item.cost == 0)
        item.btnRepair.disable();
    
    int total = 0;
    for (RepairItem &item : m_repairItems)
        total += item.cost;
    m_repairCost.setText(QString("$%1").arg(total));
    if (total == 0)
        m_repairButton.disable();
    updateCredits();
}


void Depot::repairAll()
{
    if (Chapter::get()->credits() == 0)
    {
        m_sndError.play();
        return;
    }
    for (int i = 0; i < m_repairItems.size() && Chapter::get()->credits() > 0; i++)
        repairItem(i);
}


} // namespace game
