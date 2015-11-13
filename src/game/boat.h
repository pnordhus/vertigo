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

#ifndef GAME_BOAT_H
#define GAME_BOAT_H


#include <QString>
#include <QPoint>
#include <vector>
#include <set>
#include "txt/desfile.h"
#include "util/geometry2d.h"
#include "util/geometry3d.h"


namespace game {


class Boat
{
public:
    Boat(txt::DesFile &file);

public:
    struct Mounting
    {
        QString name;
        int side;
        int type;
        Point pos;
        QString dir;
        Vector3D rel;
    };
    enum Defect
    {
        DefectGun,
        DefectToMa,
        DefectFArm,
        DefectLArm,
        DefectRArm,
        DefectBArm,
        DefectTur1,
        DefectTur2
    };

public:
    int type() const { return m_type; }
    const QString& name() const { return m_name; }
    const QString& cockpit() const { return m_cockpit; }
    const QString& moviePrefix() const { return m_moviePrefix; }
    txt::DesFile& boatFile() { return m_boatFile; }

    const std::vector<Mounting>& mountings() const { return m_mountings; }
    const Mounting* getMounting(const QString& mounting);

    std::vector<int> getItems(const QString& mounting);
    bool canBuy(int model, const QString& mounting, int *oldModel);
    bool canSell(int model, const QString& mounting);
    bool isCompatible(int model);
    int repairState(int model, const QString& mounting);
    int repairCost(int model, const QString& mounting);
    void repair(int model, const QString& mounting, float amount);
    void buy(int model, const QString& mounting);
    void sell(int model, int index, const QString& mounting);

    void upgrade(int type);

    const std::vector<int>& torpedoes() const { return m_torpedoes; }
    int magazine() const { return m_magazine; }
    int gun() const { return m_gun; }
    int engine() const { return m_engine; }
    int booster() const { return m_booster; }
    int silator() const { return m_silator; }
    int armor() const { return m_armor; }
    int nrskin() const { return m_nrskin; }
    int sensor() const { return m_sensor; }
    int tur1() const { return m_tur1; }
    int tur1soft() const { return m_tur1soft; }
    int tur2() const { return m_tur2; }
    int tur2soft() const { return m_tur2soft; }
    const std::vector<int>& buzzers() const { return m_buzzers; }
    int fixer() const { return m_fixer; }

    const float* defects() const { return m_defects; }

private:
    void load();
    void addMounting(const QString &name, int side, int type, int x, int y, const QString &dir);
    int getCapacity(int magazine);

private:
    int m_type;
    QString m_name;
    QString m_cockpit;
    QString m_moviePrefix;
    txt::DesFile m_boatFile;

    std::vector<Mounting> m_mountings;
    std::set<int> m_compatibility;
    int m_maxBuzzers;

    std::vector<int> m_torpedoes;
    int m_magazine;
    int m_gun;
    int m_engine;
    int m_booster;
    int m_silator;
    int m_armor;
    int m_nrskin;
    int m_sensor;
    int m_tur1;
    int m_tur1soft;
    int m_tur2;
    int m_tur2soft;
    std::vector<int> m_buzzers;
    int m_fixer;

    float m_defects[8];
};


} // namespace game


#endif // GAME_BOAT_H
