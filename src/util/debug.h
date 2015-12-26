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

#ifndef UTIL_DEBUG_H
#define UTIL_DEBUG_H

#include <iostream>

namespace util {

class Debug
{
public:
    ~Debug() {
        std::cerr << std::endl;
    }
};

template <typename T>
Debug&& operator << (Debug &&debug, T &&t) {
    std::cerr << std::forward<T>(t);
    return std::move(debug);
}

} // namespace util

#endif // UTIL_DEBUG_H
