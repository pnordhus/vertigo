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

#ifndef SURFACE_TESSELATOR_H
#define SURFACE_TESSELATOR_H


#include <QList>
#include <QVector2D>
#include <QVector3D>


namespace fight {


class BetaSpline;
class Element;
class Surface;


class Tesselator
{
public:
    Tesselator(int MaxLevel, Surface *surface);
    ~Tesselator();

public:
    static Tesselator* get(Surface *surface);

public:
    float heightAt(QVector2D pos) const;
    Element* tesselate(QRect rect, int level, QVector3D scale, QByteArray &textureMap, QByteArray &textureDir, int mapping);

private:
    void InitIndices(int MaxLevel);
    void PrepareElementSubset(int level, QVector3D scale, int x, int y, int textureId, QVector2D t0, QVector2D tu, QVector2D tv, Element *element);
    static float height(int x, int y);

private:
    QList<BetaSpline*> m_splines;
    short *indices;

    Surface *m_surface;
};


} // namespace fight


#endif // SURFACE_TESSELATOR_H
