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


#include "vector.h"
#include "gfx/texture.h"
#include <QVector>
#include <QMap>


namespace fight {


class Surface
{
public:
    Surface(const QString &name, int maxheightscale, int mapping);

public:
    void draw();
    float heightAt(float x, float y) const;

private:
    struct TexCoord
    {
        TexCoord() {}
        TexCoord(float s, float t) : s(s), t(t) {}

        float s;
        float t;
    };

    struct Quad
    {
        QVector<Vector> vertices;
        QVector<TexCoord> texCoords;
    };

    QList<gfx::Texture> m_texture;
    QMap<int, Quad> m_quads;
    QImage m_heightMap;
    Vector m_scale;
};


} // namespace fight


#endif // FIGHT_SURFACE_H
