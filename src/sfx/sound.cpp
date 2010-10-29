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
#include "soundsystem.h"
#include <al.h>
#include <QFile>


namespace sfx {


Sound::Sound() :
    m_source(0),
    m_buffer(0),
    m_volume(1.0f)
{

}


Sound::Sound(const QString &file) :
    m_source(0),
    m_buffer(0),
    m_volume(1.0f)
{
    load(file);
}


Sound::~Sound()
{
    stop();
    if (m_buffer != 0)
        alDeleteBuffers(1, &m_buffer);
}


void Sound::stop()
{
    if (m_source > 0) {
        alSourceStop(m_source);
        SoundSystem::get()->release(m_source);
        m_source = 0;
    }
}


bool Sound::acquire()
{
    if (m_source == 0) {
        m_source = SoundSystem::get()->acquire(this);
        if (m_source == 0)
            return false;
    }

    alSourcef(m_source, AL_GAIN, m_volume);
    return true;
}


void Sound::play()
{
    if (!acquire())
        return;

    alSourcei(m_source, AL_LOOPING, AL_FALSE);
    alSourcei(m_source, AL_BUFFER, m_buffer);
    alSourcePlay(m_source);
}


void Sound::playLoop()
{
    if (!acquire())
        return;

    alSourcei(m_source, AL_LOOPING, AL_TRUE);
    alSourcei(m_source, AL_BUFFER, m_buffer);
    alSourcePlay(m_source);
}


void Sound::pause()
{
    if (m_source > 0)
        alSourcePause(m_source);
}


void Sound::resume()
{
    if (m_source > 0) {
        ALint state;
        alGetSourcei(m_source, AL_SOURCE_STATE, &state);
        if (state == AL_PAUSED)
            alSourcePlay(m_source);
    }
}


void Sound::load(const QString &file, int rate)
{
    const QByteArray data = loadFile(file);

    stop();
    if (m_buffer != 0)
        alDeleteBuffers(1, &m_buffer);
    alGenBuffers(1, &m_buffer);
    alBufferData(m_buffer, AL_FORMAT_MONO8, data.data(), data.size(), rate <= 0 ? 22050 : rate);
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

    stop();
    if (m_buffer != 0)
        alDeleteBuffers(1, &m_buffer);
    alGenBuffers(1, &m_buffer);
    alBufferData(m_buffer, AL_FORMAT_STEREO8, data.data(), data.size(), 22050);
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
    m_volume = volume;
    if (m_source > 0)
        alSourcef(m_source, AL_GAIN, volume);
}


} // namespace sfx
