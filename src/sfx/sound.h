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

#ifndef SFX_SOUND_H
#define SFX_SOUND_H


#include <QtGlobal>


namespace sfx {


class Sound
{
public:
    Sound();
    ~Sound();

public:
    void stop();
    void play();
    void playLoop();
    void load(const QString &file, int rate = 0);
    void load(const QString &leftFile, const QString &rightFile);
    void setVolume(float volume);

private:
    Q_DISABLE_COPY(Sound);
    QByteArray loadFile(const QString &filename);

private:
    quint32 m_source;
    quint32 m_buffer;
};


} // namespace sfx


#endif // SFX_SOUND_H
