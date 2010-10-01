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
#include "gfx/texture.h"
#include "gfx/video.h"


namespace game {


class Movie : public Renderer
{
    Q_OBJECT

public:
    Movie();

signals:
    void finished();

public:
    void play(const QString &filename);
    bool showCursor() const;

private:
    void draw();
    void keyPressEvent(QKeyEvent *);
    void mousePressEvent(QMouseEvent *);

private:
    gfx::Video m_video;
    gfx::Texture m_texture;
};


} // namespace game


#endif // GAME_MOVIE_H
