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


#include "object.h"
#include "gfx/texturemanager.h"
#include <QVector2D>
#include <QTime>


namespace fight {


class Billboard : public Object
{
public:
    Billboard(gfx::TextureManager &texMan, const QString &name, int index);

public:
    void draw();

private:
    struct Stage
    {
        gfx::Texture texture;
        QVector2D texCoords[4];
        QVector2D scale;
        QVector2D offset;
    };

    float m_angle;
    float m_displayTime;
    QList<Stage> m_stages;
    int m_currentStage;
    QTime m_time;
};


} // namespace fight


#endif // FIGHT_BILLBOARD_H
