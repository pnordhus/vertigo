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

#include "betaspline.h"
#include "tesselator.h"
#include "element.h"
#include "surface.h"

#include <deque>
#include <glm/geometric.hpp>
#include <glm/gtx/norm.hpp>


namespace fight {


Tesselator::Tesselator(int MaxLevel, Surface *surface) :
    m_surface(surface)
{
	std::function<float(int, int)> height = [this](int x, int y) { return m_surface->height(x, y); };
	for (int i = 0; i <= MaxLevel; i++)
		m_splines.emplace_back(height, i, 1.0f, 0.0f);
    InitIndices(MaxLevel);
}


void Tesselator::InitIndices(int MaxLevel)
{
	int i, j, l, t;
	int Level;

	indices.reserve(4*(MaxLevel - 1)*(MaxLevel - 1)*3);

	i = 0;
	j = 1;
	Level = 2;
	while (Level <= MaxLevel)
	{
		int i0 = i;
		int j0 = j;
		for (t = 0; t < 4; t++)
		{
			for (l = 0; l < Level - 2; l++)
			{
                indices.push_back(i);
                indices.push_back(j);
				indices.push_back(j + 1);

                indices.push_back(i);
				j++;
                indices.push_back(j);
				i = (i - i0 + 1)%(4*Level - 8) + i0;
                indices.push_back(i);
			}
            indices.push_back(i);
            indices.push_back(j);
            indices.push_back((j - j0 + 1)%(4*Level - 4) + j0);
			j++;
		}
		i = j0;
		Level++;
	}
}


void Tesselator::PrepareElementSubset(Element &element, int level, const glm::vec3 &scale, int x, int y, int textureId, const glm::vec2 &t0, const glm::vec2 &tu, const glm::vec2 &tv)
{
	int i, j, k, l;

	k = element.numVertices(textureId);

    BetaSpline &spline = m_splines[level];
	spline.InitFrame(x, y);
	glm::vec3 trans(x, y, 0);

	float dt = 1.0f/(2*(level - 1));
	int u = level - 1;
	int v = level - 1;

	glm::vec3 o = spline.Beta_3_3(u, v);
	glm::vec3 t, b;
	spline.Beta_TB(u, v, t, b);
    element.addVertex(textureId, (o + trans)*scale, glm::cross(t, b), t0 + tu*(u*dt) + tv*(v*dt));
    float z = o.z;

	l = 2;
	while (l <= level)
	{
		u -= 1;
		v -= 1;
		int du = 2;
		int dv = 0;
		for (j = 0; j < 4; j++)
		{
			for (i = 0; i < l - 1; i++, u += du, v += dv)
			{
				o = spline.Beta_3_3(u, v);
				spline.Beta_TB(u, v, t, b);
                element.addVertex(textureId, (o + trans)*scale, glm::cross(t, b), t0 + tu*(u*dt) + tv*(v*dt));
			}
			int tmp = du;
			du = -dv;
			dv = tmp;
		}
		l++;
	}

	for (i = 0; i < 4*(level - 1)*(level - 1); i++)
        element.addTriangle(textureId, k + indices[i*3 + 0], k + indices[i*3 + 2], k + indices[i*3 + 1]);
}


float Tesselator::heightAt(const glm::vec2 &pos)
{
    const int x = pos.x;
    const int y = pos.y;
    const float dx = pos.x - x;
    const float dy = pos.y - y;

    BetaSpline &spline = m_splines[2];
	spline.InitFrame(x, y);

    return spline.Beta_3_3(dx, dy).z;
}


float Tesselator::heightAt(const glm::vec2 &pos, glm::vec3 &normal)
{
    const int x = pos.x;
    const int y = pos.y;
    const float dx = pos.x - x;
    const float dy = pos.y - y;

    BetaSpline &spline = m_splines[2];
	spline.InitFrame(x, y);

    normal = spline.Beta_norm(dx, dy);
    return spline.Beta_3_3(dx, dy).z;
}


bool Tesselator::intersect(const glm::vec3 &start, const glm::vec3 &end, float radius, glm::vec3 &position, glm::vec3 &normal)
{
    BetaSpline &spline = m_splines[2];

    int x = start.x;
    int y = start.y;
	spline.InitFrame(x, y);
    float dx = start.x - x;
    float dy = start.y - y;

    float z = spline.Beta_3_3(dx, dy).z;
    position = glm::vec3(start.x, start.y, z + radius + 1e-5);
    if (z > start.z - radius)
    {
        normal = spline.Beta_norm(dx, dy);
		return true;
    }

    if (x != (int)end.x || y != (int)end.y)
    {
        x = end.x;
        y = end.y;
    	spline.InitFrame(x, y);
    }
    dx = end.x - x;
    dy = end.y - y;

    z = spline.Beta_3_3(dx, dy).z;
    if (z <= end.z - radius)
        return false;

    glm::vec3 l = start;
    glm::vec3 r = end;
    glm::vec3 m;
    while (glm::distance2(l, r) > 0.01f)
    {
        m = glm::mix(l, r, 0.5f);

        if (x != (int)m.x || y != (int)m.y)
        {
            x = m.x;
            y = m.y;
    	    spline.InitFrame(x, y);
        }
        dx = m.x - x;
        dy = m.y - y;

        z = spline.Beta_3_3(dx, dy).z;
        if (z > m.z - radius)
            r = m;
        else
            l = m;
    }

    position = l;
    normal = spline.Beta_norm(dx, dy);
	return true;
}


void Tesselator::tesselate(Element &element, int level, const glm::vec3 &scale, QByteArray &textureMap, QByteArray &textureDir, int mapping)
{
    for (int y = element.rect().top(); y <= element.rect().bottom(); y++) {
        for (int x = element.rect().left(); x <= element.rect().right(); x++) {
            const int x1 = ((y & 0xff) << 8) + (x & 0xff);

            const quint8 m = textureMap[x1];
            const quint8 d = textureDir[x1];

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
            int rotate = -2 + (d & 0x03);
            if (rotate < 0)
                rotate += 4;
            const bool flip = !(d & 0x04);

            const float margin = 0.002f;
            std::deque<glm::vec2> texCoords;
            texCoords.emplace_back(0.5f + subTextureX - margin, 0.0f + subTextureY + margin);
            texCoords.emplace_back(0.0f + subTextureX + margin, 0.0f + subTextureY + margin);
            texCoords.emplace_back(0.0f + subTextureX + margin, 0.5f + subTextureY - margin);
            texCoords.emplace_back(0.5f + subTextureX - margin, 0.5f + subTextureY - margin);

            if (flip) {
                std::swap(texCoords[0], texCoords[1]);
                std::swap(texCoords[2], texCoords[3]);
            }

            for (; rotate > 0; rotate--)
            {
                texCoords.push_back(texCoords.front());
                texCoords.pop_front();
            }

            PrepareElementSubset(element, level, scale, x, y, texture, texCoords[1], texCoords[0] - texCoords[1], texCoords[2] - texCoords[1]);
        }
    }
}


} // namespace fight
