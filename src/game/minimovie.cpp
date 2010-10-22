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

#include "minimovie.h"
#include "gfx/image.h"
#include <QStringList>


namespace game {


MiniMovie::MiniMovie(const QString &path) :
    m_path(path)
{

}


MiniMovie::~MiniMovie()
{
    qDeleteAll(m_videos);
}


void MiniMovie::load(txt::DesFile &file)
{
    foreach (const QString &section, file.childGroups().filter(QRegExp("^MiniMovie"))) {
        file.beginGroup(section);

        const QString name = file.value("Name").toString();
        const QString type = file.value("Type").toString().toLower();

        Video *video = new Video;
        video->video.open(QString("%1/%2.mvi").arg(m_path, name));
        video->x = file.value("X1").toInt();
        video->y = file.value("Y1").toInt();
        video->rndMax = 0;
        video->oneShot = false;

        if (type == "loop")
            video->video.playLoop();
        else if (type == "rnd1")
            video->rndMax = 15000;
        else if (type == "rnd2")
            video->rndMax = 30000;
        else if (type == "oneshot")
            video->oneShot = true;

        video->time = float(qrand()) / RAND_MAX * video->rndMax;

        m_videos.append(video);

        file.endGroup();
    }
}


void MiniMovie::start()
{
    m_time.start();
}


void MiniMovie::update(gfx::Texture texture)
{
    const quint32 time = m_time.restart();
    bool oneshotPlaying = false;
    foreach (Video *video, m_videos) {
        if (video->oneShot)
            oneshotPlaying = video->video.isPlaying();
        if (video->rndMax > 0 && !video->video.isPlaying()) {
            video->time -= time;
            if (video->time < 0) {
                video->video.play();
                video->time = float(qrand()) / RAND_MAX * video->rndMax;
            }
        }

        gfx::Image frame = video->video.getFrame();
        if (!frame.isNull()) {
            frame.setColorTable(m_colorTable);
            texture.update(video->x, video->y, frame);
        }
    }

    if (oneshotPlaying) {
        foreach (Video *video, m_videos) {
            if (video->oneShot) {
                if (!video->video.isPlaying())
                    emit videoFinished();
                break;
            }
        }
    }
}


void MiniMovie::setColorTable(const gfx::ColorTable &colorTable)
{
    m_colorTable = colorTable;
}


void MiniMovie::playOneshot()
{
    foreach (Video *video, m_videos) {
        if (video->oneShot)
            video->video.play();
    }
}


void MiniMovie::stopOneshot()
{
    foreach (Video *video, m_videos) {
        if (video->oneShot)
            video->video.stop();
    }
    emit videoFinished();
}


} // namespace game
