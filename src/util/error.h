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

#ifndef UTIL_ERROR_H
#define UTIL_ERROR_H

namespace util {

void errorHandlerDefault(const char *file, int line);
void errorHandlerSDL(const char *file, int line);

template <typename Func, typename Handler>
auto errorVerify(Func &&expr, const char *file, int line, Handler &&handler) -> decltype(expr())
{
    auto ret = expr();
    if (!ret) {
        handler(file, line);
    }
    return ret;
}

} // namespace util

#define VERIFY(expr) util::errorVerify([&]() { return expr; }, __FILE__, __LINE__, util::errorHandlerDefault)
#define VERIFY_SDL(expr) util::errorVerify([&]() { return expr; }, __FILE__, __LINE__, util::errorHandlerSDL)

#define ASSERT(expr) VERIFY(expr)

#endif // UTIL_ERROR_H
