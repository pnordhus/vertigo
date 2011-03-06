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

#ifndef FIGHT_TRASH_H
#define FIGHT_TRASH_H


#include "effect.h"
#include "effectmanager.h"


namespace fight {


class Trash : public Effect
{
public:
    Trash(Scenario *scenario, Billboard *billboard, float angle);

public:
    void setPosition(const QVector3D &pos);
    bool intersect(const QVector3D &start, const QVector3D &dir, float radius, float &distance, QVector3D &normal);
    void destroy();

public:
    static Effects trashCollection[9];

private:
};


} // namespace fight


#endif // FIGHT_TRASH_H
