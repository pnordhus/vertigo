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


#include "game/renderer.h"


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
    void mouseMoveEvent(QMouseEvent *);

private:
    Surface *m_surface;
    float m_angleX;
    float m_angleY;
    QPoint m_mouse;
};


} // namespace fight


#endif // FIGHT_SCENARIO_H
