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

#include "sound.h"
#include <al.h>
#include <QFile>


namespace sfx {


Sound::Sound() :
    m_buffer(0)
{
    alGenSources(1, &m_source);
}


Sound::~Sound()
{
    alSourceStop(m_source);
    alDeleteSources(1, &m_source);
    if (m_buffer != 0)
        alDeleteBuffers(1, &m_buffer);
}


void Sound::stop()
{
    alSourceStop(m_source);
}


void Sound::play()
{
    alSourcei(m_source, AL_LOOPING, AL_FALSE);
    alSourcePlay(m_source);
}


void Sound::playLoop()
{
    alSourcei(m_source, AL_LOOPING, AL_TRUE);
    alSourcePlay(m_source);
}


void Sound::load(const QString &file, int rate)
{
    const QByteArray data = loadFile(file);

    alSourceStop(m_source);
    alSourcei(m_source, AL_BUFFER, 0);
    if (m_buffer != 0)
        alDeleteBuffers(1, &m_buffer);
    alGenBuffers(1, &m_buffer);
    alBufferData(m_buffer, AL_FORMAT_MONO8, data.data(), data.size(), rate <= 0 ? 22050 : rate);
    alSourcei(m_source, AL_BUFFER, m_buffer);
}


void Sound::load(const QString &leftFile, const QString &rightFile)
{
    const QByteArray left = loadFile(leftFile);
    const QByteArray right = loadFile(rightFile);

    QByteArray data;
    data.resize(left.size() + right.size());
    for (int i = 0; i < left.size(); i++) {
        data[i * 2 + 0] = left[i];
        data[i * 2 + 1] = right[i];
    }

    alSourceStop(m_source);
    alSourcei(m_source, AL_BUFFER, 0);
    if (m_buffer != 0)
        alDeleteBuffers(1, &m_buffer);
    alGenBuffers(1, &m_buffer);
    alBufferData(m_buffer, AL_FORMAT_STEREO8, data.data(), data.size(), 22050);
    alSourcei(m_source, AL_BUFFER, m_buffer);
}


QByteArray Sound::loadFile(const QString &filename)
{
    QFile file(filename);
    file.open(QFile::ReadOnly);
    file.seek(31);
    return file.readAll();
}


void Sound::setVolume(float volume)
{
    alSourcef(m_source, AL_GAIN, volume);
}


} // namespace sfx
