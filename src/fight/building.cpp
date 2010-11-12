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
#include "txt/desfile.h"
#include <QGLContext>


namespace fight {


Building::Building(ModuleManager &modMan, const QString &name, int size, float angle) :
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
        cluster.offset.setX(file.value("OffsetX").toFloat() * 16);
        cluster.offset.setY(file.value("OffsetY").toFloat() * 16);

        float height = file.value("HeightReference").toFloat();
        if (height >= 1000.0f) {
            height -= 1000.0f;
            height += 4;
            cluster.offset.setZ(64 * height * cluster.scale);
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
    glTranslatef(m_position.x(), m_position.y() - m_size * 16.0f, m_position.z());
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
