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
#include "ui/label.h"

#include <functional>

namespace game {


class Movie : public Renderer
{
public:
    Movie(std::function<void()> &&funcFinished);

public:
    void play(const QString &filename);
    void setRect(const QRect &rect);

private:
    void activate();
    void deactivate();
    void draw();
    void keyPressEvent(QKeyEvent *);
    void mousePressEvent(QMouseEvent *);

private:
    ui::Label m_logo;
    gfx::Video m_video;
    gfx::Texture m_texture;
    sfx::Stream m_stream;
    bool m_pause;
    std::function<void()> m_funcFinished;
};


} // namespace game


#endif // GAME_MOVIE_H
