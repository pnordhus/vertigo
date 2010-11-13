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


Element::Element(Surface *surface, QRect rect) :
    m_surface(surface),
    m_rect(rect),
    m_maxZ(-1e3),
    m_minZ(1e3)
{
}


Element::~Element()
{
}


int Element::numVertices(int textureId)
{
    if (!m_subsets.contains(textureId))
        m_subsets.insert(textureId, ElementSubset());
    return m_subsets[textureId].vertices.count();
}


void Element::addVertex(int textureId, QVector3D position, QVector3D normal, QVector2D texCoords)
{
    m_subsets[textureId].vertices << position;
    m_subsets[textureId].normals << normal;
    m_subsets[textureId].texCoords << texCoords;
    if (m_maxZ < position.z())
        m_maxZ = position.z();
    if (m_minZ > position.z())
        m_minZ = position.z();
}


void Element::addTriangle(int textureId, short a, short b, short c)
{
    m_subsets[textureId].indices << a << b << c;
}


QVector3D Element::center() const
{
    return QVector3D(m_rect.center().x() * m_surface->scale().x(), m_rect.center().y() * m_surface->scale().y(), m_minZ + (m_maxZ - m_minZ)/2);
}


void Element::draw()
{
    glEnable(GL_TEXTURE_2D);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    QMapIterator<int, ElementSubset> it(m_subsets);
    while (it.hasNext()) {
        it.next();

        const ElementSubset &subset = it.value();
        m_surface->bindTexture(it.key());

        glVertexPointer(3, GL_FLOAT, 0, subset.vertices.data());
        glNormalPointer(GL_FLOAT, 0, subset.normals.data());
        glTexCoordPointer(2, GL_FLOAT, 0, subset.texCoords.data());
        glDrawElements(GL_TRIANGLES, subset.indices.count(), GL_UNSIGNED_SHORT, subset.indices.data());
    }

    glDisableClientState(GL_NORMAL_ARRAY);
}


} // namespace fight
