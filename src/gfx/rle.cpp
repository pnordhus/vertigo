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

#include "rle.h"


namespace gfx {


qint32 RLE::readValue(QDataStream &stream)
{
    if (m_bitsRead < 16) {
        quint16 v;
        stream >> v;
        m_value = (m_value << 16) + v;
        m_bitsRead += 16;
    }

    Q_ASSERT(m_bitsRead >= m_bits);

    const qint32 value = (m_value >> (m_bitsRead - m_bits)) & m_mask;
    m_bitsRead -= m_bits;
    return value;
}


void RLE::increaseBits()
{
    if (m_entries.size() + 256 + 3 > int(m_mask)) {
        m_value = 0;
        m_bitsRead = 0;
        m_bits += 1;
        m_mask = (m_mask << 1) + 1;
    }
}


bool RLE::addEntry(qint32 value, QByteArray &unpacked)
{
    if (value < 3)
        return false;

    value -= 3;

    Entry newEntry;
    newEntry.pos = unpacked.size();

    if (value < 256) {
        unpacked.append(value);
        newEntry.length = 2;
    } else {
        value -= 256;
        if (value >= m_entries.size())
            return false;

        const Entry &entry = m_entries[value];
        for (unsigned int i = 0; i < entry.length; i++)
            unpacked.append(unpacked[entry.pos + i]);

        newEntry.length = entry.length + 1;
    }

    m_entries.append(newEntry);
    return true;
}


QByteArray RLE::decode(const QByteArray &input)
{
    qint32 size;
    qint32 sizeUnpacked;

    QDataStream stream(input);
    stream.setByteOrder(QDataStream::LittleEndian);
    stream >> size >> sizeUnpacked;
    stream.setByteOrder(QDataStream::BigEndian);

    m_value = 0;
    m_bitsRead = 0;
    m_bits = 9;
    m_mask = 0x1ff;
    m_entries.clear();

    QByteArray unpacked;
    unpacked.reserve(sizeUnpacked);

    while (unpacked.size() < sizeUnpacked) {
        const qint32 value = readValue(stream);
        if (!addEntry(value, unpacked)) {
            qDebug("Invalid video data!");
            return QByteArray();
        }
        increaseBits();
    }

    return unpacked;
}


QImage RLE::decodeImage(const QByteArray &data, int width, int height, const QVector<QRgb> &colorTable)
{
    QDataStream stream(data);
    stream.setByteOrder(QDataStream::LittleEndian);

    QImage image(width, height, QImage::Format_ARGB32);

    for (int y = 0; y < height; y++) {
        int x = 0;
        while (x < width && !stream.atEnd()) {
            quint16 transparent = 0;
            quint16 opaque = 0;
            stream >> transparent >> opaque;

            for (uint i = 0; i < transparent; i++)
                image.setPixel(x++, y, qRgba(0, 0, 0, 0));

            for (uint i = 0; i < opaque; i++) {
                quint8 index;
                stream >> index;
                image.setPixel(x++, y, colorTable[index]);
            }
        }
    }

    return image;
}


QImage RLE::decodeImage(const QByteArray &data, int width, int height)
{
    QDataStream stream(data);
    stream.setByteOrder(QDataStream::LittleEndian);

    QImage image(width, height, QImage::Format_ARGB32);

    for (int y = 0; y < height; y++) {
        int x = 0;
        while (x < width && !stream.atEnd()) {
            quint16 transparent = 0;
            quint16 opaque = 0;
            stream >> transparent >> opaque;

            for (uint i = 0; i < transparent; i++)
                image.setPixel(x++, y, qRgba(0, 0, 0, 0));

            for (uint i = 0; i < opaque; i++) {
                quint16 value;
                stream >> value;

                const quint8 r = (value >> 8) & 0xf8;
                const quint8 g = (value >> 3) & 0xfc;
                const quint8 b = (value << 3) & 0xf8;

                image.setPixel(x++, y, qRgb(r, g, b));
            }
        }
    }

    return image;
}


} // namespace gfx
