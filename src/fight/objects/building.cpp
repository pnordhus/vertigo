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

#include "building.h"
#include "fight/scenario.h"
#include "fight/module.h"

#include <glm/gtc/matrix_transform.hpp>

namespace fight {


Building::Building(Scenario *scenario, txt::DesFile &file, int size, float angle, int x, int y, int refx, int refy) :
    Object(scenario, file),
    m_size(size),
    m_angle(angle)
{
    int i = 0;
    glm::vec3 scale = scenario->surface().scale();
    m_position = glm::vec3(x + 0.5f, y + 0.5f - size, 0)*scale;

    while (file.sections().contains(QString("cluster%1").arg(i))) {
        file.setSection(QString("cluster%1").arg(i));

        m_clusters.emplace_back();
        Cluster &cluster = m_clusters.back();
        cluster.module = scenario->moduleManager().get(file.value("base").toString());

        file.setSection(QString("size%1").arg(i));
        cluster.scale = file.value("scale").toFloat() * 16;

        file.setSection(QString("position%1").arg(i));
        int offsetX = file.value("OffsetX").toInt();
        int offsetY = file.value("OffsetY").toInt();
        cluster.offset.x = offsetX;
        cluster.offset.y = offsetY;
        cluster.offset *= scale;

        int clusterX = x + offsetX;
        int clusterY = y + offsetY - size;

        int heightRef = file.value("HeightReference").toInt();
        if (heightRef >= 1000)
            cluster.offset.z = m_clusters[heightRef - 1000].offset.z + 256*cluster.scale; // TODO: test moar
        else
        {
            if (file.value("SurfaceLifting").toString() == "p")
            {
                if (heightRef == 0)
                    scenario->surface().setHeight(clusterX, clusterY, refx, refy, 0);
                else
                {
                    qDebug("Untested: HeightReference = -1");
                    scenario->surface().setHeight(clusterX, clusterY, clusterX, clusterY, -4); // TODO: test
                }
            }
            cluster.offset.z = scenario->surface().heightAt((clusterX + 0.5f)*scale.x, (clusterY + 0.5f)*scale.y);
        }

        switch (file.value("cardinal").toString().toLatin1()[0]) {
        case 'n':
            cluster.angle = 0;
            break;
        case 'w':
            cluster.angle = 90;
            break;
        case 's':
            cluster.angle = 180;
            break;
        case 'e':
            cluster.angle = 270;
            break;
        }

        i++;
    }

    for (Cluster &cluster : m_clusters)
    {
        glm::mat4 m(1);
        m = glm::translate(m, m_position);
        m = glm::rotate(m, glm::radians(m_angle), glm::vec3(0, 0, 1));
        m = glm::translate(m, cluster.offset);
        m = glm::scale(m, glm::vec3(cluster.scale));
        m = glm::rotate(m, glm::radians(cluster.angle), glm::vec3(0, 0, 1));
        m_box.add(cluster.module->box().transform(m));
        
        cluster.transform = m;
        cluster.invTransform = glm::inverse(m);
    }

    scenario->collisionManager().addObject(this);
}


void Building::draw()
{
    glPushMatrix();
    glTranslatef(m_position.x, m_position.y, m_position.z);
    glRotatef(m_angle, 0, 0, 1);

    for (Cluster &cluster : m_clusters)
    {
        glPushMatrix();
        glTranslatef(cluster.offset.x, cluster.offset.y, cluster.offset.z);
        glScalef(cluster.scale, cluster.scale, cluster.scale);
        glRotatef(cluster.angle, 0, 0, 1);
        cluster.module->draw();
        glPopMatrix();
    }
    glPopMatrix();
}


bool Building::intersect(const glm::vec3 &start, const glm::vec3 &dir, float radius, float &distance, glm::vec3 &normal)
{
    bool collision = false;
    for (Cluster &cluster : m_clusters)
    {
        float d;
        glm::vec3 norm;
        if (cluster.module->intersect(glm::vec3(cluster.invTransform * glm::vec4(start, 1)), glm::vec3(cluster.invTransform * glm::vec4(dir, 0))*cluster.scale, radius/cluster.scale, d, norm))
        {
            d *= cluster.scale;
            if (!collision || distance > d)
            {
                distance = d;
                normal = glm::vec3(cluster.transform * glm::vec4(norm, 0))/cluster.scale;
                collision = true;
            }
        }
    }
    return collision;
}


} // namespace fight
