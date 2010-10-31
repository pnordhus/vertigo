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
#include "gfx/colortable.h"
#include "gfx/image.h"
#include "ui/button.h"


namespace game {


Depot::Depot() :
    m_flipped(false)
{
    const gfx::ColorTable colorTable("gfx:pal/gui/border.pal");
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

    label = new ui::Label(m_backgroundLabel);
    label->setFont(gfx::Font::Large);
    label->setPosition(16, 75);
    label->setText(Chapter::get()->boat()->name());

    m_videoFlip1.open(QString("gfx:mvi/sflip/%1").arg(Chapter::get()->boat()->flipMovie1()));
    m_videoFlip2.open(QString("gfx:mvi/sflip/%1").arg(Chapter::get()->boat()->flipMovie2()));
    m_videoFlip1.setFrameRate(20);
    m_videoFlip2.setFrameRate(20);
    m_videoFlip2.play();

    Q_ASSERT(m_videoFlip1.width() == m_videoFlip2.width());
    Q_ASSERT(m_videoFlip1.height() == m_videoFlip2.height());
    m_flip.createEmpty(m_videoFlip1.width(), m_videoFlip1.height(), gfx::Texture::RGBA);

    label = new ui::Label(m_backgroundLabel);
    label->setTexture(m_flip);
    label->setPosition(16, 85);

    m_btnFlip = new ui::Button(m_backgroundLabel);
    m_btnFlip->setTexture(gfx::Image::load("gfx:img/desktop/depot/gdfliu.img", colorTable));
    m_btnFlip->setPosition(304, 243);
    m_btnFlip->setOffset(2);
    connect(m_btnFlip, SIGNAL(clicked()), SLOT(flip()));

    //gdlefd.img    Arrow left disabled
    //gdlefu.img    Arrow left active
    //gdrigd.img    Arrow right disabled
    //gdrigu.img    Arrow right active
    //gdflid.img    Flip disabled
    //gdfliu.img    Flip active
    //gdammod.img   Repair disabled (same as active)
    //gdammou.img   Repair active
    //gdbuyd.img    Buy disabled (same as active)
    //gdbuyu.img    Buy active
    //gdtinfd.img   Info disabled
    //gdtinfu.img   Info active
    //gdseld.img    Sell disabled (same as active)
    //gdselu.img    Sell active
    //chkgre.img    Check green
    //chkred.img    Check red
}


Depot::~Depot()
{
}


void Depot::flip()
{
    m_flipped = !m_flipped;
    if (m_flipped)
        m_videoFlip1.play();
    else
        m_videoFlip2.play();
}


void Depot::draw()
{
    gfx::Image frame;
    if (m_flipped)
        frame = m_videoFlip1.getFrame();
    else
        frame = m_videoFlip2.getFrame();
    if (!frame.isNull())
    {
        gfx::ColorTable colorTable = frame.colorTable();
        colorTable[0] = qRgba(0, 0, 0, 0);
        frame.setColorTable(colorTable);
        m_flip.update(0, 0, frame);
    }

    ui::Label::draw();
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
