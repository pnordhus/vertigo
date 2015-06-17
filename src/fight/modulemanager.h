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

#ifndef FIGHT_MODULEMANAGER_H
#define FIGHT_MODULEMANAGER_H


#include "module.h"
#include "gfx/texturemanager.h"


namespace fight {


class ModuleManager
{
public:
    ModuleManager(gfx::TextureManager &texMan);

public:
    Module* get(const QString &name);

private:
    gfx::TextureManager &m_textureManager;
    std::map<QString, Module> m_modules;
};


} // namespace fight


#endif // FIGHT_MODULEMANAGER_H
