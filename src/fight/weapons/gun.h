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

#ifndef FIGHT_GUN_H
#define FIGHT_GUN_H


#include <QString>
#include <vector>
#include "util/geometry3d.h"
#include "sfx/samplemap.h"


namespace fight {


class Scenario;
enum class Effects;


class Gun
{
public:
    enum State { StateReady, StateRearming, StateFiring, StateJammed, StateEmpty, StateEnergyDelay };
    struct GunState
    {
        QString longName;
        QString shortName;
    };
    static GunState* GunStates;

public:
    Gun(Scenario *scenario, int model);

public:
    int model() const { return m_model; }
    float energyDemand() const { return m_energyDemand; }
    sfx::Sample sample() const { return m_sample; }

    float defect() const { return m_defect; }
    int ammo() const { return m_ammo; }
    State state() const { return m_state; }

    void setDefect(float defect) { m_defect = defect; }
    void setAmmo(int ammo) { m_ammo = ammo; }
    void addMuzzle(const Vector3D &muzzle);
    void addMuzzles(const Vector3D &muzzle, int model);

public:
    bool update(float elapsedTime);
    bool fire(const Vector3D &pos, const Vector3D &dir, const Vector3D &up, const Vector3D &left);

protected:
    Scenario *m_scenario;

    int m_model;
    QString m_longName;
    QString m_shortName;
    QString m_imageName;

    float m_rearmingTime;
    float m_energyDemand;
    float m_defectDamping;
    float m_defectDelay;

    float m_defect;
    int m_ammo;
    State m_state;
    float m_stateTime;

    Effects m_projectile;
    std::vector<Vector3D> m_muzzles;
    sfx::Sample m_sample;
};


} // namespace fight


#endif // FIGHT_GUN_H
