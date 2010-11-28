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

#include "projectile.h"
#include "billboard.h"


namespace fight {


Projectile::Projectile(Billboard *billboard) : 
    Effect(billboard, 0)
{
}


void Projectile::setDirection(const QVector3D &direction)
{
    m_direction = direction;
}


void Projectile::draw()
{
    m_billboard->draw(m_position + m_direction*m_time.elapsed()*m_billboard->velocity()/1000, m_angle, 2, m_time.elapsed(), m_cameraMatrixInverted);
}


bool Projectile::atEnd()
{
    return m_time.elapsed()*m_billboard->velocity()/1000 > m_billboard->range();
}


} // namespace fight
