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
#include "surface/surface.h"
#include "txt/desfile.h"
#include <QGLContext>


namespace fight {


Building::Building(ModuleManager &modMan, const QString &name, int size, float angle, Surface *surface, int x, int y, int refx, int refy) :
    Object(modMan, name),
    m_size(size),
    m_angle(angle)
{
    txt::DesFile file(QString("vfx:sobjects/%1.des").arg(name));

    int i = 0;

    while (file.sections().contains(QString("cluster%1").arg(i))) {
        file.setSection(QString("cluster%1").arg(i));

        Cluster cluster;
        cluster.module = modMan.get(file.value("base").toString());

        file.setSection(QString("size%1").arg(i));
        cluster.scale = file.value("scale").toFloat() * 16;

        file.setSection(QString("position%1").arg(i));
        int offsetX = file.value("OffsetX").toInt();
        int offsetY = file.value("OffsetY").toInt();
        cluster.offset.setX(offsetX);
        cluster.offset.setY(offsetY);
        cluster.offset *= surface->scale();

        int clusterX = x + offsetX;
        int clusterY = y + offsetY - size;

        int heightRef = file.value("HeightReference").toInt();
        if (heightRef >= 1000)
            cluster.offset.setZ(m_clusters[heightRef - 1000].offset.z() + 256*cluster.scale); // TODO: test moar
        else
        {
            if (file.value("SurfaceLifting").toString() == "p")
            {
                if (heightRef == 0)
                    surface->setHeight(clusterX, clusterY, refx, refy, 0);
                else
                {
                    qDebug("Untested: HeightReference = -1");
                    surface->setHeight(clusterX, clusterY, clusterX, clusterY, -4); // TODO: test
                }
            }
            cluster.offset.setZ(surface->heightAt((clusterX + 0.5f)*surface->scale().x(), (clusterY + 0.5f)*surface->scale().y()));
        }

        switch (file.value("cardinal").toString().toAscii()[0]) {
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

        m_clusters << cluster;
        i++;
    }
}


void Building::draw()
{
    glPushMatrix();
    glTranslatef(m_position.x(), m_position.y() - m_size * 16.0f, 0);
    glRotatef(m_angle, 0, 0, 1);

    QMutableListIterator<Cluster> it(m_clusters);
    while (it.hasNext()) {
        Cluster &cluster = it.next();

        glPushMatrix();
        glTranslatef(cluster.offset.x(), cluster.offset.y(), cluster.offset.z());
        glScalef(cluster.scale, cluster.scale, cluster.scale);
        glRotatef(cluster.angle, 0, 0, 1);
        cluster.module.draw();
        glPopMatrix();
    }
    glPopMatrix();
}


} // namespace fight
