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

#ifndef FIGHT_SURFACE_H
#define FIGHT_SURFACE_H


#include "vector.h"
#include "gfx/texture.h"
#include <QVector>
#include <QMap>
#include <QVector2D>
#include <QVector3D>


namespace fight {


class BetaSpline
{
public:
    BetaSpline(int Level, float defaultBeta1, float defaultBeta2);
    ~BetaSpline();

public:
    void InitFrame(const QImage &Map, short x, short y);
    QVector3D Beta_3_3(int u, int v);
    QVector3D Beta_norm(int u, int v);
    void Beta_TB(int u, int v, QVector3D *tangent, QVector3D *binormal);

private:
    float b(int i, float t);
    float bs(int i, float t);

private:
    int Level;

	float beta1, beta2;
	float b12, b13, b22, b23;
	float delta, d;

	float* B[4];
	float* BS[4];

	QVector3D frame[4][4];
};


class Surface
{
public:
    Surface(const QString &name, int maxheightscale, int mapping);
    ~Surface();

public:
    void draw();
    float heightAt(float x, float y) const;

private:
    struct TexCoord
    {
        TexCoord() {}
        TexCoord(float s, float t) : s(s), t(t) {}

        float s;
        float t;
    };

    struct Quad
    {
        QVector<Vector> vertices;
        QVector<TexCoord> texCoords;
    };

    QList<gfx::Texture> m_texture;
    QMap<int, Quad> m_quads;
    QImage m_heightMap;
    Vector m_scale;

private:
    struct ElementSubset
    {
        QVector<QVector3D> vertices;
        QVector<QVector3D> normals;
        QVector<QVector2D> texCoords;
    };

    void InitIndices();
    float PrepareElementSubset(int Level, short x, short y, QVector3D trans, QVector2D t0, QVector2D tu, QVector2D tv, ElementSubset &subset);

private:
    static const int MaxLevel = 3;

    int m_level;
    BetaSpline *m_spline;
    short *indices;

    QMap<int, ElementSubset> m_element;
};


} // namespace fight


#endif // FIGHT_SURFACE_H
