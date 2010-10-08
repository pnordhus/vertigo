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
#include "txt/desfile.h"
#include <QStringList>


namespace game {


Desktop::Desktop(const QString &name)
{
    txt::DesFile file("dat:world/" + name + ".des");

    const QString background = file.value("Station/BackGround").toString();
    m_background.fromImage(gfx::Image::loadPCX("gfx:pic/bground/" + background + ".pcx"));

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
            video->rndMax = 7000;
        else if (type == "rnd2")
            video->rndMax = 14000;

        video->time = 500 + float(qrand()) / RAND_MAX * video->rndMax;

        m_videos.append(video);

        file.endGroup();
    }
}


Desktop::~Desktop()
{
    qDeleteAll(m_videos);
}


void Desktop::draw()
{
    setupOrthographicMatrix(640, 480);

    const quint32 time = m_time.restart();

    foreach (Video *video, m_videos) {
        if (video->rndMax > 0 && !video->video.isPlaying()) {
            video->time -= time;
            if (video->time < 0) {
                video->video.play();
                video->time = 500 + float(qrand()) / RAND_MAX * video->rndMax;
            }
        }

        QImage frame = video->video.getFrame();
        if (!frame.isNull())
            m_background.update(video->x, video->y, frame);
    }

    m_background.draw();
}


void Desktop::keyPressEvent(QKeyEvent *)
{

}


void Desktop::mousePressEvent(QMouseEvent *)
{

}


void Desktop::mouseReleaseEvent(QMouseEvent *)
{

}


void Desktop::mouseMoveEvent(QMouseEvent *)
{

}


} // namespace game
