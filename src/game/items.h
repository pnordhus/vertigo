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
    };


public:
    Item* get(int model);
    Item* getType(Type type);

public:
    static Items* get();

private:
    void addItem(int model, Type type, const QString &name, const QString &imgname, const QString &txtname);

private:
    QMap<int, Item*> m_items;
};


} // namespace game


#endif // GAME_ITEMS_H
