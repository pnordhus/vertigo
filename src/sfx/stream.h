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

#ifndef SFX_STREAM_H
#define SFX_STREAM_H


#include <QByteArray>


namespace sfx {


class Stream
{
public:
    Stream();
    ~Stream();

public:
    void add(const QByteArray &data);
    void play();
    void pause();
    bool isPlaying() const;
    quint32 queued() const;
    quint32 processed() const;

private:
    Q_DISABLE_COPY(Stream);
    void clearBuffers();

private:
    quint32 m_source;
};


} // namespace sfx


#endif // SFX_STREAM_H
