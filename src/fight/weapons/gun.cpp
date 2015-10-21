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

#include "gun.h"
#include "fight/scenario.h"
#include "txt/desfile.h"
#include "fight/effects/effectmanager.h"
#include "fight/effects/billboard.h"


namespace fight {


Gun::GunState* Gun::GunStates = nullptr;


Gun::Gun(Scenario *scenario, int model) :
    m_scenario(scenario),
    m_defect(1.0f),
    m_state(StateReady)
{
    txt::DesFile file("vfx:sobjects/globweap.des");

    if (GunStates == nullptr)
    {
        GunStates = new Gun::GunState[6];
        for (int i = 0; i < 6; i++)
        {
            file.setSection(QString("gunstate%1").arg(i));
            GunStates[i].longName = file.value("verbose").toString();
            GunStates[i].shortName = file.value("short").toString();
        }
    }

    file.setSection(QString("gun%1").arg(model));
    m_longName = file.value("longname").toString();
    m_shortName = file.value("shortname").toString();
    m_imageName = file.value("imagename").toString();
    m_rearmingTime = file.value("rearmingtime").toFloat() + 0.03f;
    m_energyDemand = file.value("energydemand").toFloat();
    m_defectDamping = file.value("defectdamping").toFloat();
    m_defectDelay = file.value("defectdelay").toFloat();

    m_ammo = file.value("defaultammo").toInt();

    static Effects effects[14] = { Effects::Shoot_PlasmaGun, Effects::Shoot_PlasmaGun, Effects::Shoot_PlasmaDeath, Effects::Shoot_PlasmaDeath, Effects::Shoot_PlasmaDeath, Effects::Shoot_DoomMortar, Effects::Shoot_MaximumImpactI, Effects::Shoot_MaximumImpact2, Effects::Shoot_Vendetta, Effects::Shoot_Vendetta, Effects::Shoot_Vendetta, Effects::Shoot_MaximumImpactI, Effects::Shoot_MaximumImpact2, Effects::Shoot_Vendetta };
    m_projectile = effects[model];

    m_range = m_scenario->effectManager().getBillboard(m_projectile)->range();

    static sfx::Sample samples[14] = { sfx::Sample::Shoot_PlasmaGun, sfx::Sample::Shoot_PlasmaGun, sfx::Sample::Shoot_PlasmaDeath, sfx::Sample::Shoot_PlasmaDeath, sfx::Sample::Shoot_PlasmaDeath, sfx::Sample::Shoot_DoomMortar, sfx::Sample::Shoot_MaximumImpact, sfx::Sample::Shoot_MaximumImpact, sfx::Sample::Shoot_Vendetta, sfx::Sample::Shoot_Vendetta, sfx::Sample::Shoot_Vendetta, sfx::Sample::Shoot_MaximumImpact, sfx::Sample::Shoot_MaximumImpact, sfx::Sample::Shoot_Vendetta };
    m_sample = samples[model];
}


void Gun::addMuzzle(const Vector3D &muzzle)
{
    m_muzzles.push_back(muzzle);
}


void Gun::addMuzzles(const Vector3D &muzzle, int model)
{
    if (model == 3073 || model == 3075 || model == 3078)
        addMuzzle(muzzle);
    else if (model == 3082)
    {
        addMuzzle(muzzle + Vector3D(0, -1, 0));
        addMuzzle(muzzle + Vector3D(0, 1, 0));
        addMuzzle(muzzle + Vector3D(0, 0, 1));
        m_ammo *= 3;
    }
    else if (model == 3077 || model == 3083)
    {
        addMuzzle(muzzle + Vector3D(0, -1, 0));
        addMuzzle(muzzle + Vector3D(0, 1, 0));
        addMuzzle(muzzle + Vector3D(0, -1, -1));
        addMuzzle(muzzle + Vector3D(0, 1, -1));
        m_ammo *= 4;
    }
    else
    {
        addMuzzle(muzzle + Vector3D(0, -1, 0));
        addMuzzle(muzzle + Vector3D(0, 1, 0));
        m_ammo *= 2;
    }
}


bool Gun::update(float elapsedTime)
{
    if (m_state == StateJammed && m_stateTime < m_scenario->time())
    {
        m_state = StateRearming;
        m_stateTime = m_scenario->time() + m_rearmingTime*1000;
    }
    if (m_state == StateRearming && m_stateTime < m_scenario->time())
        m_state = StateReady;
    if (m_state == StateReady && m_energyDemand == 0.0f && m_ammo == 0)
        m_state = StateEmpty;
    return false;
}


void Gun::fire(const Vector3D &pos, const Vector3D &dir, const Vector3D &up, const Vector3D &left)
{
    if (m_state != StateReady)
        return;
    m_state = StateFiring;
    bool jammed = false;
    float defect = (1.0f - (m_defect < 0.2f ? 0.2f : m_defect)/m_defectDamping)/m_muzzles.size(); // TODO: check formula
    for (int i = 0; i < m_muzzles.size() && !jammed; i++)
    {
        if (defect > 0 && defect > static_cast<float>(qrand())/RAND_MAX)
            jammed = true;
        else
        {
            m_scenario->effectManager().addProjectile(m_projectile, pos + dir*m_muzzles[i].x + left*m_muzzles[i].y + up*m_muzzles[i].z, dir);
            if (m_energyDemand == 0.0f)
                m_ammo--;
            sfx::SampleMap::get(m_sample).playInstance();
        }
    }
    if (jammed)
    {
        m_state = StateJammed;
        m_stateTime = m_scenario->time() + m_defectDelay*1000;
    }
    else
    {
        m_state = StateRearming;
        m_stateTime = m_scenario->time() + m_rearmingTime*1000;
    }
}


} // namespace fight
