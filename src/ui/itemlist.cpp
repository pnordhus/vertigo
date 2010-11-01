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

#include "itemlist.h"
#include "gfx/colortable.h"
#include "gfx/image.h"


namespace ui {


ItemList::ItemList(Widget *parent) :
    Widget(parent),
    m_firstItem(0),
    m_selectedItem(-1)
{
    setSize(539, 48);

    m_texture.createEmpty(435, 48, gfx::Texture::RGBA);

    m_lblList = new ui::Label(this);
    m_lblList->setTexture(m_texture);
    m_lblList->setPosition(54, 0);

    const gfx::ColorTable colorTable("gfx:pal/gui/border.pal");

    m_btnLeft = new ui::Button(this);
    m_btnLeft->setTexture(gfx::Image::load("gfx:img/desktop/depot/gdlefu.img", colorTable));
    m_btnLeft->setPressedTexture(gfx::Image::load("gfx:img/desktop/depot/gdlefd.img", colorTable));
    m_btnLeft->setPosition(13, 8);

    m_btnRight = new ui::Button(this);
    m_btnRight->setTexture(gfx::Image::load("gfx:img/desktop/depot/gdrigu.img", colorTable));
    m_btnRight->setPressedTexture(gfx::Image::load("gfx:img/desktop/depot/gdrigd.img", colorTable));
    m_btnRight->setPosition(493, 8);

    m_chkGreen = gfx::Image::load("gfx:img/desktop/depot/chkgre.img", colorTable);
    m_chkRed = gfx::Image::load("gfx:img/desktop/depot/chkred.img", colorTable);
    m_imgFrame = gfx::Image::load("gfx:img/desktop/depot/iframe.img", colorTable);
}


ItemList::~ItemList()
{
    qDeleteAll(m_items);
}


void ItemList::addItem(const gfx::Image &icon, bool red, bool green)
{
    Item *item = new Item();
    item->icon = icon;
    item->red = red;
    item->green = green;
    m_items << item;

    redraw();
}


void ItemList::selectItem(int index)
{
    m_selectedItem = index;
    redraw();
}


void ItemList::draw()
{
}

void ItemList::redraw()
{
    for (int i = 0; i < 9 && i + m_firstItem < m_items.count(); i++)
    {
        Item *item = m_items.at(i + m_firstItem);
        m_texture.update(54*i, 0, item->icon);
        if (item->green)
            m_texture.update(54*i + 4, 4, m_chkGreen);
        if (item->red)
            m_texture.update(54*i + 4, 4, m_chkRed);
        if (m_selectedItem == i + m_firstItem)
            m_texture.update(54*i, 0, m_imgFrame);
    }
}


} // namespace ui
