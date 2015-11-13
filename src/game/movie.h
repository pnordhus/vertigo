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

#ifndef GAME_MOVIE_H
#define GAME_MOVIE_H


#include "renderer.h"
#include "sfx/stream.h"
#include "gfx/texture.h"
#include "gfx/video.h"
#include "gfx/font.h"

#include <functional>

namespace game {


class Movie : public Renderer
{
public:
    enum MovieType
    {
        MovieFilm = 0,
        MovieStation = 1,
        MovieTransit = 2,
    };

public:
    Movie(const QString &filename, MovieType type, const QString &textLarge = "", const QString &textSmall = "");

public:
    void play(std::function<void()> &&funcFinished);
    void setRect(const QRect &rect);

private:
    void activate();
    void deactivate();
    void draw();
    void keyPressEvent(QKeyEvent *);
    void mousePressEvent(QMouseEvent *);

private:
    QString m_filename;
    MovieType m_type;
    QString m_textLarge;
    QString m_textSmall;

    gfx::Texture m_logo;
    gfx::Font m_fontLarge;
    gfx::Font m_fontSmall;

    gfx::Video m_video;
    gfx::Texture m_texture;
    sfx::Stream m_stream;
    bool m_pause;
    std::function<void()> m_funcFinished;
};


} // namespace game


#endif // GAME_MOVIE_H
