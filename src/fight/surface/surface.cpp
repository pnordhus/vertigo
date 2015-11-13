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
#include "element.h"
#include "betaspline.h"
#include "gfx/texture.h"
#include "gfx/colortable.h"
#include "gfx/image.h"
#include "txt/desfile.h"
#include <QGLContext>
#include <QFile>

#include <glm/geometric.hpp>

namespace fight {


Surface::Surface() :
    m_tesselator(Level, this)
{
}


void Surface::load(const QString &name, int maxheightscale, int mapping)
{
    m_mapping = mapping;

    txt::DesFile file(QString("vfx:surface/%1.des").arg(name));
    file.setSection("height");
    m_heightMap = gfx::Image::loadPCX(QString("vfx:surface/%2").arg(file.value("name").toString().replace("\\", "/")));

    qreal sx, sy, sz;
    {
        file.setSection("map");
        QFile scaleFile(QString("vfx:surface/%1").arg(file.value("scale").toString().replace("\\", "/")));
        scaleFile.open(QFile::ReadOnly);

        QDataStream stream(&scaleFile);
        stream.setByteOrder(QDataStream::LittleEndian);
        stream.setFloatingPointPrecision(QDataStream::SinglePrecision);
        stream >> sx >> sy >> sz;
        if (sz < maxheightscale)
            sz = maxheightscale;
        m_scale = Vector3D(sx / 32.0f, sy / 32.0f, sz / 32.0f);
    }

    {
        file.setSection("texture");
        QFile scaleFile(QString("vfx:surface/%1").arg(file.value("mapping").toString().replace("\\", "/")));
        scaleFile.open(QFile::ReadOnly);
        scaleFile.seek(12);

        m_textureMap = scaleFile.read(65536);
    }

    QByteArray dir;
    {
        file.setSection("texture");
        QFile scaleFile(QString("vfx:surface/%1").arg(file.value("direction").toString().replace("\\", "/")));
        scaleFile.open(QFile::ReadOnly);
        scaleFile.seek(12);

        m_textureDir = scaleFile.read(65536);
    }

    const QString sName = file.value("sname").toString();
    const QString tName = file.value("tname").toString();
    for (int i = 0; i <= 16; i++) {
        gfx::ColorTable colorTable(QString("vfx:texture/%1_%2.s16").arg(sName).arg(i, 3, 10, QChar('0')));
        m_textures.push_back(gfx::Image::load(QString("vfx:texture/%1_%2.imb").arg(tName).arg(i, 3, 10, QChar('0')), colorTable));
    }
}


float Surface::heightAt(float x, float y)
{
    return m_tesselator.heightAt(Vector2D(x / m_scale.x, y / m_scale.y)) * m_scale.z;
}


float Surface::heightAt(float x, float y, Vector3D &normal)
{
    return m_tesselator.heightAt(Vector2D(x / m_scale.x, y / m_scale.y), normal) * m_scale.z;
}


float Surface::height(int x, int y) const
{
    return qGray(m_heightMap.pixel(x & 0xff, y & 0xff));
}


void Surface::setHeight(int x, int y, int refx, int refy, int offset)
{
    int i = m_heightMap.pixelIndex(refx, refy);

    for (int dy = -1; dy < 3; dy++) {
        for (int dx = -1; dx < 3; dx++) {
            m_heightMap.setPixel(x + dx, y + dy, i + offset);
        }
    }
}


bool Surface::testCollision(const Vector3D &start, const Vector3D &end, float radius, Vector3D &position, Vector3D &normal)
{
    int x0 = end.x / m_scale.x / 8;
    int y0 = end.y / m_scale.y / 8;
    Element &element = getElement(x0*8, y0*8);
    if (element.testCollision(end, radius))
    {
        if (m_tesselator.intersect(start/m_scale, end/m_scale, radius/m_scale.z, position, normal))
        {
            position *= m_scale;
            normal = glm::normalize(normal/m_scale);
            return true;
        }
    }
    return false;
}


void Surface::bindTexture(int textureId)
{
    m_textures[textureId].bind();
}


Element& Surface::getElement(int x, int y)
{
    int id = (y << 16) + x;
    auto it = m_elements.find(id);
    if (it != m_elements.end())
        return it->second;
    Element &element = m_elements.emplace(std::piecewise_construct,
        std::forward_as_tuple(id),
        std::forward_as_tuple(this, Rect(x, y, 8, 8))).first->second;
    m_tesselator.tesselate(element, Level, m_scale, m_textureMap, m_textureDir, m_mapping);
    return element;
}


void Surface::draw(const Vector3D &position, const Vector3D &direction)
{
    int x0 = position.x / m_scale.x / 8;
    int y0 = position.y / m_scale.y / 8;

    for (int y = y0 - 2; y <= y0 + 2; y++)
        for (int x = x0 - 2; x <= x0 + 2; x++)
        {
            Element &element = getElement(x*8, y*8);
            /*QVector3D dir = element->center() - position;
            dir.normalize();
            if (QVector3D::dotProduct(dir, direction) > 0)*/
                element.draw();
        }
}


} // namespace fight
