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
#include "gfx/texturemanager.h"
#include <QDataStream>
#include <QFile>
#include <QGLContext>
#include <QVector3D>


namespace fight {


Module::Module(gfx::TextureManager &texMan, const QString &name)
{
    QFile file(name);
    file.open(QFile::ReadOnly);

    QDataStream stream(&file);
    stream.setByteOrder(QDataStream::LittleEndian);
    stream.setFloatingPointPrecision(QDataStream::SinglePrecision);

    unsigned int i, j;
    quint32 numFaces;
    quint32 numVertices;
    std::map<QString, Mesh*> meshes;

    stream.skipRawData(2);
    stream >> numFaces;
    stream.skipRawData(4);
    stream >> numVertices;
    stream.skipRawData(4);

    std::vector<Vector3D> vertices;
    vertices.reserve(numVertices);

    for (i = 0; i < numVertices; i++) {
        quint32 index;
        QVector3D qv;
        stream >> index >> qv;
        Vector3D v = Vector3D(qv.x(), qv.y(), qv.z());
        vertices.push_back(v);
        m_box.add(v);
    }

    for (i = 0; i < numFaces; i++) {
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

            std::vector<Vector2D> texCoords;
            texCoords.reserve(numVertices2 * 2);

            for (j = 0; j < numVertices2; j++) {
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
            for (j = 0; j < 3 * numVertices2; j++) {
                quint32 index;
                stream >> index;
                indices.push_back(index);
            }

            Mesh *mesh;
            auto it = meshes.find(textureName);
            if (it != meshes.end())
                mesh = it->second;
            else
            {
                m_meshes.emplace_back();
                mesh = &m_meshes.back();
                meshes[textureName] = mesh;
                mesh->texture = texMan.getModule(textureName);
            }

            int i0 = mesh->vertices.size();

            for (j = 0; j < numVertices2; j++)
            {
                mesh->vertices.push_back(vertices[indices[3 * j]]);
                mesh->texCoords.push_back(texCoords[j]);
                if (j > 1)
                {
                    mesh->indices.push_back(i0);
                    mesh->indices.push_back(i0 + j - 1);
                    mesh->indices.push_back(i0 + j);
                }
            }
        }
    }

    for (const Mesh &mesh : m_meshes)
        m_collisionMesh.addTriangles(mesh.vertices, mesh.indices);
}


void Module::draw()
{
    glEnable(GL_TEXTURE_2D);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    //glEnableClientState(GL_NORMAL_ARRAY);

    for (Mesh &mesh : m_meshes)
    {
        mesh.texture.bind();
        glVertexPointer(3, GL_FLOAT, 0, mesh.vertices.data());
        //glNormalPointer(GL_FLOAT, 0, mesh.normals.data());
        glTexCoordPointer(2, GL_FLOAT, 0, mesh.texCoords.data());
        glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_SHORT, mesh.indices.data());
    }
}


bool Module::intersect(const Vector3D &start, const Vector3D &dir, float radius, float &distance, Vector3D &normal)
{
    return m_collisionMesh.intersect(start, dir, radius, distance, normal);
}


} // namespace fight
