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

#ifndef GFX_RLE_H
#define GFX_RLE_H


#include <QImage>


namespace gfx {


class RLE
{
public:
    QByteArray decode(const QByteArray & input);
    static QImage decodeImage(const QByteArray &data, int width, int height, const QRgb &color);
    static QImage decodeImage(const QByteArray &data, int width, int height, const QVector<QRgb> &colorTable);
    static QImage decodeImage(const QByteArray &data, int width, int height);

private:
    inline qint32 readValue(QDataStream &stream);
    inline void increaseBits();
    inline bool addEntry(qint32 value, QByteArray &unpacked);

private:
    struct Entry
    {
        quint32 pos;
        quint32 length;
    };

    QVector<Entry> m_entries;
    quint32 m_value;
    quint8 m_bitsRead;
    quint8 m_bits;
    quint16 m_mask;
};


} // namespace gfx


#endif // GFX_RLE_H
