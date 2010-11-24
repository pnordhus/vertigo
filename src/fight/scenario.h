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

#ifndef FIGHT_SCENARIO_H
#define FIGHT_SCENARIO_H


#include "object.h"
#include "game/renderer.h"
#include "txt/desfile.h"
#include <QTime>


namespace fight {


class Surface;


class Scenario : public game::Renderer
{
    Q_OBJECT

public:
    Scenario(const QString &name);
    ~Scenario();

signals:
    void success();

protected:
    void draw();
    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent(QKeyEvent *);

private:
    QVector3D getPosition() const;

private:
    enum Type
    {
        TypeBoat           = 2049,
        TypeBomber         = 2050,
        TypeTank           = 2051,
        TypeTower          = 2052,
        TypeCrawler        = 2053,
        TypeTrash          = 2054,
        TypeBuilding       = 2056,
        TypePlayer         = 2057,
        TypeMine           = 2058,
        TypeTorpedoTower   = 2059,
        TypeSpace          = 2060,
        TypeNavPoint       = 2061,
        TypeActiveBuilding = 2062,
    };

    Surface *m_surface;
    QVector3D m_position;
    txt::DesFile m_file;
    gfx::TextureManager m_textureManager;
    ModuleManager m_moduleManager;
    QList<Object*> m_objects;
    QList<Object*> m_lightSources;

    QTime m_time;

    float m_left;
    float m_right;
    float m_up;
    float m_down;
    float m_forwards;
    float m_backwards;

    QMatrix4x4 m_cameraMatrix;
};


} // namespace fight


#endif // FIGHT_SCENARIO_H
