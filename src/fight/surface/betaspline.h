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


#include <functional>
#include <vector>
#include "util/geometry3d.h"


namespace fight {


class BetaSpline
{
public:
	BetaSpline(std::function<float(int, int)> height, int level, float defaultBeta1, float defaultBeta2);

public:
    void InitFrame(int x, int y);

    Vector3D Beta_3_3(float u, float v);
	Vector3D Beta_3_3(int u, int v);
	Vector3D Beta_norm(float u, float v);
	Vector3D Beta_norm(int u, int v);
	void Beta_TB(int u, int v, Vector3D &tangent, Vector3D &binormal);

private:
    float b(int i, float t);
    float bs(int i, float t);

private:
	std::function<float(int, int)> m_height;
    int m_level;

	float beta1, beta2;
	float b12, b13, b22, b23;
	float delta, d;

	std::vector<float> B[4];
	std::vector<float> BS[4];

	Vector3D frame[4][4];
};


} // namespace fight


#endif // SURFACE_BETASPLINE_H
