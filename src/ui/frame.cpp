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


namespace ui {


Frame::Frame(ui::Widget *parent) :
    Label(parent)
{

}


void Frame::setupFrame(const QSize &size, const QString &title, bool closable)
{
    const gfx::ColorTable colorTable("gfx:pal/gui/border.pal");
    const gfx::Font fontMedium("gfx:fnt/dpmedium.fnt", colorTable);

    ui::Label *lblTitle = new ui::Label(this);
    lblTitle->setFont(fontMedium);
    lblTitle->setText(title);
    lblTitle->setAlignment(ui::Label::AlignHCenter);
    lblTitle->setPosition(0, 1);
    lblTitle->setWidth(size.width());

    QImage left = gfx::Image::load("gfx:img/desktop/gui/borl.img", colorTable);
    QImage right = gfx::Image::load("gfx:img/desktop/gui/borr.img", colorTable);
    QImage bottom = gfx::Image::load("gfx:img/desktop/gui/borb1.img", colorTable);

    gfx::Texture texture;
    texture.createEmpty(size, gfx::Texture::RGB);

    const int titleLeft = (size.width() - 170) / 2;
    const int titleRight = titleLeft + 170;

    {
        const int total = size.width() - 30;
        int beg = 13;

        updateBorder(texture, colorTable, beg, 9);
        beg += (total % 45) / 2;
        beg += updateBorder(texture, colorTable, beg, 1);
        beg += updateBorder(texture, colorTable, beg, 2);

        beg += ((titleRight - beg) / 45) * 45;
        beg += updateBorder(texture, colorTable, beg, 5);
        if (13 + total - beg >= 45)
            beg += updateBorder(texture, colorTable, beg, 2);
        updateBorder(texture, colorTable, beg - 1, 9);
    }

    texture.update(                0,                  0, left.copy(0, 0, left.width(), size.height()));
    texture.update(size.width() -  7,                  0, right.copy(0, 0, right.width(), size.height()));
    texture.update(                0, size.height() -  7, bottom.copy(0, 0, size.width(), bottom.height()));
    texture.update(                0,                  0, gfx::Image::load("gfx:img/desktop/gui/edgetl.img", colorTable));
    texture.update(size.width() - 12,                  0, gfx::Image::load("gfx:img/desktop/gui/edgetr.img", colorTable));
    texture.update(size.width() - 12, size.height() - 17, gfx::Image::load("gfx:img/desktop/gui/edgebr.img", colorTable));
    texture.update(                0, size.height() - 17, gfx::Image::load("gfx:img/desktop/gui/edgebl.img", colorTable));

    texture.update(titleLeft, 0, gfx::Image::load("gfx:img/desktop/gui/bortw.img", colorTable));

    if (closable) {
        ui::Button *buttonClose = new ui::Button(this);
        buttonClose->setTexture(gfx::Image::load("gfx:img/desktop/gui/gdexitu.img", colorTable));
        buttonClose->setPressedTexture(gfx::Image::load("gfx:img/desktop/gui/gdexitd.img", colorTable));
        connect(buttonClose, SIGNAL(clicked()), SIGNAL(close()));
    }

    setTexture(texture);
    setSize(size);
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


} // namespace ui
