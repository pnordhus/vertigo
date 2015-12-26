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

#ifndef UTIL_SIZE_H
#define UTIL_SIZE_H


#include <glm/vec2.hpp>


namespace util {


template<typename T, glm::precision P = glm::defaultp>
class Size2D
{
public:
#if GLM_FORCE_NO_CTOR_INIT
    Size2D() = default;
#else
    Size2D() : width(0), height(0) { };
#endif
    Size2D(T width, T height) : width(width), height(height) { };
    Size2D(const glm::tvec2<T> &size) : width(size.x), height(size.y) { };

    template<typename R>
    Size2D(const Size2D<R> &cast) : width(static_cast<T>(cast.width)), height(static_cast<T>(cast.height)) { };

public:

public:
    T width;
    T height;
};


template<typename T>
glm::tvec2<T> operator + (const glm::tvec2<T> &a, const Size2D<T> &b)
{
    return glm::tvec2<T>(a.x + b.width, a.y + b.height);
}


template<typename T>
glm::tvec2<T> operator - (const glm::tvec2<T> &a, const Size2D<T> &b)
{
    return glm::tvec2<T>(a.x - b.width, a.y - b.height);
}


} // namespace util


namespace glm {
namespace detail {
    template <typename R, typename T, precision P>
    struct functor1<R, T, P, util::Size2D>
    {
        GLM_FUNC_QUALIFIER static util::Size2D<R, P> call(R(*Func) (T x), util::Size2D<T, P> const & v)
        {
            return util::Size2D<R, P>(Func(v.width), Func(v.height));
        }
    };
}
}


#endif // UTIL_SIZE_H
