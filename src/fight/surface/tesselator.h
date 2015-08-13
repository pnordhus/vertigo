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

#ifndef SURFACE_TESSELATOR_H
#define SURFACE_TESSELATOR_H


#include <QByteArray>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>


namespace fight {


class Element;
class Surface;
class BetaSpline;


class Tesselator
{
public:
    Tesselator(int MaxLevel, Surface *surface);

public:
	float heightAt(const glm::vec2 &pos);
    float heightAt(const glm::vec2 &pos, glm::vec3 &normal);
    void tesselate(Element &element, int level, const glm::vec3 &scale, QByteArray &textureMap, QByteArray &textureDir, int mapping);
    bool intersect(const glm::vec3 &start, const glm::vec3 &end, float radius, glm::vec3 &position, glm::vec3 &normal);

private:
    void InitIndices(int MaxLevel);
    void PrepareElementSubset(Element &element, int level, const glm::vec3 &scale, int x, int y, int textureId, const glm::vec2 &t0, const glm::vec2 &tu, const glm::vec2 &tv);

private:
	std::vector<BetaSpline> m_splines;
    std::vector<unsigned short> indices;

    Surface *m_surface;
};


} // namespace fight


#endif // SURFACE_TESSELATOR_H
