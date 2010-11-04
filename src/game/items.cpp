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

#include "items.h"
#include "chapter.h"
#include "txt/desfile.h"
#include "gfx/colortable.h"
#include <QFile>

QString trimEnd(const QString& str);

namespace game {

Items *items = NULL;

Items::Items()
{
    addItem(1, Torpedo, "STANLEY I", "stanley2", "stanley1.txt");
    addItem(2, Torpedo, "STANLEY II", "stanley3", "stanley2.txt");
    addItem(3, Torpedo, "THRESHER I", "threshr1", "threshr1.txt");
    addItem(4, Torpedo, "THRESHER II", "threshr2", "threshr2.txt");
    addItem(5, Torpedo, "BULL SHARK I", "bull1", "bull1.txt");
    addItem(6, Torpedo, "BULL SHARK II", "bull2", "bull2.txt");
    addItem(7, Torpedo, "TIGER SHARK I", "tiger1", "tiger1.txt");
    addItem(8, Torpedo, "TIGER SHARK II", "tiger2", "tiger2.txt");
    addItem(9, Torpedo, "MAN EATER I", "maneatr1", "maneatr1.txt");
    addItem(10, Torpedo, "MAN EATER II", "maneatr2", "maneatr2.txt");
    addItem(11, Torpedo, "HAMMERHEAD", "hammerhd", "hammerhd.txt");
    addItem(12, Torpedo, "FLASH SHARK I", "flash1", "flash1.txt");
    addItem(13, Torpedo, "FLASH SHARK II", "flash2", "flash2.txt");
    addItem(14, Torpedo, "FLASH SHARK III", "flash3", "flash3.txt");
    addItem(15, Torpedo, "BIG BANG I", "bigbang2", "bigbang1.txt");
    addItem(16, Torpedo, "BIG BANG II", "bigbang3", "bigbang2.txt");
    addItem(17, Torpedo, "SUBSONIC I", "subsonc1", "subsonc1.txt");
    addItem(18, Torpedo, "SUBSONIC II", "subsonc2", "subsonc2.txt");
    addItem(19, Torpedo, "CLUSTER BOMB", "ripper", "ripper.txt");
    addItem(20, Torpedo, "LEECH I", "leech1", "leech1.txt");
    addItem(21, Torpedo, "LEECH II", "leech2", "leech2.txt");
    addItem(22, Torpedo, "TORCH", "estroker", "estroker.txt");
    addItem(1025, Magazine, "MAGAZINE I", "s02_mag", "s02_mag.txt");
    addItem(1026, Magazine, "MAGAZINE II", "s04_mag", "s04_mag.txt");
    addItem(1027, Magazine, "MAGAZINE III", "s06_mag", "s06_mag.txt");
    addItem(1028, Magazine, "MAGAZINE IV", "s08_mag", "s08_mag.txt");
    addItem(1029, Magazine, "REVOLVER I", "f04_mag", "f04_mag.txt");
    addItem(1030, Magazine, "REVOLVER II", "f06_mag", "f06_mag.txt");
    addItem(1031, Magazine, "REVOLVER III", "f08_mag", "f08_mag.txt");
    addItem(1032, Magazine, "REVOLVER IV", "f10_mag", "f10_mag.txt");
    addItem(3073, Gun, "PLASMA GUN I", "plasgun1", "plasgun1.txt");
    addItem(3074, Gun, "PLASMA GUN II", "plasgun2", "plasgun2.txt");
    addItem(3075, Gun, "PLASMA DEATH I", "plasdth1", "plasdth1.txt");
    addItem(3076, Gun, "PLASMA DEATH II", "plasdth2", "plasdth2.txt");
    addItem(3077, Gun, "PLASMA DEATH III", "plasdth3", "plasdth3.txt");
    addItem(3078, Gun, "DOOM MORTAR", "d-mortar", "d-mortar.txt");
    addItem(3079, Gun, "MAX. IMPACT I", "maximp1", "maximp1.txt");
    addItem(3080, Gun, "MAX. IMPACT II", "maximp2", "maximp2.txt");
    addItem(3081, Gun, "VENDETTA I", "vendett1", "vendett1.txt");
    addItem(3082, Gun, "VENDETTA II", "vendett2", "vendett2.txt");
    addItem(3083, Gun, "VENDETTA III", "vendett3", "vendett3.txt");
    addItem(4097, Engine, "Small Block", "generat1", "generat1.txt");
    addItem(4098, Engine, "Medium Block", "generat2", "generat2.txt");
    addItem(4099, Engine, "Big Block", "generat3", "generat3.txt");
    addItem(4100, Booster, "Booster", "booster", "booster.txt");
    addItem(4101, Silator, "Silator", "silator", "silator.txt");
    addItem(5121, Armor, "Steel Armor", "armor1", "armor1.txt");
    addItem(5122, Armor, "Ceramic Armor", "armor2", "armor2.txt");
    addItem(5123, Armor, "Carbon Armor", "armor3", "armor3.txt");
    addItem(5124, Armor, "Titanium Armor", "armor4", "armor4.txt");
    addItem(5126, NRSkin, "NR-Skin", "nrskin", "nrskin.txt");
    addItem(6145, Sensor, "Spy I", "sonar1", "sonar1.txt");
    addItem(6146, Sensor, "Spy II", "sonar2", "sonar2.txt");
    addItem(6147, Sensor, "Spy III", "sonar3", "sonar3.txt");
    addItem(6148, Sensor, "Spy IV", "sonar4", "sonar4.txt");
    addItem(7169, Software, "TORP. CLAW V1.6", "softw11", "softw11.txt");
    addItem(7170, Software, "TORP. CLAW V2.1", "softw12", "softw12.txt");
    addItem(7171, Software, "TORP. CLAW V3.6", "softw13", "softw13.txt");
    addItem(7172, Software, "PUNISHER V1.0", "softw21", "softw21.txt");
    addItem(7173, Software, "PUNISHER V1.4", "softw22", "softw22.txt");
    addItem(7174, Software, "PUNISHER V2.2", "softw23", "softw23.txt");
    addItem(7175, Software, "ERASER V0.8", "softw31", "softw31.txt");
    addItem(7176, Software, "ERASER V1.0", "softw32", "softw32.txt");
    addItem(7177, Software, "ERASER V1.4", "softw33", "softw33.txt");
    addItem(9217, Buzzer, "Buzzer", "buzzer1", "buzzer1.txt");
    addItem(9218, Fixer, "Fixer", "repair", "repair.txt");

    txt::DesFile file("dat:world/weapcost.des");
    file.setSection("Cost");
    int c;
    for (int c = 0; file.contains(QString("C%1").arg(c)); c++)
    {
        QString str = file.value(QString("C%1").arg(c)).toString();
        str.truncate(str.indexOf(' '));
        Item *item = m_items.value(str.toInt());
        if (item)
            item->cost = file.value(QString("C%1C").arg(c)).toInt();
    }
}


Items::~Items()
{
    qDeleteAll(m_items);
}


void Items::addItem(int model, Type type, const QString &name, const QString &imgname, const QString &txtname)
{
    Item *item = new Item;
    item->model = model;
    item->type = type;
    item->longname = name;
    item->imgname = imgname;

    gfx::ColorTable colorTable;
    if (type == Torpedo || type == Magazine)
        colorTable.loadFromFile("gfx:pal/depot/torpedo.pal");
    else if (type == Gun)
        colorTable.loadFromFile("gfx:pal/depot/gun.pal");
    else
        colorTable.loadFromFile("gfx:pal/depot/specs.pal");
    item->icon = gfx::Image::load(QString("gfx:img/icon/%1.img").arg(imgname), colorTable);

    QFile file("txt:thing/" + txtname);
    if (file.open(QFile::ReadOnly))
    {
        while (!file.atEnd())
            item->text.append(trimEnd(file.readLine()));
    }

    m_items.insert(model, item);
}


Items::Item* Items::get(int model)
{
    if (items == NULL)
        items = new Items();
    return items->m_items.value(model);
}


void Items::insertType(Type type, QList<int> &list)
{
    Station station = Chapter::get()->stations().value(Chapter::get()->currentStation());
    foreach (Item *item, items->m_items.values())
        if (item->type == type && station.depotPrices().contains(item->model))
            list << item->model;
}


QList<int> Items::getDepotItems(const QString &mounting)
{
    if (items == NULL)
        items = new Items();
    QList<int> list;
    if (mounting == "DEFE")
    {
        insertType(Armor, list);
        insertType(NRSkin, list);
        insertType(Sensor, list);
        insertType(Buzzer, list);
        insertType(Fixer, list);
    }
    if (mounting == "GENE")
    {
        insertType(Engine, list);
        insertType(Booster, list);
        insertType(Silator, list);
    }
    if (mounting == "TORP")
    {
        insertType(Magazine, list);
        insertType(Torpedo, list);
    }
    if (mounting == "GUN")
    {
        insertType(Gun, list);
    }
    if (mounting == "TUR1" || mounting == "TUR2")
    {
        insertType(Gun, list);
        insertType(Software, list);
    }
    return list;
}


int Items::getDepotPrice(int model)
{
    if (items == NULL)
        items = new Items();
    Station station = Chapter::get()->stations().value(Chapter::get()->currentStation());
    return (int)(0.9*items->m_items.value(model)->cost*station.depotPrices().value(model));
}


} // namespace game
