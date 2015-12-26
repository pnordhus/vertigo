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

#include "betaspline.h"
#include "gfx/image.h"

#include <glm/geometric.hpp>


namespace fight {


BetaSpline::BetaSpline(std::function<float(int, int)> height, int level, float defaultBeta1, float defaultBeta2) :
	m_height(height),
	m_level(level)
{
	if (m_level < 2)
		return;
	int n = 2*(m_level - 1);

	beta1 = defaultBeta1;
	beta2 = defaultBeta2;
	b12 = beta1*beta1;
	b13 = b12*beta1;
	b22 = beta2*beta2;
	b23 = b22*beta2;
	delta = 2.0f*b13 + 4.0f*b12 + 4.0f*beta1 + beta2 + 2.0f;
	d = 1.0f/delta;

	int i, j;

	for (i = 0; i < 4; i++)
		for (j = 0; j < n + 1; j++)
			B[i].push_back(b(i, (float)j/n));
	for (i = 0; i < 4; i++)
		for (j = 0; j < n + 1; j++)
			BS[i].push_back(bs(i, (float)j/n));
}


float BetaSpline::b(int i, float t)
{
	float s = 1.0f - t;
	float t2 = t*t;
	float t3 = t2*t;
	switch (i) 
	{
		case 0: 
		{
			return 2*b13*d*s*s*s;
		}
		case 1: 
		{
			return d*(2*b13*t*(t2-3*t+3)+2*b12*(t3-3*t2+2)+2*beta1*(t3-3*t+2)+beta2*(2*t3-3*t2+1));
		}
		case 2: 
		{
			return d*(2*b12*t2*(-t+3)+2*beta1*t*(-t2+3)+beta2*t2*(-2*t+3)+2*(-t3+1));
		}
		case 3: 
		{
			return 2*t3*d;
		}
	}
	return 0;
}


float BetaSpline::bs(int i, float t)
{
	float s = 1.0f - t;
	float t2 = t*t;
	float t3 = t2*t;
	switch (i) 
	{
		case 0: 
		{
			return -6*b13*d*s*s;
		}
		case 1: 
		{
			return 6*d*(b13*(t2-2*t+1)+b12*t*(t-2)+beta1*(t2-1)+beta2*t*(t-1));
		}
		case 2: 
		{
			return 6*d*(b12*t*(-t+2)+beta1*(-t2+1)+beta2*t*(-t+1)-t2);
		}
		case 3: 
		{
			return 6*t2*d;
		}
	}
	return 0;
}


void BetaSpline::InitFrame(int x, int y)
{
	for (short k = 0; k < 4; k++) 
		for (short l = 0; l < 4; l++) 
            frame[k][l] = Vector3D(l - 1, k - 1, m_height(x + l - 1, y + k - 1));
}


Vector3D BetaSpline::Beta_3_3(float u, float v)
{
	short k, l;
	Vector3D t = Vector3D(0, 0, 0);

	for (k = 0; k < 4; k++) 
		for (l = 0; l < 4; l++) 
			t += frame[k][l] * (b(l, u) * b(k, v));
	return t;
}


Vector3D BetaSpline::Beta_3_3(int u, int v)
{
	short k, l;
	Vector3D t = Vector3D(0, 0, 0);

	for (k = 0; k < 4; k++) 
		for (l = 0; l < 4; l++) 
			t += frame[k][l] * (B[l][u] * B[k][v]);
	return t;
}


Vector3D BetaSpline::Beta_norm(float u, float v)
{
	short k, l;
	Vector3D t = Vector3D(0, 0, 0);
	Vector3D o = Vector3D(0, 0, 0);

	for (k = 0; k < 4; k++)
		for (l = 0; l < 4; l++)
			t += frame[k][l] * (bs(l, u) * b(k, v));
	for (k = 0; k < 4; k++)
		for (l = 0; l < 4; l++)
			o += frame[k][l] * (b(l, u) * bs(k, v));
	return glm::cross(t, o);
}


Vector3D BetaSpline::Beta_norm(int u, int v)
{
	short k, l;
	Vector3D t = Vector3D(0, 0, 0);
	Vector3D o = Vector3D(0, 0, 0);

	for (k = 0; k < 4; k++)
		for (l = 0; l < 4; l++)
			t += frame[k][l] * (BS[l][u] * B[k][v]);
	for (k = 0; k < 4; k++) 
		for (l = 0; l < 4; l++) 
			o += frame[k][l] * (B[l][u] * BS[k][v]);
    return glm::cross(t, o);
}


void BetaSpline::Beta_TB(int u, int v, Vector3D &tangent, Vector3D &binormal)
{
	short k, l;
	tangent = Vector3D(0, 0, 0);
	binormal = Vector3D(0, 0, 0);

	for (k = 0; k < 4; k++)
		for (l = 0; l < 4; l++)
			tangent += frame[k][l] * (BS[l][u] * B[k][v]);
	tangent = glm::normalize(tangent);
	for (k = 0; k < 4; k++) 
		for (l = 0; l < 4; l++) 
			binormal += frame[k][l] * (B[l][u] * BS[k][v]);
	binormal = glm::normalize(binormal);
}


} // namespace fight
