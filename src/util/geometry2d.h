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

#ifndef GEOMETRY2D_H
#define GEOMETRY2D_H


#include <glm/vec2.hpp>
#include <glm/common.hpp>
#include "size.hpp"
#include "rect.hpp"


typedef util::RectangleExclusive<int> Rect;

typedef util::RectangleInclusive<float> RectF;

typedef glm::ivec2 Point;

typedef glm::vec2 PointF;

typedef util::Size2D<int> Size;

typedef util::Size2D<float> SizeF;


#endif // GEOMETRY2D_H
