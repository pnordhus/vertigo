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

#ifndef FIGHT_BOUNDINGBOX_H
#define FIGHT_BOUNDINGBOX_H


#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>


namespace fight {


class BoundingBox
{
public:
    BoundingBox();
    BoundingBox(const BoundingBox &box);
    BoundingBox(const glm::vec3 &min, const glm::vec3 &max);

public:
    const glm::vec3& minPoint() const { return m_min; }
    const glm::vec3& maxPoint() const { return m_max; }
    glm::vec3 dim() const { return m_max - m_min; }
    BoundingBox transform(const glm::mat4 &m)  const;

    void add(const glm::vec3 &point);
    void add(const BoundingBox &box);
    bool test(const glm::vec3 &center, float radius) const;

public:
    glm::vec3 m_min;
    glm::vec3 m_max;
};


} // namespace fight


#endif // FIGHT_BOUNDINGBOX_H
