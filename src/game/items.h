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

#ifndef GAME_ITEMS_H
#define GAME_ITEMS_H


#include "station.h"
#include "gfx/image.h"
#include <QString>
#include <QStringList>
#include <QMap>


namespace game {


class Items
{
public:
    enum Type { Torpedo, Magazine, Gun, Engine, Booster, Silator, Armor, NRSkin, Sensor, Software, Buzzer, Fixer };

public:
    Items();
    ~Items();

public:
    struct Item
    {
        int model;
        Type type;
        int cost;
        QString longname;
        QString imgname;
        QStringList text;
        gfx::Image icon;
    };


public:
    static Item* get(int model);
    static QList<int> getDepotItems(const QString &mounting);
    static int getDepotPrice(int model);

private:
    void addItem(int model, Type type, const QString &name, const QString &imgname, const QString &txtname);
    static void insertType(Type type, QList<int> &list);

private:
    QMap<int, Item*> m_items;
};


} // namespace game


#endif // GAME_ITEMS_H
