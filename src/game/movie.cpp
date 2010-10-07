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

#include "movie.h"
#include <QApplication>
#include <QKeyEvent>


namespace game {


Movie::Movie()
{

}


void Movie::play(const QString &filename)
{
    m_video.open(filename);

    m_texture.createEmpty(m_video.width(), m_video.height() + 4, gfx::Texture::RGBA);

    QImage line(m_texture.width(), 1, QImage::Format_Indexed8);
    line.setColorTable(QVector<QRgb>() << qRgb(44, 44, 51));
    line.fill(0);

    m_texture.update(0, 0, line);
    m_texture.update(0, m_texture.height() - 1, line);
}


bool Movie::showCursor() const
{
    return false;
}


void Movie::draw()
{
    if (m_video.atEnd()) {
        emit finished();
        return;
    }

    if (m_stream.queued() < 2 || m_stream.processed() > 0) {
        const QByteArray audio = m_video.getAudio();
        if (!audio.isEmpty())
            m_stream.add(audio);

        if (!m_stream.isPlaying())
            m_stream.play();
    }

    QImage image = m_video.getFrame();
    if (!image.isNull())
        m_texture.update(0, 2, image);

    setupOrthographicMatrix(m_texture.width(), m_texture.height());
    m_texture.draw();
}


void Movie::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape)
        emit finished();
}


void Movie::mousePressEvent(QMouseEvent *event)
{

}


} // namespace game
