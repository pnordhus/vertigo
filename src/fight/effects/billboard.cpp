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
#include "txt/desfile.h"
#include <QGLContext>


namespace fight {


QVector3D square[4] = {QVector3D(-1, 1, 0), QVector3D(1, 1, 0), QVector3D(1, -1, 0), QVector3D(-1, -1, 0)};


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
    QVector2D center;
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

        Stage stage;
        stage.texture = texMan.getModule(file.value("imgblockname").toString(), true);

        QVector2D t0(file.value("x1").toInt() / 255.0f, file.value("y1").toInt() / 255.0f);
        stage.texCoords[0] = t0;
        if (file.contains("width"))
        {
            numInX = file.value("numinx").toInt();
            width = file.value("width").toInt() / 255.0f;
            height = file.value("height").toInt() / 255.0f;
            center = QVector2D(width/2, height/2);
            stage.texCoords[1] = t0 + QVector2D(width - 1/255.0f, 0);
            stage.texCoords[2] = t0 + QVector2D(width - 1/255.0f, height - 1/255.0f);
            stage.texCoords[3] = t0 + QVector2D(0, height - 1/255.0f);
        }
        else
        {
            stage.texCoords[1] = QVector2D(file.value("x2").toInt() / 255.0f, file.value("y2").toInt() / 255.0f);
            stage.texCoords[2] = QVector2D(file.value("x3").toInt() / 255.0f, file.value("y3").toInt() / 255.0f);
            stage.texCoords[3] = QVector2D(file.value("x4").toInt() / 255.0f, file.value("y4").toInt() / 255.0f);

            width = (stage.texCoords[1] - t0).x() + 1/255.0f;
            height = (stage.texCoords[3] - t0).y() + 1/255.0f;
            center = QVector2D(file.value("xcenter").toInt() / 255.0f, file.value("ycenter").toInt() / 255.0f);
            center -= t0;
        }

        stage.scale = QVector2D(width, height)*scale*2; // TODO: test scale
        stage.offset = QVector2D(1, -1) - QVector2D(center.x()/width, -center.y()/height)*2;

        m_stages << stage;
    }

    if (m_stages.count() < numOfStages)
    {
        if (numInX == 0)
            numInX = (int)(1.0f/width);
        i = m_stages.count() - 1;
        for (j = 1; j < numOfStages - i; j++)
        {
            Stage stage = m_stages[i];
            int x = (int)(stage.texCoords[0].x()/width + 1e-5);
            for (k = 0; k < 4; k++)
                stage.texCoords[k] += QVector2D(width*((x + j)%numInX - x), height*((x + j)/numInX));
            m_stages << stage;
        }
    }
}



void Billboard::draw(QVector3D position, float angle, float scale, int time, const QMatrix4x4 &cameraMatrixInverted)
{
    int currentStage = time/m_displayTime%m_stages.count();

    glEnable(GL_TEXTURE_2D);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_NOTEQUAL, 0.0);

    glPushMatrix();
    glTranslatef(position.x(), position.y(), position.z());
    glScalef(m_scale*scale, m_scale*scale, m_scale*scale);
    glMultMatrixd(cameraMatrixInverted.data());
    glRotatef(angle, 0, 0, 1);
    glScalef(m_stages[currentStage].scale.x(), m_stages[currentStage].scale.y(), 0);
    glTranslatef(m_stages[currentStage].offset.x(), m_stages[currentStage].offset.y(), 0);

    m_stages[currentStage].texture.bind();
    glVertexPointer(3, GL_FLOAT, 0, square);
    glTexCoordPointer(2, GL_FLOAT, 0, m_stages[currentStage].texCoords);
    glDrawArrays(GL_QUADS, 0, 4);

    glPopMatrix();
    glDisable(GL_ALPHA_TEST);
}


} // namespace fight
