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

#ifndef SFX_SOUNDSYSTEM_H
#define SFX_SOUNDSYSTEM_H


#include "sound.h"
#include <QMap>


namespace sfx {


class Sound;
struct SoundSystemPrivate;


enum StandardSound
{
    Click,
    Woop,
    FrameOpen,
    FrameClose,
    NotebookShow,
    NotebookHide,
    NotebookBackground,
    EnComShow,
    EnComHide = NotebookHide,
};


class SoundSystem
{
public:
    SoundSystem();
    ~SoundSystem();

public:
    Sound* sound(StandardSound);
    quint32 acquire(Sound* sound);
    void release(quint32 source);
    void pauseAll();
    void resumeAll();

public:
    static SoundSystem *get() { Q_ASSERT(m_singleton); return m_singleton; }

private:
    Q_DISABLE_COPY(SoundSystem);
    void releaseUnused();

private:
    static SoundSystem *m_singleton;
    SoundSystemPrivate *d;
    QMap<StandardSound, Sound*> m_standardSounds;
    QMap<quint32, Sound*> m_sources;
};


} // namespace sfx


#endif // SFX_SOUNDSYSTEM_H
