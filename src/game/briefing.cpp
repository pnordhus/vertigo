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

#include "briefing.h"
#include "chapter.h"
#include "gfx/colortable.h"
#include "gfx/image.h"
#include "txt/stringtable.h"
#include <QApplication>
#include <QGLContext>
#include <QKeyEvent>


namespace game {


Briefing::Briefing()
{
    hideCursor();

    const gfx::ColorTable colorTableArrow("gfx:pal/connect/areapoin.pal");
    const gfx::ColorTable colorTable("gfx:pal/notebook/notebook.pal");
    m_font.load("gfx:fnt/nfont1a.fnt", colorTable, true);

    m_backgroundSound.load("sfx:snd/vfx/inter.pcm", 11025);

    // TODO: Choose cockpit
    m_background.setTexture(gfx::Image::load("vfx:cockpit/000/cockpit.r16", 640, 480));
    setRootWidget(&m_background);

    QImage image(640, 480, QImage::Format_Indexed8);
    image.fill(0);
    image.setColorTable(QVector<QRgb>() << qRgba(0, 0, 0, 128));
    m_lblMain = new ui::Label(&m_background);
    m_lblMain->setTexture(image);

    m_lblMap = new ui::Label(m_lblMain);
    m_lblMap->setPosition(304, 32);
    // TODO: Map transparency
    m_lblMap->setTexture(gfx::Image::load(QString("gfx:pic/area/%1.r16").arg(Chapter::get()->area()->map()), 304, 284));

    m_lblArrow = new ui::Label(m_lblMap);
    m_lblArrow->setPosition(Chapter::get()->mission()->pos() - QPoint(7, 7));
    m_lblArrow->setTexture(gfx::Image::load("gfx:img/connect/areapoin.img", colorTableArrow));

    ui::Label *label;

    float posY = 64;
    foreach (const QString &textLine, Chapter::get()->mission()->textB())
    {
        label = new ui::Label(m_lblMain);
        label->setFont(m_font);
        label->setText(textLine);
        label->setPosition(0, posY);
        label->setWidth(304);
        label->setAlignment(ui::Label::AlignHCenter);
        posY += 11;
    }

    if (Chapter::get()->mission()->textP().count() > 0)
    {
        posY = 332;
        label = new ui::Label(m_lblMain);
        label->setFont(m_font);
        label->setText(txt::StringTable::get(txt::Briefing_Targets));
        label->setPosition(18, posY);
        label->setWidth(304);
        posY += 11;
        label = new ui::Label(m_lblMain);
        label->setFont(m_font);
        label->setText(txt::StringTable::get(txt::Briefing_TargetsLine));
        label->setPosition(18, posY);
        label->setWidth(304);
        posY += 11;
        foreach (const QString &textLine, Chapter::get()->mission()->textP())
        {
            label = new ui::Label(m_lblMain);
            label->setFont(m_font);
            label->setText(textLine);
            label->setPosition(18, posY);
            label->setWidth(304);
            posY += 11;
        }
    }

    if (Chapter::get()->mission()->textS().count() > 0)
    {
        posY = 332;
        label = new ui::Label(m_lblMain);
        label->setFont(m_font);
        label->setText(txt::StringTable::get(txt::Briefing_Hints));
        label->setPosition(322, posY);
        label->setWidth(304);
        posY += 11;
        label = new ui::Label(m_lblMain);
        label->setFont(m_font);
        label->setText(txt::StringTable::get(txt::Briefing_HintsLine));
        label->setPosition(322, posY);
        label->setWidth(304);
        posY += 11;
        foreach (const QString &textLine, Chapter::get()->mission()->textS())
        {
            label = new ui::Label(m_lblMain);
            label->setFont(m_font);
            label->setText(textLine);
            label->setPosition(322, posY);
            label->setWidth(304);
            posY += 11;
        }
    }

    label = new ui::Label(m_lblMain);
    label->setFont(m_font);
    label->setText(txt::StringTable::get(txt::Briefing_PressAnyKey));
    label->setPosition(0, 454);
    label->setWidth(640);
    label->setAlignment(ui::Label::AlignHCenter);
}

void Briefing::activate()
{
    m_backgroundSound.playLoop();
}


void Briefing::deactivate()
{
    m_backgroundSound.stop();
}

void Briefing::keyPressEvent(QKeyEvent *event)
{
    emit startEngine();
}


} // namespace game
