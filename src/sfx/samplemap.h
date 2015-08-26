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

#ifndef SFX_SAMPLEMAP_H
#define SFX_SAMPLEMAP_H


#include "sound.h"
#include <vector>


namespace sfx {


enum class Sample
{
    Sample0,
    Sample1,
    Sample2,
    Sample3,
    Sample4,
    Sample5,
    Sample6,
    Sample7,
    Sample8,
    Sample9,
    Sample10,
    Sample11,
    Sample12,
    Sample13,
    Sample14,
    Sample15,
    Sample16,
    Sample17,
    Sample18,
    Sample19,
    Sample20,
    Sample21,
    Radio,
    Sample23,
    Sample24,
    Sample25,
    Sample26,
    Sample27,
    Sample28,
    Sample29,
    Sample30,
    Sample31,
    Sample32,
    Sample33,
    Sample34,
    Sample35,
    Sample36,
    Sample37,
    Sample38,
    Sample39,
    Sample40,
    Sample41,
    Sample42,
    Sample43,
    Sample44,
    Sample45,
    Sample46,
    Sample47,
    Sample48,
    Sample49,
    Sample50,
    Sample51,
    Sample52,
    Sample53,
    Sample54,
    Sample55,
    Sample56,
    Sample57,
    Sample58,
    Sample59,
    Sample60,
    Sample61,
    Sample62,
    Sample63,
    SonarActivated,
    SonarDeactivated,
    Sample66,
    Sample67,
    TargetIdentified,
    TargetDestroyed,
    Sample70,
    TargetLost,
    Sample72,
    Sample73,
    Sample74,
    Sample75,
    Sample76,
    Sample77,
    MissionAccomplished,
    Autopilot,
    NavPoint,
    Sample81,
    Sample82,
    MissionFailed,
};


class SampleMap
{
public:
    static void load();
    static void reset();
    static Sound& get(Sample sample) { return m_samples[static_cast<int>(sample)]; }

private:
    static std::vector<Sound> m_samples;
};


} // namespace sfx


#endif // SFX_SAMPLEMAP_H
