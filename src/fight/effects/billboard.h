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


#include <vector>
#include "util/geometry3d.h"
#include "gfx/texture.h"


namespace gfx { class TextureManager; }
namespace txt { class DesFile; }


namespace fight {


class BoundingBox;


class Billboard
{
public:
    Billboard(gfx::TextureManager &texMan, txt::DesFile &name, int index, bool blendColor = true);

public:
    void draw(const Vector3D &position, float angle, float scale, int time, const Matrix &cameraMatrixInverted);

    BoundingBox box();
    bool intersect(const Vector3D &start, const Vector3D &dir, float &distance);
    
    int duration() const { return m_stages.size()*m_displayTime; }
    float range() const { return m_range; }
    float velocity() const { return m_velocity; }
    float collisionRadius() const { return m_collisionRadius; }
    float noiseLevel() const { return m_noiseLevel; }
    int kineticShield() const { return m_kineticShield; }
    int kineticStrength() const { return m_kineticStrength; }
    int shockStrength() const { return m_shockStrength; }

private:
    struct Stage
    {
        gfx::Texture texture;
        Vector2D texCoords[4];
        Vector2D scale;
        Vector2D offset;
    };

    int m_displayTime;
    std::vector<Stage> m_stages;
    float m_scale;
    bool m_blendColor;

    float m_range;
    float m_velocity;
    float m_collisionRadius;
    float m_noiseLevel;
    int m_kineticShield;
    int m_kineticStrength;
    int m_shockStrength;
};


} // namespace fight


#endif // FIGHT_BILLBOARD_H
