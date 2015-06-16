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

#ifndef FIGHT_BILLBOARD_H
#define FIGHT_BILLBOARD_H


#include "../object.h"


namespace fight {


class Billboard
{
public:
    Billboard(gfx::TextureManager &texMan, txt::DesFile &name, int index);

public:
    void draw(const glm::vec3 &position, float angle, float scale, int time, const QMatrix4x4 &cameraMatrixInverted);

    BoundingBox box();
    bool intersect(const glm::vec3 &start, const glm::vec3 &dir, float &distance);
    
    int duration() const { return m_stages.count()*m_displayTime; }
    float range() const { return m_range; }
    float velocity() const { return m_velocity; }
    float collisionRadius() const { return m_collisionRadius; }
    int kineticStrength() const { return m_kineticStrength; }
    int shockStrength() const { return m_shockStrength; }

private:
    struct Stage
    {
        gfx::Texture texture;
        glm::vec2 texCoords[4];
        glm::vec2 scale;
        glm::vec2 offset;
    };

    int m_displayTime;
    QList<Stage> m_stages;
    float m_scale;

    float m_range;
    float m_velocity;
    float m_collisionRadius;
    int m_kineticStrength;
    int m_shockStrength;
};


} // namespace fight


#endif // FIGHT_BILLBOARD_H
