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


namespace game {


Depot::Depot() :
	m_boat(Chapter::get()->boat()),
    m_state(Flipping),
    m_side(1)
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
    label->setText(m_boat->name());

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

    m_btnFlip = new ui::Button(m_backgroundLabel);
    m_btnFlip->setTexture(gfx::Image::load("gfx:img/desktop/depot/gdfliu.img", colorTable));
    m_btnFlip->setPressedTexture(gfx::Image::load("gfx:img/desktop/depot/gdflid.img", colorTable));
    m_btnFlip->setPosition(304, 243);
    connect(m_btnFlip, SIGNAL(clicked()), SLOT(flip()));

    m_btnRepair = new ui::Button(m_backgroundLabel);
    m_btnRepair->setTexture(gfx::Image::load("gfx:img/desktop/depot/gdammou.img", colorTable));
    m_btnRepair->setPressedTexture(gfx::Image::load("gfx:img/desktop/depot/gdammod.img", colorTable));
    m_btnRepair->setPosition(304, 75);

    m_btnLeft1 = new ui::Button(m_backgroundLabel);
    m_btnLeft1->setTexture(gfx::Image::load("gfx:img/desktop/depot/gdlefu.img", colorTable));
    m_btnLeft1->setPressedTexture(gfx::Image::load("gfx:img/desktop/depot/gdlefd.img", colorTable));
    m_btnLeft1->setPosition(21, 16);

    m_btnRight1 = new ui::Button(m_backgroundLabel);
    m_btnRight1->setTexture(gfx::Image::load("gfx:img/desktop/depot/gdrigu.img", colorTable));
    m_btnRight1->setPressedTexture(gfx::Image::load("gfx:img/desktop/depot/gdrigd.img", colorTable));
    m_btnRight1->setPosition(501, 16);

    m_btnLeft2 = new ui::Button(m_backgroundLabel);
    m_btnLeft2->setTexture(gfx::Image::load("gfx:img/desktop/depot/gdlefu.img", colorTable));
    m_btnLeft2->setPressedTexture(gfx::Image::load("gfx:img/desktop/depot/gdlefd.img", colorTable));
    m_btnLeft2->setPosition(21, 301);

    m_btnRight2 = new ui::Button(m_backgroundLabel);
    m_btnRight2->setTexture(gfx::Image::load("gfx:img/desktop/depot/gdrigu.img", colorTable));
    m_btnRight2->setPressedTexture(gfx::Image::load("gfx:img/desktop/depot/gdrigd.img", colorTable));
    m_btnRight2->setPosition(501, 301);

    m_btnBuy = new ui::Button(m_backgroundLabel);
    m_btnBuy->setTexture(gfx::Image::load("gfx:img/desktop/depot/gdbuyu.img", colorTable));
    m_btnBuy->setPressedTexture(gfx::Image::load("gfx:img/desktop/depot/gdbuyd.img", colorTable));
    m_btnBuy->setPosition(507, 243);

    m_btnSell = new ui::Button(m_backgroundLabel);
    m_btnSell->setTexture(gfx::Image::load("gfx:img/desktop/depot/gdselu.img", colorTable));
    m_btnSell->setPressedTexture(gfx::Image::load("gfx:img/desktop/depot/gdseld.img", colorTable));
    m_btnSell->setPosition(507, 75);

    m_btnInfo = new ui::Button(m_backgroundLabel);
    m_btnInfo->setTexture(gfx::Image::load("gfx:img/desktop/depot/gdtinfu.img", colorTable));
    m_btnInfo->setPressedTexture(gfx::Image::load("gfx:img/desktop/depot/gdtinfd.img", colorTable));
    m_btnInfo->setPosition(507, 131);

	for (int i = 0; i < m_boat->mountings().count(); i++)
	{
		Boat::Mounting *mounting = m_boat->mountings().at(i);
		ui::Arrow *arrow = new ui::Arrow(mounting->dir, mounting->pos, true, m_backgroundLabel);
		arrow->setText(mounting->name);
		arrow->setValue(i);
        arrow->hide();
		//connect(arrow, SIGNAL(clicked(int)), SLOT(loadMounting(int)));
		m_mountingArrows << arrow;
	}

    //chkgre.img    Check green
    //chkred.img    Check red
}


Depot::~Depot()
{
}


void Depot::flip()
{
    foreach (ui::Arrow *arrow, m_mountingArrows)
        arrow->hide();

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
}


void Depot::draw()
{
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

    ui::Label::draw();

    switch (m_state)
    {
    case Flipping:
    case Ready:
    case Repair:
        return;
    case Loading:
        if (m_time.elapsed() < 75)
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
                if (mounting->side == m_side)
                {
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
                m_loadingState = List1;
                m_loadingItem = 0;
            }
        }
        if (m_loadingState == List1)
        {
            m_loadingState = List2;
            m_loadingItem = 0;
        }
        if (m_loadingState == List2)
        {
            m_state = Ready;
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
