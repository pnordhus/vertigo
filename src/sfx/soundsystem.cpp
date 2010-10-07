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
#include <AL/alc.h>


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
}


SoundSystem::~SoundSystem()
{
    alcDestroyContext(d->context);
    alcCloseDevice(d->device);
    delete d;
    m_singleton = NULL;
}


} // namespace sfx
