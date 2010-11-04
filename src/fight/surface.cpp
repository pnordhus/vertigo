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

#include "surface.h"
#include "gfx/colortable.h"
#include "gfx/image.h"
#include "txt/desfile.h"
#include <QGLContext>
#include <QFile>


namespace fight {


Surface::Surface(const QString &name, int mapping)
{
    txt::DesFile file(QString("vfx:surface/%1.des").arg(name));
    file.setSection("height");
    const QImage heightMap = gfx::Image::loadPCX(QString("vfx:surface/%2").arg(file.value("name").toString().replace("\\", "/")));

    float scaleX = 1;
    float scaleY = 1;
    float scaleZ = 1;

    {
        file.setSection("map");
        QFile scaleFile(QString("vfx:surface/%1").arg(file.value("scale").toString().replace("\\", "/")));
        scaleFile.open(QFile::ReadOnly);

        int x,y,z;
        QDataStream stream(&scaleFile);
        stream.setByteOrder(QDataStream::LittleEndian);
        stream >> x >> y >> z;
        scaleX = *(float*) &x;
        scaleY = *(float*) &y;
        scaleZ = *(float*) &z;
    }

    QByteArray map;
    {
        file.setSection("texture");
        QFile scaleFile(QString("vfx:surface/%1").arg(file.value("mapping").toString().replace("\\", "/")));
        scaleFile.open(QFile::ReadOnly);
        scaleFile.seek(24);

        map = scaleFile.readAll();
    }

    QByteArray dir;
    {
        file.setSection("texture");
        QFile scaleFile(QString("vfx:surface/%1").arg(file.value("direction").toString().replace("\\", "/")));
        scaleFile.open(QFile::ReadOnly);
        scaleFile.seek(24);

        dir = scaleFile.readAll();
    }

    for (int y = 0; y < heightMap.height() - 13; y++) {
        for (int x = 12; x < heightMap.width() - 1; x++) {
            const int x1 = y * heightMap.width() + x - 12;

            const quint8 m = map[x1];
            const quint8 d = dir[x1];

            int texture = (m >> 2);

            switch (mapping) {
            case 0:
                if (texture == 9)
                    texture = 15;
                break;
            case 1:
                if (texture == 9)
                    texture = 15;
                else if (texture == 10)
                    texture = 16;
                break;
            case 2:
                if (texture == 9)
                    texture = 15;
                else if (texture == 11)
                    texture = 14;
                break;
            }

            const float subTextureX = (m & 0x01) ? 0.5f : 0.0f;
            const float subTextureY = (m & 0x02) ? 0.5f : 0.0f;
            int rotate = d & 0x03;
            const bool flip = d & 0x04;
            if ((m & 3) >= 2) {
                if (flip)
                    rotate += 3;
                else
                    rotate++;
            }

            QList<TexCoord> texCoords;
            texCoords << TexCoord(0.0f + subTextureX, 0.0f + subTextureY);
            texCoords << TexCoord(0.0f + subTextureX, 0.5f + subTextureY);
            texCoords << TexCoord(0.5f + subTextureX, 0.5f + subTextureY);
            texCoords << TexCoord(0.5f + subTextureX, 0.0f + subTextureY);

            if (flip) {
                qSwap(texCoords[0], texCoords[3]);
                qSwap(texCoords[1], texCoords[2]);
            }

            for (; rotate > 0; rotate--)
                texCoords.append(texCoords.takeFirst());

            Quad &quad = m_quads[texture];
            Vector v;

            foreach (const TexCoord &coord, texCoords)
                quad.texCoords << coord;

            v.x = (x + 0) * scaleX;
            v.y = (y + 0) * scaleY;
            v.z = qGray(heightMap.pixel(x + 0, y + 0)) * scaleZ;
            quad.vertices << v;

            v.x = (x + 0) * scaleX;
            v.y = (y + 1) * scaleY;
            v.z = qGray(heightMap.pixel(x + 0, y + 1)) * scaleZ;
            quad.vertices << v;

            v.x = (x + 1) * scaleX;
            v.y = (y + 1) * scaleY;
            v.z = qGray(heightMap.pixel(x + 1, y + 1)) * scaleZ;
            quad.vertices << v;

            v.x = (x + 1) * scaleX;
            v.y = (y + 0) * scaleY;
            v.z = qGray(heightMap.pixel(x + 1, y + 0)) * scaleZ;
            quad.vertices << v;
        }
    }

    const QString sName = file.value("sname").toString();
    const QString tName = file.value("tname").toString();
    for (int i = 0; i <= 16; i++) {
        gfx::ColorTable colorTable(QString("vfx:texture/%1_%2.s16").arg(sName).arg(i, 3, 10, QChar('0')));
        m_texture << gfx::Image::load(QString("vfx:texture/%1_%2.imb").arg(tName).arg(i, 3, 10, QChar('0')), colorTable);
    }
}


void Surface::draw()
{
    glEnable(GL_TEXTURE_2D);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    QMapIterator<int, Quad> it(m_quads);
    while (it.hasNext()) {
        it.next();

        const Quad &quad = it.value();
        m_texture[it.key()].bind();

        glVertexPointer(3, GL_FLOAT, sizeof(Vector), quad.vertices.data());
        glTexCoordPointer(2, GL_FLOAT, 0, quad.texCoords.data());
        glDrawArrays(GL_QUADS, 0, quad.vertices.size());
    }
}


} // namespace fight
