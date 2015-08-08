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

#include "billboard.h"
#include "fight/collisionmesh.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/norm.hpp>

namespace fight {


glm::vec3 square[4] = { glm::vec3(-1, 1, 0), glm::vec3(1, 1, 0), glm::vec3(1, -1, 0), glm::vec3(-1, -1, 0) };


Billboard::Billboard(gfx::TextureManager &texMan, txt::DesFile &file, int index)
{
    file.setSection(QString("size%1").arg(index));
    m_scale = file.value("scale").toFloat();

    file.setSection(QString("collision%1").arg(index));
    m_collisionRadius = file.value("radiusscale").toFloat();

    file.setSection(QString("offense%1").arg(index));
    m_kineticStrength = file.value("kineticstrength").toInt();
    m_shockStrength = file.value("shockstrength").toInt();

    file.setSection(QString("kinematics%1").arg(index));
    if (file.contains("range"))
    {
        m_range = file.value("range").toInt() / 32.0f;
        m_velocity = file.value("velocity").toInt() / 32.0f;
    }


    file.setSection(QString("anim%1").arg(index));
    float scale = file.value("scale").toFloat();

    m_displayTime = (int)(file.value("displaytime").toFloat()*1000) + 20;
    int numOfStages = file.value("numofstages").toInt();
    int numInX = 0;

    int i, j, k;
    float width, height;
    glm::vec2 center;
    for (i = -1; ; i++)
    {
        if (i < 0)
        {
            file.setSection(QString("anim%1raster").arg(index));
            if (!file.contains("imgblockname"))
                i++;
        }
        if (i >= 0)
        {
            file.setSection(QString("anim%1stage%2").arg(index).arg(i));
            if (!file.contains("imgblockname"))
                break;
        }

        m_stages.emplace_back();
        Stage &stage = m_stages.back();
        stage.texture = texMan.getModule(file.value("imgblockname").toString(), true);

        glm::vec2 t0(file.value("x1").toInt() / 255.0f, file.value("y1").toInt() / 255.0f);
        stage.texCoords[0] = t0;
        if (file.contains("width"))
        {
            numInX = file.value("numinx").toInt();
            width = file.value("width").toInt() / 255.0f;
            height = file.value("height").toInt() / 255.0f;
            center = glm::vec2(width/2, height/2);
            stage.texCoords[1] = t0 + glm::vec2(width - 1/255.0f, 0);
            stage.texCoords[2] = t0 + glm::vec2(width - 1/255.0f, height - 1/255.0f);
            stage.texCoords[3] = t0 + glm::vec2(0, height - 1/255.0f);
        }
        else
        {
            stage.texCoords[1] = glm::vec2(file.value("x2").toInt() / 255.0f, file.value("y2").toInt() / 255.0f);
            stage.texCoords[2] = glm::vec2(file.value("x3").toInt() / 255.0f, file.value("y3").toInt() / 255.0f);
            stage.texCoords[3] = glm::vec2(file.value("x4").toInt() / 255.0f, file.value("y4").toInt() / 255.0f);

            width = (stage.texCoords[1] - t0).x + 1/255.0f;
            height = (stage.texCoords[3] - t0).y + 1/255.0f;
            center = glm::vec2(file.value("xcenter").toInt() / 255.0f, file.value("ycenter").toInt() / 255.0f);
            center -= t0;
        }

        stage.scale = glm::vec2(width, height)*scale*2.0f; // TODO: test scale
        stage.offset = glm::vec2(1, -1) - glm::vec2(center.x/width, -center.y/height)*2.0f;
    }

    if (m_stages.size() < numOfStages)
    {
        if (numInX == 0)
            numInX = (int)(1.0f/width);
        i = m_stages.size() - 1;
        for (j = 1; j < numOfStages - i; j++)
        {
            m_stages.push_back(m_stages[i]);
            Stage &stage = m_stages.back();
            int x = (int)(stage.texCoords[0].x/width + 1e-5);
            for (k = 0; k < 4; k++)
                stage.texCoords[k] += glm::vec2(width*((x + j)%numInX - x), height*((x + j)/numInX));
        }
    }
}



void Billboard::draw(const glm::vec3 &position, float angle, float scale, int time, const glm::mat4 &cameraMatrixInverted)
{
    int currentStage = time/m_displayTime%m_stages.size();

    glEnable(GL_TEXTURE_2D);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_NOTEQUAL, 0.0);

    glPushMatrix();
    glTranslatef(position.x, position.y, position.z);
    glMultMatrixf(glm::value_ptr(cameraMatrixInverted));
    glScalef(m_scale*scale, m_scale*scale, 1);
    glRotatef(angle, 0, 0, 1);
    glScalef(m_stages[currentStage].scale.x, m_stages[currentStage].scale.y, 1);
    glTranslatef(m_stages[currentStage].offset.x, m_stages[currentStage].offset.y, 2);

    m_stages[currentStage].texture.bind();
    glVertexPointer(3, GL_FLOAT, 0, square);
    glTexCoordPointer(2, GL_FLOAT, 0, m_stages[currentStage].texCoords);
    glDrawArrays(GL_QUADS, 0, 4);

    glPopMatrix();
    glDisable(GL_ALPHA_TEST);
}


BoundingBox Billboard::box()
{
    float scale = m_scale*m_stages[0].scale.length();
    return BoundingBox(glm::vec3(-1, -1, -1)*scale, glm::vec3(1, 1, 1)*scale);
}


bool Billboard::intersect(const glm::vec3 &start, const glm::vec3 &dir, float &distance)
{
    return CollisionMesh::intersectSphereLine(start, dir, m_scale*m_scale*glm::length2(m_stages[0].scale), distance);
}


} // namespace fight
