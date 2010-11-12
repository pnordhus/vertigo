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

#ifndef FIGHT_SURFACE_H
#define FIGHT_SURFACE_H


#include "../vector.h"
#include "gfx/texture.h"
#include <QVector>
#include <QMap>
#include <QVector2D>
#include <QVector3D>


namespace fight {


class Tesselator;
class Element;


class Surface
{
public:
    Surface(const QString &name, int maxheightscale, int mapping);
    ~Surface();

public:
    void draw(QVector3D position, QVector3D direction);
    float heightAt(float x, float y) const;
    QVector3D scale() const { return m_scale; }
    float height(int x, int y) const;
    void bindTexture(int textureId);

private:
    Element* getElement(QPoint pos);

private:
    QList<gfx::Texture> m_textures;
    QImage m_heightMap;
    QByteArray m_textureMap;
    QByteArray m_textureDir;
    QVector3D m_scale;
    int m_mapping;

private:
    static const int Level = 3;
    Tesselator *m_tesselator;
    QMap<int, Element*> m_elements;
};


} // namespace fight


#endif // FIGHT_SURFACE_H
