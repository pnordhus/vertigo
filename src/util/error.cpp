/***************************************************************************
 *  Copyright (C) 2015  Philipp Nordhus                                    *
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

#include "error.h"

#include <SDL.h>
#include <iostream>

namespace util {

void errorHandlerDefault(const char *file, int line)
{
    std::cerr << file << ":" << line << std::endl;
    std::abort();
}

void errorHandlerSDL(const char *file, int line)
{
    std::cerr << "'" << SDL_GetError() << "' in " << file << ":" << line << std::endl;
    std::abort();
}

} // namespace util
