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

    m_backgroundSound.load("sfx:snd/bground/b04.pcl", "sfx:snd/bground/b04.pcr");

    // TODO: Choose cockpit
    m_background.setTexture(gfx::Image::load("vfx:cockpit/000/cockpit.r16", 640, 480));

    QImage image(640, 480, QImage::Format_Indexed8);
    image.fill(0);
    image.setColorTable(QVector<QRgb>() << qRgba(0, 0, 0, 128));

    ui::Label *label;

    label = new ui::Label(&m_background);
    label->setTexture(image);

    label = new ui::Label(&m_background);
    label->setPosition(304, 32);
    // TODO: Map transparency
    label->setTexture(gfx::Image::load(QString("gfx:pic/area/%1.r16").arg(Chapter::get()->area()->map()), 304, 284));

    setRootWidget(&m_background);
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
