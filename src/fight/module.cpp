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
#include <QDataStream>
#include <QFile>
#include <QGLContext>
#include <QVector3D>


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
    QMap<QString, int> textures;

    stream.skipRawData(2);
    stream >> numFaces;
    stream.skipRawData(4);
    stream >> numVertices;
    stream.skipRawData(4);

    std::vector<glm::vec3> vertices;
    vertices.reserve(numVertices);

    for (unsigned int i = 0; i < numVertices; i++) {
        quint32 index;
        QVector3D qv;
        stream >> index >> qv;
        glm::vec3 v = glm::vec3(qv.x(), qv.y(), qv.z());
        vertices.push_back(v);
        m_box.add(v);
    }

    for (unsigned int i = 0; i < numFaces; i++) {
        quint32 faceNumber;
        quint32 type;
        quint32 numVertices;
        quint32 numVertices2;

        stream >> faceNumber;
        stream >> type;
        stream.skipRawData(2);
        stream >> numVertices;

        if (type == 0) {
            stream.skipRawData(8);
            stream.skipRawData(numVertices * 12);
            stream.skipRawData(4);
        } else if (type == 1) {
            stream.skipRawData(32);
            stream >> numVertices2;
            stream.skipRawData(4);

            std::vector<glm::vec2> texCoords;
            texCoords.reserve(numVertices2 * 2);

            for (unsigned int i = 0; i < numVertices2; i++) {
                quint16 s;
                quint16 t;

                stream.skipRawData(2);
                stream >> s;
                stream.skipRawData(2);
                stream >> t;

                texCoords.emplace_back(s / 255.0f, t / 255.0f);
            }
            quint32 nameLength;
            stream >> nameLength;
            stream.skipRawData(4);

            const QString textureName = file.read(nameLength);

            std::vector<quint32> indices;
            indices.reserve(3 * numVertices2);
            for (unsigned int i = 0; i < 3 * numVertices2; i++) {
                quint32 index;
                stream >> index;
                indices.push_back(index);
            }

            int textureId;
            if (textures.contains(textureName))
                textureId = textures[textureName];
            else
            {
                textureId = m_textures.count();
                textures.insert(textureName, textureId);
                m_textures << texMan.getModule(textureName);
                m_meshes << Mesh();
            }

            Mesh &mesh = m_meshes[textureId];
            int i0 = mesh.vertices.size();

            for (unsigned int j = 0; j < numVertices2; j++)
            {
                mesh.vertices.push_back(vertices[indices[3 * j]]);
                mesh.texCoords.push_back(texCoords[j]);
                if (j > 1)
                {
                    mesh.indices.push_back(i0);
                    mesh.indices.push_back(i0 + j - 1);
                    mesh.indices.push_back(i0 + j);
                }
            }
        }
    }

    foreach (const Mesh &mesh, m_meshes)
        m_collisionMesh.addTriangles(mesh.vertices, mesh.indices);
}


void ModulePrivate::draw()
{
    glEnable(GL_TEXTURE_2D);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    //glEnableClientState(GL_NORMAL_ARRAY);

    for (int i = 0; i < m_textures.count(); i++)
    {
        m_textures[i].bind();
        glVertexPointer(3, GL_FLOAT, 0, m_meshes[i].vertices.data());
        //glNormalPointer(GL_FLOAT, 0, m_meshes[i].normals.data());
        glTexCoordPointer(2, GL_FLOAT, 0, m_meshes[i].texCoords.data());
        glDrawElements(GL_TRIANGLES, m_meshes[i].indices.size(), GL_UNSIGNED_SHORT, m_meshes[i].indices.data());
    }
}


bool ModulePrivate::intersect(const glm::vec3 &start, const glm::vec3 &dir, float radius, float &distance, glm::vec3 &normal)
{
    return m_collisionMesh.intersect(start, dir, radius, distance, normal);
}


} // namespace fight
