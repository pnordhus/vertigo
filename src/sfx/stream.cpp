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

#include "stream.h"

#include <al.h>

namespace sfx {

Stream::Stream()
{
    alGenSources(1, &m_source);
}

Stream::~Stream()
{
    alSourceStop(m_source);
    clearBuffers();
    alDeleteSources(1, &m_source);
}

void Stream::add(const std::vector<char> &data)
{
    clearBuffers();

    ALuint buffer;
    alGenBuffers(1, &buffer);
    alBufferData(buffer, AL_FORMAT_STEREO8, data.data(), data.size(), 22050);
    alSourceQueueBuffers(m_source, 1, &buffer);
}

void Stream::play()
{
    alSourcePlay(m_source);
}

void Stream::pause()
{
    alSourcePause(m_source);
}

bool Stream::isPlaying() const
{
    ALint state;
    alGetSourcei(m_source, AL_SOURCE_STATE, &state);
    return (state == AL_PLAYING);
}

int Stream::queued() const
{
    ALint v;
    alGetSourcei(m_source, AL_BUFFERS_QUEUED, &v);
    return v;
}

int Stream::processed() const
{
    ALint v;
    alGetSourcei(m_source, AL_BUFFERS_PROCESSED, &v);
    return v;
}

void Stream::clearBuffers()
{
    const int buffersProcessed = processed();
    if (buffersProcessed > 0) {
        std::vector<ALuint> bufs(buffersProcessed);
        alSourceUnqueueBuffers(m_source, buffersProcessed, &bufs[0]);
        alDeleteBuffers(buffersProcessed, &bufs[0]);
    }
}

} // namespace sfx
