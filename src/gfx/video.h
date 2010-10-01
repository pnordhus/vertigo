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

#ifndef GFX_VIDEO_H
#define GFX_VIDEO_H


#include <QFile>
#include <QImage>
#include <QTime>
#include "util/colortable.h"


namespace gfx {


class Video
{
public:
    Video();
    Video(const QString &filename);

public:
    void open(const QString &filename);
    QImage getFrame();
    QByteArray getAudio();
    bool atEnd() const;
    quint32 width() const { return m_width; }
    quint32 height() const { return m_height; }

private:
    void loadColorTable(const QByteArray &data);
    void loadVideoFull(const QByteArray &data);
    void loadVideoDiff(const QByteArray &data);
    QImage createImage();

private:
    Q_DISABLE_COPY(Video);

    struct Entry
    {
        enum Type { ColorTable = 5, VideoFull = 1, VideoDiff = 2, AudioLeft = 3, AudioRight = 4 };
        Type type;
        quint32 length;
        quint32 offset;
    };

    QList<Entry> m_entries;
    util::ColorTable m_colorTable;
    QByteArray m_indexMap;
    QByteArray m_frame;
    QFile m_file;

    quint32 m_width;
    quint32 m_height;
    quint32 m_videoPos;

    QTime m_time;
};


} // namespace gfx


#endif // GFX_VIDEO_H
