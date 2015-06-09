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


#include "gfx/texturemanager.h"
#include "boundingbox.h"
#include "collisionmesh.h"
#include <QVector2D>


namespace fight {


class ModulePrivate : public QSharedData
{
public:
    void load(gfx::TextureManager &texMan, const QString &name);
    void draw();
    BoundingBox box() const { return m_box; }
    bool intersect(const QVector3D &start, const QVector3D &dir, float radius, float &distance, QVector3D &normal);

private:
    struct Mesh
    {
        QVector<QVector3D> vertices;
        //QVector<QVector3D> normals;
        QVector<QVector2D> texCoords;
        QVector<quint16> indices;
    };

    BoundingBox m_box;
    QList<gfx::Texture> m_textures;
    QList<Mesh> m_meshes;
    CollisionMesh m_collisionMesh;
};


class Module
{
public:
    Module();
    Module(gfx::TextureManager &texMan, const QString &name);

public:
    void draw();
    BoundingBox box() const { return d->box(); }
    bool intersect(const QVector3D &start, const QVector3D &dir, float radius, float &distance, QVector3D &normal);

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


inline bool Module::intersect(const QVector3D &start, const QVector3D &dir, float radius, float &distance, QVector3D &normal)
{
    return d->intersect(start, dir, radius, distance, normal);
}


} // namespace fight


#endif // FIGHT_MODULE_H
