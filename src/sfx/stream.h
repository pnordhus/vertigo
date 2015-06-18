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

#ifndef SFX_STREAM_H
#define SFX_STREAM_H

#include "util/core.h"

#include <vector>

namespace sfx {

class Stream : util::no_copy
{
public:
    Stream();
    ~Stream();

public:
    void add(const std::vector<char> &data);
    void play();
    void pause();
    bool isPlaying() const;
    int queued() const;
    int processed() const;

private:
    void clearBuffers();

private:
    unsigned int m_source;
};

} // namespace sfx

#endif // SFX_STREAM_H
