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


    Movie::Movie(const QString &filename, MovieType type, const QString &textLarge, const QString &textSmall) :
    m_pause(false),
    m_filename(filename),
    m_type(type),
    m_textLarge(textLarge),
    m_textSmall(textSmall),
    m_logo(gfx::Image::load("gfx:img/movie/mvilogo.img", gfx::ColorTable("gfx:pal/movie/moviepal.pal"))),
    m_fontLarge(gfx::Font::Large),
    m_fontSmall(gfx::Font::Small)
{
}


void Movie::activate()
{

}


void Movie::deactivate()
{

}

void Movie::play(std::function<void()> &&funcFinished)
{
    hideCursor();
    m_funcFinished = std::move(funcFinished);

    m_video.open(m_filename);
    m_video.play();

    m_texture.createEmpty(m_video.width(), m_video.height() + 4, gfx::Texture::RGBA);

    QImage line(m_texture.width(), 1, QImage::Format_Indexed8);
    line.setColorTable(QVector<QRgb>() << qRgb(44, 44, 51));
    line.fill(0);

    m_texture.update(0, 0, line);
    m_texture.update(0, m_texture.height() - 1, line);
}


void Movie::setRect(const QRect &rect)
{
    Renderer::setRect(rect);

    setupOrthographicMatrix(m_texture.width(), m_texture.height());
}


void Movie::draw()
{
    if (m_video.atEnd() && !m_stream.isPlaying()) {
        m_funcFinished();
    } else {
        if (!m_pause) {
            if (m_stream.queued() < 2 || m_stream.processed() > 0) {
                const std::vector<char> audio = m_video.getAudio();
                if (!audio.empty()) {
                    m_stream.add(audio);
                    if (!m_stream.isPlaying())
                        m_stream.play();
                }
            }

            QImage image = m_video.getFrame();
            if (!image.isNull())
                m_texture.update(0, 2, image);
        }

        setupGL();
        m_texture.draw();

        int y = static_cast<int>((2 - rectOrtho().y())/rectOrtho().height()*height());
        if (m_type == MovieFilm && y >= 74)
        {
            setupOrthographicMatrix(190, height());
            setupGL(false);
            m_logo.draw(0, (y - 74)/2);
        }
        if (m_type == MovieTransit)
        {
            setupOrthographicMatrix(200, 480);
            setupGL(false);
            m_fontLarge.draw(m_textLarge, Rect(0, y/2*480/height() - 20, 200, -1), true, false);
            m_fontSmall.draw(m_textSmall, Rect(0, y/2*480/height() - 6, 200, -1), true, false);
        }
        if (m_type == MovieStation)
        {
            setupOrthographicMatrix(width()*480/height(), 480);
            setupGL(false);
            m_fontLarge.draw(m_textLarge, Rect(8, 8, -1, -1), false, false);
            m_fontSmall.draw(m_textSmall, Rect(8, 22, -1, -1), false, false);
        }
    }
}


void Movie::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        m_funcFinished();
        return;
    }

    if (event->key() == Qt::Key_Space) {
        m_pause = !m_pause;
        if (m_pause)
            m_stream.pause();
        else
            m_stream.play();
    }
}


void Movie::mousePressEvent(QMouseEvent *event)
{

}


} // namespace game
