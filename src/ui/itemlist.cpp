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

ItemList::ItemList(std::function<void(int)> &&funcClicked, Widget *parent, bool showChecks, int maxItems) :
    Widget(parent),
    m_funcClicked(std::move(funcClicked)),
    m_btnLeft([this]() { scrollLeft(); }, this),
    m_btnRight([this]() { scrollRight(); }, this),
    m_items(0),
    m_firstItem(0),
    m_selectedItem(-1),
    m_offset(0)
{
    setSize(539, 48);

    m_texture.createEmpty(54*maxItems + 3, 48, gfx::Texture::RGBA);
    if (showChecks)
        m_textureChecks.createEmpty(54*maxItems + 3, 48, gfx::Texture::RGBA);

    const gfx::ColorTable colorTable("gfx:pal/gui/border.pal");
    gfx::ColorTable colorTableDisabled;
    for (int i = 0; i < 256; i++)
        colorTableDisabled << qRgba(qRed(colorTable[i]), qGreen(colorTable[i]), qBlue(colorTable[i]), 128);

    m_btnLeft.setTexture(gfx::Image::load("gfx:img/desktop/depot/gdlefu.img", colorTable));
    m_btnLeft.setPressedTexture(gfx::Image::load("gfx:img/desktop/depot/gdlefd.img", colorTable));
    m_btnLeft.setDisabledTexture(gfx::Image::load("gfx:img/desktop/depot/gdlefu.img", colorTableDisabled));
    m_btnLeft.setPosition(13, 8);
    m_btnLeft.disable();

    m_btnRight.setTexture(gfx::Image::load("gfx:img/desktop/depot/gdrigu.img", colorTable));
    m_btnRight.setPressedTexture(gfx::Image::load("gfx:img/desktop/depot/gdrigd.img", colorTable));
    m_btnRight.setDisabledTexture(gfx::Image::load("gfx:img/desktop/depot/gdrigu.img", colorTableDisabled));
    m_btnRight.setPosition(493, 8);
    m_btnRight.disable();

    m_chkGreen = gfx::Image::load("gfx:img/desktop/depot/chkgre.img", colorTable);
    m_chkRed = gfx::Image::load("gfx:img/desktop/depot/chkred.img", colorTable);
    m_textureFrame.fromImage(gfx::Image::load("gfx:img/desktop/depot/iframe.img", colorTable));

    m_sndButton.load("sfx:snd/desktop/button.pcm");
}

void ItemList::addItem(const gfx::Image &icon)
{
    addItem(icon, false, false);
}

void ItemList::addItem(const gfx::Image &icon, bool red, bool green)
{
    int x = 54*m_items;
    m_texture.update(x, 0, icon);
    if (m_textureChecks.isValid())
    {
        if (green)
            m_textureChecks.update(x + 4, 4, m_chkGreen);
        if (red)
            m_textureChecks.update(x + 4, 4, m_chkRed);
    }
    m_items++;
    updateButtons();
}

void ItemList::selectItem(int index)
{
    m_selectedItem = index;
    if (m_firstItem + 8 <= m_selectedItem)
    {
        m_firstItem = m_selectedItem - 7;
        m_offset = -54;
        m_time.restart();
        updateButtons();
    }
}

void ItemList::clear()
{
    m_texture.createEmpty(m_texture.width(), m_texture.height(), gfx::Texture::RGBA);
    if (m_textureChecks.isValid())
        m_textureChecks.createEmpty(m_textureChecks.width(), m_textureChecks.height(), gfx::Texture::RGBA);
    m_items = 0;
    m_firstItem = 0;
    m_selectedItem = -1;
    m_offset = 0;
    updateButtons();
}

void ItemList::draw()
{
    const int scrollTime = 50;
    if (m_offset != 0) {
        if (m_time.elapsed() < scrollTime) {
            m_offset = (m_offset > 0 ? 54 : -54)*(scrollTime - m_time.elapsed())/scrollTime;
        } else {
            m_offset = 0;
        }
    }

    m_texture.draw(54, 0, QRectF(m_firstItem*54 + m_offset, 0, 435, 48));
    if (m_textureChecks.isValid())
        m_textureChecks.draw(54, 0, QRectF(m_firstItem*54 + m_offset, 0, 435, 48));

    int x = (m_selectedItem - m_firstItem)*54 - m_offset;
    if (x > -m_textureFrame.width() && x < 0)
        m_textureFrame.draw(54, 0, QRectF(m_textureFrame.width() + x, 0, -x, m_textureFrame.height()));
    else if (x >= 0 && x < 435 - m_textureFrame.width())
        m_textureFrame.draw(54 + x, 0);
    else if (x >= 435 - m_textureFrame.width() && x < 435)
        m_textureFrame.draw(54 + x, 0, QRectF(0, 0, 435 - x, m_textureFrame.height()));
}

void ItemList::scrollLeft()
{
    if (m_firstItem > 0)
    {
        m_firstItem--;
        m_offset = 54;
        m_time.restart();
        m_sndButton.play();
    }
    updateButtons();
}

void ItemList::scrollRight()
{
    if (m_firstItem + 8 < m_items)
    {
        m_firstItem++;
        m_offset = -54;
        m_time.restart();
        m_sndButton.play();
    }
    updateButtons();
}

bool ItemList::mousePressEvent(const QPoint &pos, Qt::MouseButton button)
{
    QRect rect = mapToGlobal(QRect(54, 0, 435, 48));
    if (button == Qt::LeftButton && rect.contains(pos))
    {
        int index = m_firstItem + (pos.x() - rect.x())/54;
        if (index >= 0 && index < m_items)
        {
            m_funcClicked(index);
            return true;
        }
    }

    return false;
}

void ItemList::updateButtons()
{
    m_btnLeft.setEnabled(m_firstItem > 0);
    m_btnRight.setEnabled(m_firstItem + 8 < m_items);
}

} // namespace ui
