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

#include "surface.h"
#include "gfx/colortable.h"
#include "gfx/image.h"
#include "txt/desfile.h"
#include <QGLContext>
#include <QFile>


namespace fight {


Surface::Surface(const QString &name, int maxheightscale, int mapping)
{
    m_level = 3; // Set it to 3 for even smoother surface.
    m_spline = new BetaSpline(m_level, 1, 0);
    InitIndices();

    txt::DesFile file(QString("vfx:surface/%1.des").arg(name));
    file.setSection("height");
    m_heightMap = gfx::Image::loadPCX(QString("vfx:surface/%2").arg(file.value("name").toString().replace("\\", "/")));

    m_scale.x = 1;
    m_scale.y = 1;
    m_scale.z = 1;

    {
        file.setSection("map");
        QFile scaleFile(QString("vfx:surface/%1").arg(file.value("scale").toString().replace("\\", "/")));
        scaleFile.open(QFile::ReadOnly);

        QDataStream stream(&scaleFile);
        stream.setByteOrder(QDataStream::LittleEndian);
        stream.setFloatingPointPrecision(QDataStream::SinglePrecision);
        stream >> m_scale.x >> m_scale.y >> m_scale.z;
        m_scale.x /= 32.0f;
        m_scale.y /= 32.0f;
        m_scale.z = maxheightscale / m_scale.z;
    }

    QByteArray map;
    {
        file.setSection("texture");
        QFile scaleFile(QString("vfx:surface/%1").arg(file.value("mapping").toString().replace("\\", "/")));
        scaleFile.open(QFile::ReadOnly);
        scaleFile.seek(24);

        map = scaleFile.readAll();
    }

    QByteArray dir;
    {
        file.setSection("texture");
        QFile scaleFile(QString("vfx:surface/%1").arg(file.value("direction").toString().replace("\\", "/")));
        scaleFile.open(QFile::ReadOnly);
        scaleFile.seek(24);

        dir = scaleFile.readAll();
    }

    for (int y = 0; y < m_heightMap.height() - 13; y++) {
        for (int x = 12; x < m_heightMap.width() - 1; x++) {
            const int x1 = y * m_heightMap.width() + x - 12;

            const quint8 m = map[x1];
            const quint8 d = dir[x1];

            int texture = (m >> 2);

            switch (mapping) {
            case 0:
                if (texture == 9)
                    texture = 15;
                break;
            case 1:
                if (texture == 9)
                    texture = 15;
                else if (texture == 10)
                    texture = 16;
                break;
            case 2:
                if (texture == 9)
                    texture = 15;
                else if (texture == 11)
                    texture = 14;
                break;
            }

            const float subTextureX = (m & 0x01) ? 0.5f : 0.0f;
            const float subTextureY = (m & 0x02) ? 0.5f : 0.0f;
            int rotate = -2 + (d & 0x03);
            if (rotate < 0)
                rotate += 4;
            const bool flip = !(d & 0x04);

            const float margin = 0.002f;
            QList<TexCoord> texCoords;
            texCoords << TexCoord(0.5f + subTextureX - margin, 0.0f + subTextureY + margin);
            texCoords << TexCoord(0.0f + subTextureX + margin, 0.0f + subTextureY + margin);
            texCoords << TexCoord(0.0f + subTextureX + margin, 0.5f + subTextureY - margin);
            texCoords << TexCoord(0.5f + subTextureX - margin, 0.5f + subTextureY - margin);

            if (flip) {
                qSwap(texCoords[0], texCoords[1]);
                qSwap(texCoords[2], texCoords[3]);
            }

            for (; rotate > 0; rotate--)
                texCoords.append(texCoords.takeFirst());

            Quad &quad = m_quads[texture];
            Vector v;

            foreach (const TexCoord &coord, texCoords)
                quad.texCoords << coord;

            v.x = (x + 1) * m_scale.x;
            v.y = (y + 0) * m_scale.y;
            v.z = qGray(m_heightMap.pixel(x + 1, y + 0)) * m_scale.z;
            quad.vertices << v;

            v.x = (x + 0) * m_scale.x;
            v.y = (y + 0) * m_scale.y;
            v.z = qGray(m_heightMap.pixel(x + 0, y + 0)) * m_scale.z;
            quad.vertices << v;

            v.x = (x + 0) * m_scale.x;
            v.y = (y + 1) * m_scale.y;
            v.z = qGray(m_heightMap.pixel(x + 0, y + 1)) * m_scale.z;
            quad.vertices << v;

            v.x = (x + 1) * m_scale.x;
            v.y = (y + 1) * m_scale.y;
            v.z = qGray(m_heightMap.pixel(x + 1, y + 1)) * m_scale.z;
            quad.vertices << v;

            ElementSubset &subset = m_element[texture];
            PrepareElementSubset(m_level, (short)x, (short)y, QVector3D(x, y, 0), QVector2D(texCoords[1].s, texCoords[1].t), QVector2D(texCoords[0].s - texCoords[1].s, texCoords[0].t - texCoords[1].t), QVector2D(texCoords[2].s - texCoords[1].s, texCoords[2].t - texCoords[1].t), subset);
        }
    }

    const QString sName = file.value("sname").toString();
    const QString tName = file.value("tname").toString();
    for (int i = 0; i <= 16; i++) {
        gfx::ColorTable colorTable(QString("vfx:texture/%1_%2.s16").arg(sName).arg(i, 3, 10, QChar('0')));
        m_texture << gfx::Image::load(QString("vfx:texture/%1_%2.imb").arg(tName).arg(i, 3, 10, QChar('0')), colorTable);
    }
}


Surface::~Surface()
{
    delete m_spline;
    delete indices;
}


void Surface::InitIndices()
{
	int i, j, k, l, t;
	int Level;

	indices = new short[4*(MaxLevel - 1)*(MaxLevel - 1)*3];

	k = 0;
	i = 0;
	j = 1;
	Level = 2;
	while (Level <= MaxLevel)
	{
		int i0 = i;
		int j0 = j;
		for (t = 0; t < 4; t++)
		{
			for (l = 0; l < Level - 2; l++)
			{
				indices[k++] = (short)i;
				indices[k++] = (short)j;
				indices[k++] = (short)(j + 1);

				indices[k++] = (short)i;
				j++;
				indices[k++] = (short)j;
				i = (i - i0 + 1)%(4*Level - 8) + i0;
				indices[k++] = (short)i;
			}
			indices[k++] = (short)i;
			indices[k++] = (short)j;
			indices[k++] = (short)((j - j0 + 1)%(4*Level - 4) + j0);
			j++;
		}
		i = j0;
		Level++;
	}
}


float Surface::PrepareElementSubset(int Level, short x, short y, QVector3D trans, QVector2D t0, QVector2D tu, QVector2D tv, ElementSubset &subset)
{
	int i, j, k, l;

	k = subset.vertices.count();
    ElementSubset MeshBuilder;

	m_spline->InitFrame(m_heightMap, x, y);
    QVector3D scale(m_scale.x, m_scale.y, m_scale.z);

	float dt = 1.0f/(2*(Level - 1));
	int u = Level - 1;
	int v = Level - 1;

	QVector3D o = m_spline->Beta_3_3(u, v);
	QVector3D t, b;
	m_spline->Beta_TB(u, v, &t, &b);
    QVector3D n = QVector3D::crossProduct(t, b);
    MeshBuilder.vertices << (o + trans)*scale;
    MeshBuilder.normals << n;
    MeshBuilder.texCoords << t0 + tu*u*dt + tv*v*dt;
    float z = o.z();

	l = 2;
	while (l <= Level)
	{
		u -= 1;
		v -= 1;
		int du = 2;
		int dv = 0;
		for (j = 0; j < 4; j++)
		{
			for (i = 0; i < l - 1; i++, u += du, v += dv)
			{
				o = m_spline->Beta_3_3(u, v);
				m_spline->Beta_TB(u, v, &t, &b);
                n = QVector3D::crossProduct(t, b);
                MeshBuilder.vertices << (o + trans)*scale;
                MeshBuilder.normals << n;
                MeshBuilder.texCoords << t0 + tu*u*dt + tv*v*dt;
			}
			int tmp = du;
			du = -dv;
			dv = tmp;
		}
		l++;
	}

	for (i = 0; i < 4*(Level - 1)*(Level - 1); i++)
        for (j = 0; j < 3; j++)
        {
            l = indices[i*3 + j];
            subset.vertices << MeshBuilder.vertices[l];
            subset.normals << MeshBuilder.normals[l];
            subset.texCoords << MeshBuilder.texCoords[l];
        }

	return z + trans.z();
}


float Surface::heightAt(float x, float y) const
{
    return qGray(m_heightMap.pixel(x, y)) * m_scale.z;
}


void Surface::draw()
{
    //glEnable(GL_TEXTURE_2D);
    //glDisable(GL_TEXTURE_2D);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

/*    QMapIterator<int, Quad> it(m_quads);
    while (it.hasNext()) {
        it.next();

        const Quad &quad = it.value();
        m_texture[it.key()].bind();

        glVertexPointer(3, GL_FLOAT, sizeof(Vector), quad.vertices.data());
        glTexCoordPointer(2, GL_FLOAT, 0, quad.texCoords.data());
        glDrawArrays(GL_QUADS, 0, quad.vertices.size());
    }*/

    glEnableClientState(GL_NORMAL_ARRAY);
    glFrontFace(GL_CCW);

    QMapIterator<int, ElementSubset> it(m_element);
    while (it.hasNext()) {
        it.next();

        const ElementSubset &subset = it.value();
        m_texture[it.key()].bind();

        glVertexPointer(3, GL_FLOAT, 0, subset.vertices.data());
        glNormalPointer(GL_FLOAT, 0, subset.normals.data());
        glTexCoordPointer(2, GL_FLOAT, 0, subset.texCoords.data());
        glDrawArrays(GL_TRIANGLES, 0, subset.vertices.size());
    }

    glFrontFace(GL_CW);
}


BetaSpline::BetaSpline(int Level, float defaultBeta1, float defaultBeta2)
{
	this->Level = Level;
	int n = 2*(Level - 1);

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
	{
        B[i] = new float[n + 1];
		for (j = 0; j < n + 1; j++)
			B[i][j] = b(i, (float)j/n);
	}
	for (i = 0; i < 4; i++)
	{
        BS[i] = new float[n + 1];
		for (j = 0; j < n + 1; j++)
			BS[i][j] = bs(i, (float)j/n);
	}
}


BetaSpline::~BetaSpline()
{
	for (int i = 0; i < 4; i++)
    {
        delete B[i];
        delete BS[i];
    }
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


void BetaSpline::InitFrame(const QImage &Map, short x, short y)
{
	for (short k = 0; k < 4; k++) 
		for (short l = 0; l < 4; l++) 
            frame[k][l] = QVector3D(l - 1, k - 1, qGray(Map.pixel((x + l - 1 + Map.width())%Map.width(), (y + k - 1 + Map.height())%Map.height())));
}


QVector3D BetaSpline::Beta_3_3(int u, int v)
{
	short k, l;
	QVector3D t = QVector3D(0, 0, 0);

	for (k = 0; k < 4; k++) 
		for (l = 0; l < 4; l++) 
			t += frame[k][l] * (B[l][u] * B[k][v]);
	return t;
}

QVector3D BetaSpline::Beta_norm(int u, int v)
{
	short k, l;
	QVector3D t = QVector3D(0, 0, 0);
	QVector3D o = QVector3D(0, 0, 0);

	for (k = 0; k < 4; k++)
		for (l = 0; l < 4; l++)
			t += frame[k][l] * (BS[l][u] * B[k][v]);
	for (k = 0; k < 4; k++) 
		for (l = 0; l < 4; l++) 
			o += frame[k][l] * (B[l][u] * BS[k][v]);
    return QVector3D::crossProduct(t, o);
}

void BetaSpline::Beta_TB(int u, int v, QVector3D *tangent, QVector3D *binormal)
{
	short k, l;
	*tangent = QVector3D(0, 0, 0);
	*binormal = QVector3D(0, 0, 0);

	for (k = 0; k < 4; k++)
		for (l = 0; l < 4; l++)
			*tangent += frame[k][l] * (BS[l][u] * B[k][v]);
    tangent->normalize();
	for (k = 0; k < 4; k++) 
		for (l = 0; l < 4; l++) 
			*binormal += frame[k][l] * (B[l][u] * BS[k][v]);
	binormal->normalize();
}


} // namespace fight
