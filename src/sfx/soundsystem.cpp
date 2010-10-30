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

#include "soundsystem.h"
#include <al.h>
#include <alc.h>


namespace sfx {


SoundSystem *SoundSystem::m_singleton = NULL;


struct SoundSystemPrivate
{
    ALCdevice* device;
    ALCcontext* context;
};


SoundSystem::SoundSystem()
{
    Q_ASSERT(m_singleton == NULL);
    m_singleton = this;

    d = new SoundSystemPrivate;
    d->device = alcOpenDevice(NULL);
    d->context = alcCreateContext(d->device, NULL);
    alcMakeContextCurrent(d->context);

    m_standardSounds.insert(Click, new Sound("sfx:snd/desktop/click.pcm"));
    m_standardSounds.insert(Woop, new Sound("sfx:snd/desktop/woop.pcm"));
    m_standardSounds.value(Woop)->setVolume(0.1f);

    m_standardSounds.insert(NotebookShow, new Sound("sfx:snd/desktop/noteb1.pcm"));
    m_standardSounds.insert(NotebookHide, new Sound("sfx:snd/desktop/noteb3.pcm"));
    m_standardSounds.insert(EnComShow, new Sound("sfx:snd/desktop/noteb2.pcm"));
    m_standardSounds.insert(NotebookBackground, new Sound("sfx:snd/desktop/noteback.pcm"));
    m_standardSounds.value(NotebookBackground)->setVolume(0.033f);
}


SoundSystem::~SoundSystem()
{
    qDeleteAll(m_standardSounds);

    Q_ASSERT(m_sources.isEmpty());

    alcDestroyContext(d->context);
    alcCloseDevice(d->device);
    delete d;
    m_singleton = NULL;
}


Sound* SoundSystem::sound(StandardSound snd)
{
    Sound *sound = m_standardSounds.value(snd);
    Q_ASSERT(sound);
    return sound;
}


void SoundSystem::releaseUnused()
{
    // stop all Sounds with stopped sources
    // this implicitly frees the sources
    QMapIterator<quint32, Sound*> it(m_sources);
    while (it.hasNext()) {
        it.next();

        ALint state;
        alGetSourcei(it.key(), AL_SOURCE_STATE, &state);
        if (state == AL_STOPPED)
            it.value()->stop();
    }
}


quint32 SoundSystem::acquire(Sound *sound)
{
    releaseUnused();
    if (m_sources.size() == 32)
        return 0;

    quint32 source;
    alGenSources(1, &source);
    m_sources.insert(source, sound);
    return source;
}


void SoundSystem::release(quint32 source)
{
    alSourceStop(source);
    alDeleteSources(1, &source);
    m_sources.remove(source);
}


void SoundSystem::pauseAll()
{
    releaseUnused();

    foreach (Sound* sound, m_sources)
        sound->pause();
}


void SoundSystem::resumeAll()
{
    foreach (Sound* sound, m_sources)
        sound->resume();
}


} // namespace sfx
