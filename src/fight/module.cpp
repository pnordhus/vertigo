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

#include "module.h"
#include "vector.h"
#include <QDataStream>
#include <QFile>
#include <QGLContext>


namespace fight {


void ModulePrivate::load(gfx::TextureManager &texMan, const QString &name)
{
    QFile file(name);
    file.open(QFile::ReadOnly);

    QDataStream stream(&file);
    stream.setByteOrder(QDataStream::LittleEndian);
    stream.setFloatingPointPrecision(QDataStream::SinglePrecision);

    quint32 numFaces;
    quint32 numVertices;

    stream.skipRawData(2);
    stream >> numFaces;
    stream.skipRawData(4);
    stream >> numVertices;
    stream.skipRawData(4);

    QVector<Vector> vertices;
    vertices.reserve(numVertices);

    for (unsigned int i = 0; i < numVertices; i++) {
        quint32 index;
        Vector v;
        stream >> index >> v.x >> v.y >> v.z;
        vertices << v;
    }

    for (unsigned int i = 0; i < numFaces; i++) {
        quint32 faceNumber;
        quint32 unknown;
        quint32 numVertices;
        quint32 numVertices2;
        quint32 someNumber;

        stream >> faceNumber;
        stream >> unknown;
        stream.skipRawData(2);
        stream >> numVertices;
        stream.skipRawData(8);
        stream >> someNumber;
        stream.skipRawData(12);

        if (unknown == 0)
            continue;

        stream.skipRawData(8);
        stream >> numVertices2;
        stream.skipRawData(4);

        QVector<TexCoord> texCoords;
        texCoords.reserve(numVertices2 * 2);
        for (unsigned int i = 0; i < numVertices2; i++) {
            quint32 s;
            quint32 t;

            stream >> s;
            stream >> t;

            texCoords << TexCoord(s / 65535.0 / 255, t / 65535.0f / 255);
        }

        quint32 nameLength;
        stream >> nameLength;
        stream.skipRawData(4);

        const QString textureName = file.read(nameLength);

        QVector<quint32> indices;
        indices.reserve(3 * numVertices2);
        for (unsigned int i = 0; i < 3 * numVertices2; i++) {
            quint32 index;
            stream >> index;
            indices << index;
        }

        Face face;
        face.texture = texMan.getModule(textureName);
        for (unsigned int j = 0; j < numVertices2; j++) {
            face.vertices << vertices[indices[3 * j]];
            face.texCoords << texCoords[j];
        }

        m_faces << face;
    }
}


void ModulePrivate::draw()
{
    glEnable(GL_TEXTURE_2D);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    QMutableListIterator<Face> it(m_faces);
    while (it.hasNext()) {
        Face &face = it.next();
        face.texture.bind();
        glVertexPointer(3, GL_FLOAT, sizeof(Vector), face.vertices.data());
        glTexCoordPointer(2, GL_FLOAT, 0, face.texCoords.data());
        glDrawArrays(GL_TRIANGLE_FAN, 0, face.vertices.size());
    }
}


} // namespace fight
