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

#include "samplemap.h"
#include "txt/desfile.h"


namespace sfx {


std::vector<Sound> SampleMap::m_samples;


void SampleMap::load()
{
    if (m_samples.size() > 0)
        return;

    txt::DesFile fileDes("sfx:snd/smpmap.des");
    fileDes.setSection("samplemap");

    for (int i = 0; i <= 83; i++)
    {
        m_samples.emplace_back();
        QString nam = fileDes.value(QString("nam%1").arg(i)).toString().replace("\\", "/");
        if (nam == "")
            continue;
        m_samples.back().load(QString("sfx:snd/%1.pcm").arg(nam), fileDes.value(QString("per%1").arg(i)).toInt());
        m_samples.back().setVolume(fileDes.value(QString("vol%1").arg(i)).toInt()/255.0f);
    }
}


void SampleMap::reset()
{
    m_samples.clear();
}


} // namespace sfx
