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

#ifndef SURFACE_ELEMENT_H
#define SURFACE_ELEMENT_H


#include <map>
#include <vector>
#include "util/geometry2d.h"
#include "util/geometry3d.h"


namespace fight {


class Surface;


class Element
{
public:
    Element(Surface *surface, const Rect &rect);

public:
    int numVertices(int textureId);
	void addVertex(int textureId, const Vector3D &position, const Vector3D &normal, const Vector2D &texCoords);
    void addTriangle(int textureId, unsigned short a, unsigned short b, unsigned short c);

    void draw();
    Rect rect() const { return m_rect; }
    float maxZ() const { return m_maxZ; }
    float minZ() const { return m_minZ; }
    Vector3D center() const;
    bool testCollision(const Vector3D &center, float radius);

private:
    struct ElementSubset
    {
        std::vector<Vector3D> vertices;
        std::vector<Vector3D> normals;
        std::vector<Vector2D> texCoords;
        std::vector<unsigned short> indices;
    };

private:
    Surface *m_surface;
    Rect m_rect;
    float m_maxZ;
    float m_minZ;
    std::vector<float> m_heights;

    std::map<int, ElementSubset> m_subsets;
};


} // namespace fight


#endif // SURFACE_ELEMENT_H
