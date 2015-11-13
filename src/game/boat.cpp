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

#include "boat.h"
#include "items.h"
#include <algorithm>


namespace game {


Boat::Boat(txt::DesFile &file) : 
    m_nrskin(0),
    m_fixer(0),
    m_booster(0),
    m_silator(0),
    m_tur1(0),
    m_tur1soft(0),
    m_tur2(0),
    m_tur2soft(0)
{
    file.setSection("Ship");
    m_type = file.value("type").toInt();
    load();

    for (int i = 0; i < m_mountings.size(); i++)
    {
        const Mounting &mounting = m_mountings[i];
        file.setSection(QString("ShipMounting%1").arg(i));
        if (file.contains("armor"))
            buy(file.value("armor").toInt(), mounting.name);
        if (file.contains("nrs"))
            buy(5126, mounting.name);
        if (file.contains("sensor"))
            buy(file.value("sensor").toInt(), mounting.name);
        if (file.contains("fixer"))
            buy(9218, mounting.name);
        for (int buzzer = 0; file.contains(QString("buzzer%1").arg(buzzer)); buzzer++)
            buy(9217, mounting.name);
        if (file.contains("engine"))
            buy(file.value("engine").toInt(), mounting.name);
        if (file.contains("booster"))
            buy(4100, mounting.name);
        if (file.contains("silator"))
            buy(4101, mounting.name);
        if (file.contains("model"))
            buy(file.value("model").toInt(), mounting.name);
        for (int torpedo = 0; file.contains(QString("torpedo%1").arg(torpedo)); torpedo++)
            buy(file.value(QString("torpedo%1").arg(torpedo)).toInt(), mounting.name);
        if (file.contains("software"))
            buy(file.value("software").toInt(), mounting.name);
    }

    file.setSection("defect");
    m_defects[DefectGun] = file.value("gun", 1.0f).toFloat();
    m_defects[DefectToMa] = file.value("toma", 1.0f).toFloat();
    m_defects[DefectFArm] = file.value("farm", 1.0f).toFloat();
    m_defects[DefectLArm] = file.value("larm", 1.0f).toFloat();
    m_defects[DefectRArm] = file.value("rarm", 1.0f).toFloat();
    m_defects[DefectBArm] = file.value("barm", 1.0f).toFloat();
    m_defects[DefectTur1] = file.value("tur1", 1.0f).toFloat();
    m_defects[DefectTur2] = file.value("tur2", 1.0f).toFloat();
}


void Boat::load()
{
    if (m_type == 2049)
    {
        m_boatFile.load("vfx:sobjects/hiob.des");
        m_moviePrefix = "hiob";
        m_maxBuzzers = 2;

        addMounting("DEFE", 0, 3, 280, 173, "TopLeft");
        addMounting("GENE", 0, 4, 80, 132, "BottomRight");
        addMounting("GUN", 1, 1, 324, 188, "BottomLeft");
        addMounting("TORP", 1, 0, 276, 129, "TopLeft");
    }
    if (m_type == 2050)
    {
        m_boatFile.load("vfx:sobjects/gator.des");
        m_moviePrefix = "gator";
        m_maxBuzzers = 2;

        addMounting("DEFE", 0, 3, 240, 183, "BottomLeft");
        addMounting("GENE", 0, 4, 56, 163, "TopRight");
        addMounting("GUN", 1, 1, 311, 163, "TopLeft");
        addMounting("TORP", 1, 0, 175, 148, "BottomRight");
        addMounting("TUR1", 1, 2, 90, 175, "TopRight");
    }
    if (m_type == 2051)
    {
        m_boatFile.load("vfx:sobjects/zorn.des");
        m_moviePrefix = "zorn";
        m_maxBuzzers = 2;

        addMounting("DEFE", 0, 3, 217, 194, "BottomLeft");
        addMounting("GENE", 0, 4, 116, 175, "BottomLeft");
        addMounting("TUR2", 0, 2, 132, 129, "BottomRight");
        addMounting("GUN", 1, 1, 235, 180, "TopLeft");
        addMounting("TORP", 1, 0, 251, 211, "BottomLeft");
        addMounting("TUR1", 1, 2, 119, 146, "TopRight");
    }
    if (m_type == 2052)
    {
        m_boatFile.load("vfx:sobjects/succubus.des");
        m_moviePrefix = "succub";
        m_maxBuzzers = 2;

        addMounting("DEFE", 0, 3, 216, 191, "BottomLeft");
        addMounting("GENE", 0, 4, 155, 143, "TopRight");
        addMounting("TUR2", 0, 2, 106, 175, "BottomRight");
        addMounting("GUN", 1, 1, 253, 186, "BottomLeft");
        addMounting("TORP", 1, 0, 253, 161, "TopLeft");
        addMounting("TUR1", 1, 2, 143, 174, "BottomLeft");
    }

    m_boatFile.setSection("Ship");
    m_name = m_boatFile.value("Name").toString();
    m_boatFile.setSection("Cockpit");
    m_cockpit = m_boatFile.value("Name").toString();

    m_boatFile.setSection("Compatibility");
    for (int comp = 0; m_boatFile.contains(QString("Comp%1").arg(comp)); comp++)
    {
        QString str = m_boatFile.value(QString("Comp%1").arg(comp)).toString();
        str.truncate(str.indexOf(' '));
        int model = str.toInt();
        if (model != 99999)
            m_compatibility.insert(model);
    }
}


void Boat::addMounting(const QString &name, int side, int type, int x, int y, const QString &dir)
{
    Vector3D rel;
    m_boatFile.setSection(QString("shipmounting%1").arg(m_mountings.size()));
    if (m_boatFile.contains("relativex"))
        rel = Vector3D(m_boatFile.value("relativex").toFloat()/32, m_boatFile.value("relativey").toFloat()/32, m_boatFile.value("relativez").toFloat()/32);
    if (m_boatFile.contains("relative0x"))
        rel = Vector3D(m_boatFile.value("relative0x").toFloat()/32, m_boatFile.value("relative0y").toFloat()/32, m_boatFile.value("relative0z").toFloat()/32);

    m_mountings.emplace_back();
    Mounting &mounting = m_mountings.back();
    mounting.name = name;
    mounting.side = side;
    mounting.type = type;
    mounting.pos = Point(x, y);
    mounting.dir = dir;
    mounting.rel = rel;
}


const Boat::Mounting* Boat::getMounting(const QString& mounting)
{
    auto it = std::find_if(m_mountings.begin(), m_mountings.end(), [&mounting](const Mounting &x) { return x.name == mounting; });
    if (it != m_mountings.end())
        return &(*it);
    return nullptr;
}


void Boat::upgrade(int type)
{
    m_type = type;
    m_mountings.clear();
    m_compatibility.clear();
    load();
}


int Boat::getCapacity(int magazine)
{
    if (magazine == 1025)
        return 4;
    if (magazine == 1026)
        return 6;
    if (magazine == 1027)
        return 8;
    if (magazine == 1028)
        return 10;
    if (magazine == 1029)
        return 6;
    if (magazine == 1030)
        return 8;
    if (magazine == 1031)
        return 10;
    if (magazine == 1032)
        return 12;
    return 0;
}


std::vector<int> Boat::getItems(const QString& mounting)
{
    std::vector<int> list;
    if (mounting == "DEFE")
    {
        list.push_back(m_armor);
        if (m_nrskin)
            list.push_back(m_nrskin);
        list.push_back(m_sensor);
        if (m_fixer)
            list.push_back(m_fixer);
        foreach (int buzz, m_buzzers)
            list.push_back(buzz);
    }
    if (mounting == "GENE")
    {
        list.push_back(m_engine);
        if (m_booster)
            list.push_back(m_booster);
        if (m_silator)
            list.push_back(m_silator);
    }
    if (mounting == "GUN")
    {
        list.push_back(m_gun);
    }
    if (mounting == "TORP")
    {
        list.push_back(m_magazine);
        foreach (int torp, m_torpedoes)
            list.push_back(torp);
    }
    if (mounting == "TUR1")
    {
        if (m_tur1)
            list.push_back(m_tur1);
        if (m_tur1soft)
            list.push_back(m_tur1soft);
    }
    if (mounting == "TUR2")
    {
        if (m_tur2)
            list.push_back(m_tur2);
        if (m_tur2soft)
            list.push_back(m_tur2soft);
    }
    return list;
}


bool Boat::canBuy(int model, const QString& mounting, int *oldModel)
{
    Items::Item *item = Items::get(model);
    *oldModel = -1;
    if (mounting == "DEFE")
    {
        if (item->type == Items::Armor)
            *oldModel = m_armor;
        if (item->type == Items::NRSkin)
            *oldModel = m_nrskin;
        if (item->type == Items::Sensor)
            *oldModel = m_sensor;
        if (item->type == Items::Buzzer && m_buzzers.size() < m_maxBuzzers)
            return true;
        if (item->type == Items::Fixer)
            *oldModel = m_fixer;
    }
    if (mounting == "GENE")
    {
        if (item->type == Items::Engine)
            *oldModel = m_engine;
        if (item->type == Items::Booster)
            *oldModel = m_booster;
        if (item->type == Items::Silator)
            *oldModel = m_silator;
    }
    if (mounting == "GUN")
    {
        if (item->type == Items::Gun)
            *oldModel = m_gun;
    }
    if (mounting == "TORP")
    {
        if (item->type == Items::Magazine)
        {
            *oldModel = m_magazine;
            if (getCapacity(model) < m_torpedoes.size())
                return false;
        }
        if (item->type == Items::Torpedo)
        {
            if (m_torpedoes.size() < getCapacity(m_magazine))
                return true;
        }
    }
    if (mounting == "TUR1")
    {
        if (item->type == Items::Gun)
            *oldModel = m_tur1;
        if (item->type == Items::Software)
            *oldModel = m_tur1soft;
    }
    if (mounting == "TUR2")
    {
        if (item->type == Items::Gun)
            *oldModel = m_tur2;
        if (item->type == Items::Software)
            *oldModel = m_tur2soft;
    }
    if (*oldModel == 0)
        return true;
    if (*oldModel > 0 && *oldModel != model)
        return true;
    return false;
}


bool Boat::canSell(int model, const QString& mounting)
{
    Items::Item *item = Items::get(model);
    if (mounting == "DEFE")
    {
        if (item->type == Items::NRSkin)
            return true;
        if (item->type == Items::Buzzer)
            return true;
        if (item->type == Items::Fixer)
            return true;
    }
    if (mounting == "GENE")
    {
        if (item->type == Items::Booster)
            return true;
        if (item->type == Items::Silator)
            return true;
    }
    if (mounting == "GUN")
    {
    }
    if (mounting == "TORP")
    {
        if (item->type == Items::Torpedo)
            return true;
    }
    if (mounting == "TUR1")
    {
        if (item->type == Items::Gun && m_defects[DefectTur1] == 1.0f)
            return true;
        if (item->type == Items::Software)
            return true;
    }
    if (mounting == "TUR2")
    {
        if (item->type == Items::Gun && m_defects[DefectTur2] == 1.0f)
            return true;
        if (item->type == Items::Software)
            return true;
    }
    return false;
}


bool Boat::isCompatible(int model)
{
    return m_compatibility.find(model) != m_compatibility.end();
}


int Boat::repairState(int model, const QString& mounting)
{
    if (model == 0)
        return 100;
    Items::Item *item = Items::get(model);
    float state = 100;
    if (mounting == "DEFE")
    {
        if (item->type == Items::Armor)
        {
            state = m_defects[DefectFArm]*25;
            state += m_defects[DefectLArm]*25;
            state += m_defects[DefectRArm]*25;
            state += m_defects[DefectBArm]*25;
        }
    }
    if (mounting == "GENE")
    {
    }
    if (mounting == "GUN")
    {
        if (item->type == Items::Gun)
            state = m_defects[DefectGun]*100;
    }
    if (mounting == "TORP")
    {
        if (item->type == Items::Magazine)
            state = m_defects[DefectToMa]*100;
    }
    if (mounting == "TUR1")
    {
        if (item->type == Items::Gun)
            state = m_defects[DefectTur1]*100;
    }
    if (mounting == "TUR2")
    {
        if (item->type == Items::Gun)
            state = m_defects[DefectTur2]*100;
    }
    return static_cast<int>(state);
}


int Boat::repairCost(int model, const QString& mounting)
{
    if (model == 0)
        return 0;
    Items::Item *item = Items::get(model);
    int price = Items::getDepotPrice(model);
    float cost = 0;
    if (mounting == "DEFE")
    {
        if (item->type == Items::Armor)
        {
            cost += (1.0f - m_defects[DefectFArm])*price/32;
            cost += (1.0f - m_defects[DefectLArm])*price/32;
            cost += (1.0f - m_defects[DefectRArm])*price/32;
            cost += (1.0f - m_defects[DefectBArm])*price/32;
        }
    }
    if (mounting == "GENE")
    {
    }
    if (mounting == "GUN")
    {
        if (item->type == Items::Gun)
            cost = (1.0f - m_defects[DefectGun])*price/40;
    }
    if (mounting == "TORP")
    {
        if (item->type == Items::Magazine)
            cost = (1.0f - m_defects[DefectToMa])*price/18.2f;
    }
    if (mounting == "TUR1")
    {
        if (item->type == Items::Gun)
            cost = (1.0f - m_defects[DefectTur1])*price/40;
    }
    if (mounting == "TUR2")
    {
        if (item->type == Items::Gun)
            cost = (1.0f - m_defects[DefectTur2])*price/40;
    }
    return static_cast<int>(cost);
}


void Boat::repair(int model, const QString& mounting, float amount)
{
    Items::Item *item = Items::get(model);
    if (mounting == "DEFE")
    {
        if (item->type == Items::Armor)
        {
            m_defects[DefectFArm] = amount == 1.0f ? 1.0f : m_defects[DefectFArm]*(1 - amount) + amount;
            m_defects[DefectLArm] = amount == 1.0f ? 1.0f : m_defects[DefectLArm]*(1 - amount) + amount;
            m_defects[DefectRArm] = amount == 1.0f ? 1.0f : m_defects[DefectRArm]*(1 - amount) + amount;
            m_defects[DefectBArm] = amount == 1.0f ? 1.0f : m_defects[DefectBArm]*(1 - amount) + amount;
        }
    }
    if (mounting == "GENE")
    {
    }
    if (mounting == "GUN")
    {
        if (item->type == Items::Gun)
            m_defects[DefectGun] = amount == 1.0f ? 1.0f : m_defects[DefectGun]*(1 - amount) + amount;
    }
    if (mounting == "TORP")
    {
        if (item->type == Items::Magazine)
            m_defects[DefectToMa] = amount == 1.0f ? 1.0f : m_defects[DefectToMa]*(1 - amount) + amount;
    }
    if (mounting == "TUR1")
    {
        if (item->type == Items::Gun)
            m_defects[DefectTur1] = amount == 1.0f ? 1.0f : m_defects[DefectTur1]*(1 - amount) + amount;
    }
    if (mounting == "TUR2")
    {
        if (item->type == Items::Gun)
            m_defects[DefectTur2] = amount == 1.0f ? 1.0f : m_defects[DefectTur2]*(1 - amount) + amount;
    }
}


void Boat::buy(int model, const QString& mounting)
{
    Items::Item *item = Items::get(model);

    if (mounting == "DEFE")
    {
        if (item->type == Items::Armor)
            m_armor = model;
        if (item->type == Items::NRSkin)
            m_nrskin = model;
        if (item->type == Items::Sensor)
            m_sensor = model;
        if (item->type == Items::Buzzer)
            m_buzzers.push_back(model);
        if (item->type == Items::Fixer)
            m_fixer = model;
    }
    if (mounting == "GENE")
    {
        if (item->type == Items::Engine)
            m_engine = model;
        if (item->type == Items::Booster)
            m_booster = model;
        if (item->type == Items::Silator)
            m_silator = model;
    }
    if (mounting == "GUN")
    {
        if (item->type == Items::Gun)
            m_gun = model;
    }
    if (mounting == "TORP")
    {
        if (item->type == Items::Magazine)
            m_magazine = model;
        if (item->type == Items::Torpedo)
            m_torpedoes.push_back(model);
    }
    if (mounting == "TUR1")
    {
        if (item->type == Items::Gun)
            m_tur1 = model;
        if (item->type == Items::Software)
            m_tur1soft = model;
    }
    if (mounting == "TUR2")
    {
        if (item->type == Items::Gun)
            m_tur2 = model;
        if (item->type == Items::Software)
            m_tur2soft = model;
    }
}


void Boat::sell(int model, int index, const QString& mounting)
{
    Items::Item *item = Items::get(model);
    if (mounting == "DEFE")
    {
        if (item->type == Items::NRSkin)
            m_nrskin = 0;
        if (item->type == Items::Buzzer)
            m_buzzers.erase(m_buzzers.begin() + index);
        if (item->type == Items::Fixer)
            m_fixer = 0;
    }
    if (mounting == "GENE")
    {
        if (item->type == Items::Booster)
            m_booster = 0;
        if (item->type == Items::Silator)
            m_silator = 0;
    }
    if (mounting == "GUN")
    {
    }
    if (mounting == "TORP")
    {
        if (item->type == Items::Torpedo)
            m_torpedoes.erase(m_torpedoes.begin() + index);
    }
    if (mounting == "TUR1")
    {
        if (item->type == Items::Gun)
            m_tur1 = 0;
        if (item->type == Items::Software)
            m_tur1soft = 0;
    }
    if (mounting == "TUR2")
    {
        if (item->type == Items::Gun)
            m_tur2 = 0;
        if (item->type == Items::Software)
            m_tur2soft = 0;
    }
}


} // namespace game
