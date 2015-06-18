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

#ifndef FIGHT_MODULE_H
#define FIGHT_MODULE_H


#include "gfx/texturemanager.h"
#include "boundingbox.h"
#include "collisionmesh.h"


namespace fight {


class Module
{
public:
    Module(gfx::TextureManager &texMan, const QString &name);

public:
    void draw();
    const BoundingBox& box() const { return m_box; }
    bool intersect(const glm::vec3 &start, const glm::vec3 &dir, float radius, float &distance, glm::vec3 &normal);

private:
    struct Mesh
    {
        gfx::Texture texture;
        std::vector<glm::vec3> vertices;
        //std::vector<glm::vec3> normals;
        std::vector<glm::vec2> texCoords;
        std::vector<quint16> indices;
    };

    BoundingBox m_box;
    std::list<Mesh> m_meshes;
    CollisionMesh m_collisionMesh;
};


} // namespace fight


#endif // FIGHT_MODULE_H
