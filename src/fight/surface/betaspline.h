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

#ifndef SURFACE_BETASPLINE_H
#define SURFACE_BETASPLINE_H


#include <QVector3D>


namespace fight {


class BetaSpline
{
public:
    BetaSpline(int Level, float defaultBeta1, float defaultBeta2);
    ~BetaSpline();

public:
    void InitFrame(float height(int,int), int x, int y);

    QVector3D Beta_3_3(float u, float v);
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


} // namespace fight


#endif // SURFACE_BETASPLINE_H
