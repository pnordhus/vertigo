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
#include "sfx/soundsystem.h"
#include "hud/hud.h"
#include <QApplication>
#include <QGLContext>
#include <QKeyEvent>


namespace game {


Briefing::Briefing() :
    m_state(Init),
    m_toggleState(false)
{
    hideCursor();

    const gfx::ColorTable colorTableArrow("gfx:pal/connect/areapoin.pal");
    const gfx::ColorTable colorTable("gfx:pal/notebook/notebook.pal");
    m_font.load("gfx:fnt/nfont1a.fnt", colorTable, true);

    m_backgroundSound.load("sfx:snd/vfx/inter.pcm", 11025);
    m_openSound.load("sfx:snd/desktop/areamap.pcm");
    m_woopSound.load("sfx:snd/desktop/woop.pcm", 33075);
    m_woopSound.setVolume(0.1f);

    QImage image(640, 480, QImage::Format_Indexed8);
    image.fill(0);
    image.setColorTable(QVector<QRgb>() << qRgba(0, 0, 0, 128));
    m_lblMain.setTexture(image);

    m_lblMap = new ui::Label(&m_lblMain);
    m_lblMap->setPosition(304, 32);
    m_lblMap->setTexture(gfx::Image::load(QString("gfx:pic/area/%1.r16").arg(Chapter::get()->area()->map()), 304, 284, true));

    m_lblArrow = new ui::Label(m_lblMap);
    m_lblArrow->setPosition(Chapter::get()->mission()->pos() - QPoint(7, 7));
    m_lblArrow->setTexture(gfx::Image::load("gfx:img/connect/areapoin.img", colorTableArrow));
    m_lblArrow->setVisible(false);
}

void Briefing::activate()
{
    m_backgroundSound.playLoop();
    m_openSound.play();
}


void Briefing::deactivate()
{
    m_backgroundSound.stop();
}


void Briefing::setRect(const QRect &rect)
{
    Chapter::get()->hud()->setRect(rect);
    Renderer::setRect(rect);
}


void Briefing::draw()
{
    Chapter::get()->hud()->draw();
    setupGL(false);
    m_lblMain.doDraw();

    switch (m_state)
    {
    case Text:
    case Targets:
    case Hints:
        if (m_time.elapsed() < 75)
            return;
        break;
    case Arrow:
    case PressKey:
        if (m_time.elapsed() < 500)
            return;
        m_toggleState = !m_toggleState;
        break;
    }

    m_time.restart();

    ui::Label *label;

    if (m_state == Init)
    {
        m_state = Text;
        m_nextLine = 0;
    }

    if (m_state == Text)
    {
        if (m_nextLine < Chapter::get()->mission()->textB().count())
        {
            label = new ui::Label(&m_lblMain);
            label->setFont(m_font);
            label->setText(Chapter::get()->mission()->textB().at(m_nextLine));
            label->setPosition(0, 64 + m_nextLine*11);
            label->setWidth(304);
            label->setAlignment(ui::Label::AlignHCenter);
            m_woopSound.playInstance();
            m_nextLine++;
        }
        else
        {
            m_state = Targets;
            m_nextLine = -3;
        }
    }

    if (m_state == Targets)
    {
        if (Chapter::get()->mission()->textP().count() > 0 &&
            m_nextLine < Chapter::get()->mission()->textP().count())
        {
            if (m_nextLine >= -2)
            {
                label = new ui::Label(&m_lblMain);
                label->setFont(m_font);
                if (m_nextLine == -2)
                    label->setText(txt::StringTable::get(txt::Briefing_Targets));
                else if (m_nextLine == -1)
                    label->setText(txt::StringTable::get(txt::Briefing_TargetsLine));
                else
                    label->setText(Chapter::get()->mission()->textP().at(m_nextLine));
                label->setPosition(18, 354 + m_nextLine*11);
                label->setWidth(304);
                m_woopSound.playInstance();
            }
            m_nextLine++;
        }
        else
        {
            m_state = Hints;
            m_nextLine = -3;
        }
    }

    if (m_state == Hints)
    {
        if (Chapter::get()->mission()->textS().count() > 0 &&
            m_nextLine < Chapter::get()->mission()->textS().count())
        {
            if (m_nextLine >= -2)
            {
                label = new ui::Label(&m_lblMain);
                label->setFont(m_font);
                if (m_nextLine == -2)
                    label->setText(txt::StringTable::get(txt::Briefing_Hints));
                else if (m_nextLine == -1)
                    label->setText(txt::StringTable::get(txt::Briefing_HintsLine));
                else
                    label->setText(Chapter::get()->mission()->textS().at(m_nextLine));
                label->setPosition(322, 354 + m_nextLine*11);
                label->setWidth(304);
                m_woopSound.playInstance();
            }
            m_nextLine++;
        }
        else
        {
            m_state = Arrow;
            m_nextLine = 0;
            m_eventInit();
        }
    }

    if (m_state == Arrow)
    {
        m_lblArrow->setVisible(m_toggleState);
        m_nextLine++;
        if (m_nextLine > 5)
        {
            m_lblPressKey = new ui::Label(&m_lblMain);
            m_lblPressKey->setFont(m_font);
            m_lblPressKey->setText(txt::StringTable::get(txt::Briefing_PressAnyKey));
            m_lblPressKey->setPosition(0, 454);
            m_lblPressKey->setWidth(640);
            m_lblPressKey->setAlignment(ui::Label::AlignHCenter);

            m_state = PressKey;
        }
    }

    if (m_state == PressKey)
    {
        m_lblArrow->setVisible(m_toggleState);
        m_lblPressKey->setVisible(m_toggleState);
        if (m_toggleState)
            m_woopSound.play();
    }
}

void Briefing::keyPressEvent(QKeyEvent *event)
{
    if (m_state == PressKey) {
        m_eventStart();
    }
}


} // namespace game
