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

#include "horizon.h"
#include "hud.h"
#include "fight/scenario.h"
#include "fight/boats/player.h"
#include <glm/trigonometric.hpp>


namespace hud {


// Bresenham's line algorithm
void drawLine(QImage &image, float x1, float y1, float x2, float y2, int color)
{
    const bool steep = (glm::abs(y2 - y1) > glm::abs(x2 - x1));
    if (steep)
    {
        std::swap(x1, y1);
        std::swap(x2, y2);
    }

    if (x1 > x2)
    {
        std::swap(x1, x2);
        std::swap(y1, y2);
    }
 
    const float dx = x2 - x1;
    const float dy = glm::abs(y2 - y1);
 
    float error = dx/2.0f;
    const int ystep = (y1 < y2) ? 1 : -1;
    int y = (int)y1;
 
    const int maxX = (int)x2;
 
    for (int x = (int)x1; x <= maxX; x++)
    {
        if (steep)
            image.setPixel(y, x, color);
        else
            image.setPixel(x, y, color);
 
        error -= dy;
        if (error < 0)
        {
            y += ystep;
            error += dx;
        }
    }
}


void drawSegment(QImage &image, const PointF &c, std::vector<PointF> &points, std::vector<int> &colors, int i)
{
    drawLine(image, c.x + points[i].x, c.y + points[i].y, c.x + points[i + 1].x, c.y + points[i + 1].y, 0xff000000 + colors[i]*32);
    drawLine(image, c.x - points[i].x, c.y + points[i].y, c.x - points[i + 1].x, c.y + points[i + 1].y, 0xff000000 + colors[i]*32);
    drawLine(image, c.x + points[i].x, c.y + points[i].y + 1, c.x + points[i + 1].x, c.y + points[i + 1].y + 1, 0x7f000000 + colors[i]*32);
    drawLine(image, c.x - points[i].x, c.y + points[i].y + 1, c.x - points[i + 1].x, c.y + points[i + 1].y + 1, 0x7f000000 + colors[i]*32);
    drawLine(image, c.x + points[i].x, c.y + points[i].y - 1, c.x + points[i + 1].x, c.y + points[i + 1].y - 1, 0x7f000000 + colors[i]*32);
    drawLine(image, c.x - points[i].x, c.y + points[i].y - 1, c.x - points[i + 1].x, c.y + points[i + 1].y - 1, 0x7f000000 + colors[i]*32);
}


Horizon::Horizon(HUD *hud, glm::ivec2 center, const txt::DesFile &file) :
    ui::Widget(hud->widget()),
    m_hud(hud),
    m_center(center)
{
    int i;

    std::vector<PointF> upPoints(file.value("UP_NumOfPoints").toInt());
    std::vector<int> upColors(file.value("UP_NumOfPoints").toInt());
    for (i = 0; i < upPoints.size(); i++)
    {
        upPoints[i] = PointF(file.value(QString("UP_PointX%1").arg(i)).toFloat(), file.value(QString("UP_PointY%1").arg(i)).toFloat());
        upColors[i] = file.value(QString("UP_Color%1").arg(i)).toInt();
    }
        
    std::vector<PointF> downPoints(file.value("DN_NumOfPoints").toInt());
    std::vector<int> downColors(file.value("DN_NumOfPoints").toInt());
    for (i = 0; i < upPoints.size(); i++)
    {
        downPoints[i] = PointF(file.value(QString("DN_PointX%1").arg(i)).toFloat(), file.value(QString("DN_PointY%1").arg(i)).toFloat());
        downColors[i] = file.value(QString("DN_Color%1").arg(i)).toInt();
    }

    PointF c(upPoints[1].x, 10);
    QImage pitch(c.x*2 + 1, c.y*2 + 1, QImage::Format_ARGB32);
    pitch.fill(0);
    drawSegment(pitch, c, upPoints, upColors, 0);
    m_pitch = gfx::Texture(pitch, true);

    c = PointF(upPoints[upPoints.size() - 1].x, 10);
    QImage up(c.x*2 + 1, c.y*2 + 1, QImage::Format_ARGB32);
    up.fill(0);
    for (i = 1; i < upPoints.size() - 1; i++)
        drawSegment(up, c, upPoints, upColors, i);
    m_up = gfx::Texture(up, true);

    c = PointF(downPoints[downPoints.size() - 1].x, 10);
    QImage down(c.x*2 + 1, c.y*2 + 1, QImage::Format_ARGB32);
    down.fill(0);
    for (i = 1; i < downPoints.size() - 1; i++)
        drawSegment(down, c, downPoints, downColors, i);
    m_down = gfx::Texture(down, true);
}


void Horizon::draw()
{
    glm::ivec2 p = m_hud->project(m_center);
    float roll = glm::degrees(m_hud->scenario()->player()->roll());
    float pitch = glm::degrees(m_hud->scenario()->pitch());

    glPushMatrix();
    glTranslatef(p.x, p.y, 0.0f);
    glRotatef(roll, 0, 0, 1);
    if (pitch < 0)
    {
        glTranslatef(-(m_up.width() + 1)/2, -(m_up.height() + 1)/2, 0.0f);
        m_up.draw();
    }
    else
    {
        glTranslatef(-(m_down.width() + 1)/2, -(m_down.height() + 1)/2, 0.0f);
        m_down.draw();
    }
    glPopMatrix();

    glPushMatrix();
    glTranslatef(p.x, p.y + pitch/4, 0.0f);
    glRotatef(roll, 0, 0, 1);
    glTranslatef(-(m_pitch.width() + 1)/2, -(m_pitch.height() + 1)/2, 0.0f);
    m_pitch.draw();
    glPopMatrix();
}


} // namespace hud
