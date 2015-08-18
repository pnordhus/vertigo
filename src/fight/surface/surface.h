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


#include <map>
#include <vector>
#include <QImage>
#include "tesselator.h"


namespace gfx { class Texture; }


namespace fight {


class Element;


class Surface
{
public:
    Surface();
    void load(const QString &name, int maxheightscale, int mapping);

public:
    void draw(const Vector3D &position, const Vector3D &direction);
    float heightAt(float x, float y);
    float heightAt(float x, float y, Vector3D &normal);
    const Vector3D& scale() const { return m_scale; }
    float height(int x, int y) const;
    void bindTexture(int textureId);
    void setHeight(int x, int y, int refx, int refy, int offset);
    bool testCollision(const Vector3D &start, const Vector3D &end, float radius, Vector3D &position, Vector3D &normal);

private:
    Element& getElement(int x, int y);

private:
    std::vector<gfx::Texture> m_textures;
    std::map<int, Element> m_elements;
    QImage m_heightMap;
    QByteArray m_textureMap;
    QByteArray m_textureDir;
    Vector3D m_scale;
    int m_mapping;

private:
    static const int Level = 3;
    Tesselator m_tesselator;
};


} // namespace fight


#endif // FIGHT_SURFACE_H
