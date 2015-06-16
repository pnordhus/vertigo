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

#ifndef FIGHT_BUILDING_H
#define FIGHT_BUILDING_H


#include "object.h"


namespace fight {


class Surface;


class Building : public Object
{
public:
    Building(Scenario *scenario, const QString &name, int size, float angle, int x, int y, int refx, int refy);

public:
    void draw();
    bool intersect(const glm::vec3 &start, const glm::vec3 &dir, float radius, float &distance, glm::vec3 &normal);

private:
    struct Cluster
    {
        Module module;
        glm::vec3 offset;
        float scale;
        float angle;
        glm::mat4 transform;
        glm::mat4 invTransform;
    };

    QList<Cluster> m_clusters;
    int m_size;
    float m_angle;
};


} // namespace fight


#endif // FIGHT_BUILDING_H
