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

#ifndef FIGHT_MODULE_H
#define FIGHT_MODULE_H


#include "vector.h"
#include "gfx/texturemanager.h"


namespace fight {


class ModulePrivate : public QSharedData
{
public:
    void load(gfx::TextureManager &texMan, const QString &name);
    void draw();

private:
    struct TexCoord
    {
        TexCoord() {}
        TexCoord(float s, float t) : s(s), t(t) {}

        float s;
        float t;
    };

    struct Face
    {
        gfx::Texture texture;
        QVector<Vector> vertices;
        QVector<TexCoord> texCoords;
    };

    QList<Face> m_faces;
};


class Module
{
public:
    Module();
    Module(gfx::TextureManager &texMan, const QString &name);

public:
    void draw();

private:
    QExplicitlySharedDataPointer<ModulePrivate> d;
};


inline Module::Module() :
    d(new ModulePrivate)
{

}


inline Module::Module(gfx::TextureManager &texMan, const QString &name) :
    d(new ModulePrivate)
{
    d->load(texMan, name);
}


inline void Module::draw()
{
    d->draw();
}


} // namespace fight


#endif // FIGHT_MODULE_H
