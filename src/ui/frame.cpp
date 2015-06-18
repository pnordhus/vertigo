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

#include "button.h"
#include "frame.h"
#include "gfx/font.h"
#include "gfx/image.h"
#include "sfx/soundsystem.h"

namespace ui {

Frame::Frame(std::function<void()> &&funcClose, ui::Widget *parent) :
    Label(parent),
    m_funcClose(std::move(funcClose)),
    m_lblTitle(this),
    m_btnClose([this]() { closeFrame(); }, this)
{
}

void Frame::setupFrame(const QSize &size, const QString &title, bool closable)
{
    const gfx::ColorTable colorTable("gfx:pal/gui/border.pal");

    m_lblTitle.setFont(gfx::Font::Medium);
    m_lblTitle.setText(title);
    m_lblTitle.setAlignment(ui::Label::AlignHCenter);
    m_lblTitle.setPosition(0, 1);
    m_lblTitle.setWidth(size.width());

    QImage left = gfx::Image::load("gfx:img/desktop/gui/borl.img", colorTable);
    QImage right = gfx::Image::load("gfx:img/desktop/gui/borr.img", colorTable);
    QImage bottom = gfx::Image::load("gfx:img/desktop/gui/borb1.img", colorTable);

    gfx::Texture texture;
    texture.createEmpty(size, gfx::Texture::RGB);

    {
        QList<QImage> images;
        const int total = size.width() - 30;
        static const int id[] = { 1,2,3,3,4,5,2,4,3,1 };
        int sum = 0;
        for (int i = 0; i < 10; i++) {
            if (i == 3 && total < 270)
                continue;

            QImage image = getBorder(colorTable, id[i]);
            if (sum + image.width() > total)
                break;
            sum += image.width();
            images << image;
        }

        int beg = 13;
        updateBorder(texture, colorTable, beg, 9);
        beg += 2 + (total - sum) / 2;

        foreach (QImage image, images) {
            texture.update(beg, 0, image);
            beg += image.width();
        }

        updateBorder(texture, colorTable, beg - 1, 9);
    }

    texture.update(                0,                  0, left.copy(0, 0, left.width(), size.height()));
    texture.update(size.width() -  7,                  0, right.copy(0, 0, right.width(), size.height()));
    texture.update(                0, size.height() -  7, bottom.copy(0, 0, size.width(), bottom.height()));
    texture.update(                0,                  0, gfx::Image::load("gfx:img/desktop/gui/edgetl.img", colorTable));
    texture.update(size.width() - 12,                  0, gfx::Image::load("gfx:img/desktop/gui/edgetr.img", colorTable));
    texture.update(size.width() - 12, size.height() - 17, gfx::Image::load("gfx:img/desktop/gui/edgebr.img", colorTable));
    texture.update(                0, size.height() - 17, gfx::Image::load("gfx:img/desktop/gui/edgebl.img", colorTable));

    texture.update((size.width() - 170) / 2, 0, gfx::Image::load("gfx:img/desktop/gui/bortw.img", colorTable));

    m_btnClose.setVisible(closable);
    m_btnClose.setTexture(gfx::Image::load("gfx:img/desktop/gui/gdexitu.img", colorTable));
    m_btnClose.setPressedTexture(gfx::Image::load("gfx:img/desktop/gui/gdexitd.img", colorTable));

    setTexture(texture);
    setSize(size);

    sfx::SoundSystem::get()->sound(sfx::FrameOpen)->play();
}

int Frame::updateBorder(gfx::Texture texture, const gfx::ColorTable &colorTable, int x, int id)
{
    QImage image = gfx::Image::load(QString("gfx:img/desktop/gui/bort%1.img").arg(id), colorTable);
    if (texture.width() - (x + image.width()) < 0) {
        const int width = texture.width() - x;
        texture.update(x, 0, image.copy(0, 0, width, image.height()));
        return width;
    } else {
        texture.update(x, 0, image);
        return image.width();
    }
}

void Frame::closeFrame()
{
    sfx::SoundSystem::get()->sound(sfx::FrameClose)->play();
    m_funcClose();
}

QImage Frame::getBorder(const gfx::ColorTable &colorTable, int id)
{
    return gfx::Image::load(QString("gfx:img/desktop/gui/bort%1.img").arg(id), colorTable);
}

} // namespace ui
