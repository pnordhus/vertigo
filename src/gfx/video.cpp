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

#include "video.h"

#include <cstring>

namespace gfx {

Video::Video()
{

}

Video::Video(const QString &filename)
{
    open(filename);
}

void Video::open(const QString &filename)
{
    m_playing = false;
    m_looping = false;
    m_videoPos = 0;
    m_nextVideoPos = 0;
    m_audioPos = 0;
    m_lastAudioPos = 0;
    m_entries.clear();
    m_frame.clear();

    m_file = util::File(QString(filename).replace(":", "/").toUtf8().constData());

    std::uint32_t totalFrames;
    std::uint32_t audioFrames;
    std::uint32_t videoFrames;
    std::uint32_t hasAudio;
    std::uint32_t channels;
    std::uint32_t sampleRate;

    m_file.skipBytes(8);
    m_file >> m_width >> m_height;
    m_file >> totalFrames >> audioFrames >> videoFrames;
    m_file >> m_frameRate;
    m_file >> hasAudio >> channels >> sampleRate;
    m_file.skipBytes(4);

    m_hasAudio = hasAudio;

    const std::uint32_t dataOffset = ((13 * sizeof(std::uint32_t) + totalFrames * 4 * sizeof(std::uint32_t) - 1) / 2048 + 1) * 2048;

    std::uint32_t total = 0;
    for (unsigned int i = 0; i < totalFrames; i++) {
        std::uint32_t type;
        std::uint32_t unknown;
        std::uint32_t length;
        std::uint32_t offset;
        m_file >> type >> unknown >> length >> offset;

        if (offset == 0)
            offset = dataOffset + total;
        else
            offset += dataOffset;

        Entry entry;
        entry.length = length;
        entry.offset = offset;
        entry.type = Entry::Type(type);

        m_entries.emplace_back(entry);

        total += length;
    }

    m_time.start();
}

void Video::play()
{
    m_playing = true;
    m_looping = false;
    reset();
}

void Video::playLoop()
{
    m_playing = true;
    m_looping = true;
    reset();
}

void Video::stop()
{
    m_playing = false;
    m_looping = false;
    reset();
}

void Video::setFrameRate(int frameRate)
{
    m_frameRate = frameRate;
}

void Video::reset()
{
    m_videoPos = 0;
    m_nextVideoPos = 0;
    m_frame.clear();
}

Image Video::getFrame()
{
    if (!m_playing)
        return QImage();

    bool gotFrame = false;

    if (atEnd()) {
        if (m_looping) {
            reset();
        } else {
            m_playing = false;
            return QImage();
        }
    }

    if (m_time.elapsed() < 1000.0f / m_frameRate)
        return QImage();

    m_time.restart();

    while (!atEnd() && !gotFrame) {
        const Entry &entry = m_entries[m_videoPos++];

        m_file.seek(entry.offset);

        switch (entry.type) {
        case Entry::ColorTable:
            loadColorTable(entry.length);
            if (m_hasAudio)
                gotFrame = true;
            break;

        case Entry::VideoFull:
            loadVideoFull(entry.length);
            gotFrame = !m_frame.empty();
            break;

        case Entry::VideoDiff:
            loadVideoDiff(entry.length);
            gotFrame = true;
            break;

        default:
            break;
        }

        if (m_videoPos < m_nextVideoPos)
            gotFrame = false;
    }

    if (gotFrame)
        return createImage();

    return QImage();
}

QByteArray Video::getAudio()
{
    if (!m_playing)
        return QByteArray();

    const std::uint32_t lastAudioPos = m_lastAudioPos;
    m_lastAudioPos = m_audioPos;

    bool gotAudioLeft = false;
    bool gotAudioRight = false;

    QByteArray data;
    data.resize(22050 * 2);

    while (m_audioPos < m_entries.size() && (!gotAudioLeft || !gotAudioRight)) {
        const Entry &entry = m_entries[m_audioPos++];

        m_file.seek(entry.offset);

        switch (entry.type) {
        case Entry::AudioLeft:
            mergeChannel(data, entry.length, 0);
            gotAudioLeft = true;
            break;

        case Entry::AudioRight:
            mergeChannel(data, entry.length, 1);
            gotAudioRight = true;
            break;

        default:
            break;
        }
    }

    if (gotAudioLeft && gotAudioRight) {
        if (lastAudioPos < m_videoPos) {
            bool colorTableChanged = false;
            for (int i = m_videoPos; i >= int(lastAudioPos); i--) {
                if (m_entries[i].type == Entry::ColorTable) {
                    colorTableChanged = true;
                    break;
                }
            }

            m_videoPos = lastAudioPos;
            if (colorTableChanged) {
                while (m_entries[m_videoPos].type != Entry::ColorTable)
                    m_videoPos--;
            } else {
                while (m_entries[m_videoPos].type != Entry::VideoFull)
                    m_videoPos--;
            }
        }

        if (lastAudioPos > m_videoPos)
            m_nextVideoPos = lastAudioPos;

        return data;
    }

    return QByteArray();
}

void Video::mergeChannel(QByteArray &data, int size, int channelIndex)
{
    ASSERT(data.size() >= 2 * size);
    for (std::size_t i = 0; i < size; ++i) {
        data.data()[i * 2 + channelIndex] = m_file.getChar() + 0x80;
    }
}

bool Video::atEnd() const
{
    return m_videoPos >= std::uint32_t(m_entries.size());
}

void Video::loadColorTable(int size)
{
    m_colorTable.load(m_file);
    if (size > 256 * 3) {
        m_indexMap.resize(size - 256 * 3);
        m_file.read(m_indexMap.data(), m_indexMap.size());
    }

    std::memset(m_frame.data(), 0, m_frame.size());
}

void Video::loadVideoFull(int size)
{
    decode(size, m_frame);
}

void Video::loadVideoDiff(int size)
{
    std::uint32_t sizeIndices;
    std::uint32_t sizeIndicesUnpacked;
    std::uint32_t sizeBitmap;
    std::uint32_t sizeBitmapUnpacked;
    m_file >> sizeIndices >> sizeIndicesUnpacked >> sizeBitmap >> sizeBitmapUnpacked;

    std::vector<char> indices;
    indices.resize(sizeIndicesUnpacked);
    if (sizeIndices == sizeIndicesUnpacked) {
        m_file.read(indices.data(), indices.size());
    } else {
        decode(sizeIndices, indices);
    }

    std::vector<char> bitmap;
    bitmap.resize(sizeBitmapUnpacked);
    if (sizeBitmap == sizeBitmapUnpacked) {
        m_file.read(bitmap.data(), bitmap.size());
    } else {
        decode(sizeBitmap, bitmap);
    }

    std::uint32_t num = 0;
    std::uint32_t pos = 0;
    for (unsigned int i = 0; i < sizeBitmapUnpacked; i++) {
        for (int j = 7; j >= 0; j--) {
            if ((bitmap[i] >> j) & 0x01) {
                m_frame[pos] = indices[num];
                num++;
            }
            pos++;
        }
    }
}

void Video::decode(int size, std::vector<char> &output)
{
    const std::size_t startOffset = m_file.position();
    std::int32_t sizeCompressed;
    std::int32_t sizeDecompressed;
    m_file >> sizeCompressed >> sizeDecompressed;

    output.reserve(sizeDecompressed);
    VERIFY(decompressLZW(size - 2 * sizeof(std::int32_t), output));
    m_file.seek(startOffset + size);
}

bool Video::decompressLZW(int size, std::vector<char> &out)
{
    struct DictEntry
    {
        std::size_t prev;
        char c;
        std::size_t length;
    };

    std::vector<DictEntry> dictionary;
    dictionary.push_back({ 0, 0, 0 });
    dictionary.push_back({ 0, 0, 0 });
    dictionary.push_back({ 0, 0, 0 });
    for (int i = 0; i < 256; ++i) {
        dictionary.push_back({ 0, static_cast<char>(i), 1 });
    }

    out.clear();
    std::size_t inOffset = 0;
    int codeLength = 9;
    int bitsAvailable = 0;
    std::uint32_t mask = 0x1ff;
    std::uint32_t codeBuffer = 0;

    auto getCode = [&] (std::uint16_t &value) {
        while (bitsAvailable < codeLength) {
            if (inOffset >= size) {
                return false;
            }
            codeBuffer = (codeBuffer << 8) | static_cast<unsigned char>(m_file.getChar());
            ++inOffset;
            bitsAvailable += 8;
        }
        bitsAvailable -= codeLength;
        value = (codeBuffer >> bitsAvailable) & mask;
        return true;
    };

    std::uint16_t last = 0;
    VERIFY(getCode(last));
    out.emplace_back(dictionary[last].c);
    char cLast = dictionary[last].c;

    std::uint16_t next = 0;
    while (getCode(next)) {
        if (next < dictionary.size()) {
            char first = 0;
            int i = next;
            while (i) {
                first = dictionary[i].c;
                i = dictionary[i].prev;
            }

            dictionary.push_back({ last, first, dictionary[last].length + 1 });
        } else if (next == dictionary.size()) {
            dictionary.push_back({ last, cLast, dictionary[last].length + 1 });
        } else {
            ASSERT(false);
        }

        std::uint16_t i = next;
        out.resize(out.size() + dictionary[next].length);
        for (int c = 0; c < dictionary[next].length; ++c) {
            out[out.size() - c - 1] = dictionary[i].c;
            cLast = dictionary[i].c;
            i = dictionary[i].prev;
        }
        last = next;

        if (dictionary.size() + 1 > mask) {
            codeBuffer = 0;
            bitsAvailable = 0;
            ++codeLength;
            ++inOffset;
            m_file.skipBytes(1);
            if (inOffset & 1) {
                ++inOffset;
                m_file.skipBytes(1);
            }
            mask = (mask << 1) + 1;
        }
    }

    return true;
}

QImage Video::createImage()
{
    if (m_frame.size() == 0)
        return createEmpty();

    QImage image(m_width, m_height, QImage::Format_Indexed8);
    image.fill(0);
    image.setColorTable(m_colorTable);

    const bool wide = (m_width * m_height) > m_frame.size();

    const std::uint8_t *index = reinterpret_cast<const std::uint8_t*>(m_frame.data());
    for (unsigned int y = 0; y < m_height; y++) {
        std::uint8_t *line = image.scanLine(y);
        if (wide) {
            const bool even = (y % 2 == 0);

            if (!even)
                *line++ = *index;

            *line++ = *index;
            std::uint8_t lastIndex = *index++;
            for (unsigned int x = 1; x < m_width / 2; x++) {
                *line++ = m_indexMap[lastIndex * 256 + *index];
                *line++ = *index;
                lastIndex = *index++;
            }

            if (even)
                *line++ = lastIndex;
        } else {
            std::memcpy(line, index, m_width);
            index += m_width;
        }
    }

    return image;
}

QImage Video::createEmpty()
{
    QImage image(m_width, m_height, QImage::Format_Indexed8);
    image.fill(0);
    image.setColorTable(QVector<QRgb>() << qRgb(0, 0, 0));
    return image;
}

} // namespace gfx
