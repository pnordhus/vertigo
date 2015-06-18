/***************************************************************************
 *  Copyright (C) 2015  Philipp Nordhus                                    *
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

#include "deferreddeletable.h"

namespace util {

void DeferredDeletable::deleteLater()
{
    m_deletePool.emplace_back(this);
}

void DeferredDeletable::clear()
{
    m_deletePool.clear();
}

std::vector<std::unique_ptr<DeferredDeletable>> DeferredDeletable::m_deletePool;

} // namespace util
