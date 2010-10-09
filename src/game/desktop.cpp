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

#include "desktop.h"
#include "gfx/image.h"
#include "gfx/font.h"
#include "txt/desfile.h"
#include <QStringList>


namespace game {


Desktop::Desktop(const QString &name)
{
    util::ColorTable colorTable("gfx:pal/gui/border.pal");

    gfx::Font fontSmall("gfx:fnt/dpsmall.fnt", colorTable);
    gfx::Font fontMedium("gfx:fnt/dpmedium.fnt", colorTable);
    gfx::Font fontLarge("gfx:fnt/dplarge.fnt", colorTable);

    txt::DesFile file("dat:world/" + name + ".des");

    file.beginGroup("Station");

    const QString background = file.value("BackGround").toString();
    const QString backgroundSound = file.value("Sound").toString();
    const QString nameSound = "st" + file.value("SFV").toString().right(4);
    m_background.fromImage(gfx::Image::loadPCX("gfx:pic/bground/" + background + ".pcx"));
    m_backgroundSound.load("sfx:snd/bground/" + backgroundSound + ".pcl", "sfx:snd/bground/" + backgroundSound + ".pcr");
    m_nameSound.load("sfx:snd/names/" + nameSound + ".pcm");

    m_lblBackground.setTexture(m_background);
    setRootWidget(&m_lblBackground);

    ui::Label *label;

    label = new ui::Label;
    label->setFont(fontLarge);
    label->setPosition(8, 8);
    label->setText(file.value("Name").toString());
    m_lblBackground.addChild(label);

    label = new ui::Label;
    label->setFont(fontSmall);
    label->setPosition(8, 10 + fontLarge.height());
    label->setText(file.value("Description").toStringList().join(", "));
    m_lblBackground.addChild(label);

    m_btnNotebook = new ui::Button;
    m_btnNotebook->setTexture(gfx::Image::loadPCX("gfx:pic/notebook/nbklein.pcx"));
    m_btnNotebook->setPosition(572, 424);
    connect(m_btnNotebook, SIGNAL(clicked()), SLOT(showNotebook()));
    m_lblBackground.addChild(m_btnNotebook);

    file.endGroup();

    foreach (const QString &section, file.childGroups().filter("MiniMovie")) {
        file.beginGroup(section);

        const QString name = file.value("Name").toString();
        const QString type = file.value("Type").toString().toLower();

        Video *video = new Video;
        video->video.open("gfx:mvi/desktop/" + name + ".mvi");
        video->x = file.value("X1").toInt();
        video->y = file.value("Y1").toInt();
        video->rndMax = 0;

        if (type == "loop")
            video->video.playLoop();
        else if (type == "rnd1")
            video->rndMax = 15000;
        else if (type == "rnd2")
            video->rndMax = 30000;

        video->time = float(qrand()) / RAND_MAX * video->rndMax;

        m_videos.append(video);

        file.endGroup();
    }
}


Desktop::~Desktop()
{
    qDeleteAll(m_videos);
}


void Desktop::activate()
{
    m_time.start();
    m_backgroundSound.playLoop();
    m_nameSound.play();
}


void Desktop::deactivate()
{
    m_backgroundSound.stop();
}


void Desktop::draw()
{
    const quint32 time = m_time.restart();

    foreach (Video *video, m_videos) {
        if (video->rndMax > 0 && !video->video.isPlaying()) {
            video->time -= time;
            if (video->time < 0) {
                video->video.play();
                video->time = float(qrand()) / RAND_MAX * video->rndMax;
            }
        }

        QImage frame = video->video.getFrame();
        if (!frame.isNull())
            m_background.update(video->x, video->y, frame);
    }

    Menu::draw();
}


void Desktop::showNotebook()
{
    m_btnNotebook->hide();
}


} // namespace game
