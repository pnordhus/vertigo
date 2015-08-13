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

#include "element.h"
#include "surface.h"
#include <QGLContext>


namespace fight {


Element::Element(Surface *surface, const util::Rect &rect) :
    m_surface(surface),
    m_rect(rect),
    m_maxZ(-1e3),
    m_minZ(1e3),
    m_heights(rect.width*rect.height, -1e3)
{
}


int Element::numVertices(int textureId)
{
    return m_subsets[textureId].vertices.size();
}


void Element::addVertex(int textureId, const glm::vec3 &position, const glm::vec3 &normal, const glm::vec2 &texCoords)
{
	m_subsets[textureId].vertices.push_back(position);
    m_subsets[textureId].normals.push_back(normal);
    m_subsets[textureId].texCoords.push_back(texCoords);
    if (m_maxZ < position.z)
        m_maxZ = position.z;
    if (m_minZ > position.z)
        m_minZ = position.z;

    int ix, iy;
    ix = position.x/m_surface->scale().x + 1e-3 - m_rect.x;
    iy = position.y/m_surface->scale().y + 1e-3 - m_rect.y;
    if (ix >= 0 && ix < m_rect.width && iy >= 0 && iy < m_rect.height && m_heights[iy*m_rect.width + ix] < position.z)
        m_heights[iy*m_rect.width + ix] = position.z;
    ix = position.x/m_surface->scale().x - 1e-3 - m_rect.x;
    iy = position.y/m_surface->scale().y + 1e-3 - m_rect.y;
    if (ix >= 0 && ix < m_rect.width && iy >= 0 && iy < m_rect.height && m_heights[iy*m_rect.width + ix] < position.z)
        m_heights[iy*m_rect.width + ix] = position.z;
    ix = position.x/m_surface->scale().x + 1e-3 - m_rect.x;
    iy = position.y/m_surface->scale().y - 1e-3 - m_rect.y;
    if (ix >= 0 && ix < m_rect.width && iy >= 0 && iy < m_rect.height && m_heights[iy*m_rect.width + ix] < position.z)
        m_heights[iy*m_rect.width + ix] = position.z;
    ix = position.x/m_surface->scale().x - 1e-3 - m_rect.x;
    iy = position.y/m_surface->scale().y - 1e-3 - m_rect.y;
    if (ix >= 0 && ix < m_rect.width && iy >= 0 && iy < m_rect.height && m_heights[iy*m_rect.width + ix] < position.z)
        m_heights[iy*m_rect.width + ix] = position.z;
}


void Element::addTriangle(int textureId, unsigned short a, unsigned short b, unsigned short c)
{
    m_subsets[textureId].indices.push_back(a);
    m_subsets[textureId].indices.push_back(b);
    m_subsets[textureId].indices.push_back(c);
}


glm::vec3 Element::center() const
{
    return glm::vec3(m_rect.center().x * m_surface->scale().x, m_rect.center().y * m_surface->scale().y, m_minZ + (m_maxZ - m_minZ)/2);
}


bool Element::testCollision(const glm::vec3 &center, float radius)
{
    int ix = center.x/m_surface->scale().x - m_rect.x;
    int iy = center.y/m_surface->scale().y - m_rect.y;
    if (ix >= 0 && ix < m_rect.width && iy >= 0 && iy < m_rect.height &&
        m_heights[iy*m_rect.width + ix] > center.z - radius)
        return true;
    return false;
}


void Element::draw()
{
    glEnable(GL_TEXTURE_2D);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    for (auto &pair : m_subsets)
    {
        const ElementSubset &subset = pair.second;
        m_surface->bindTexture(pair.first);

        glVertexPointer(3, GL_FLOAT, 0, subset.vertices.data());
        glNormalPointer(GL_FLOAT, 0, subset.normals.data());
        glTexCoordPointer(2, GL_FLOAT, 0, subset.texCoords.data());
        glDrawElements(GL_TRIANGLES, subset.indices.size(), GL_UNSIGNED_SHORT, subset.indices.data());
    }

    glDisableClientState(GL_NORMAL_ARRAY);
}


} // namespace fight
