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

#ifndef TXT_STRINGTABLE_H
#define TXT_STRINGTABLE_H


#include <QStringList>


namespace txt {


enum String
{
    Text0,
    Text1,
    Text2,
    MainMenu_NewGame,
    MainMenu_QuitGame,
    Text5,
    Text6,
    Text7,
    Text8,
    Text9,
    Text10,
    Text11,
    Text12,
    Text13,
    Notebook_Title,
    Notebook_TitleLine,
    Notebook_Missions,
    Notebook_LoadSave,
    Notebook_Map,
    Notebook_Options,
    Notebook_Back,
    Text21,
    Text22,
    Text23,
    Text24,
    Text25,
    Text26,
    Text27,
    Text28,
    Text29,
    Text30,
    Text31,
    Text32,
    Text33,
    Text34,
    Text35,
    Text36,
    Text37,
    Text38,
    Text39,
    Text40,
    Text41,
    Text42,
    Text43,
    Text44,
    Text45,
    Text46,
    Text47,
    Text48,
    Text49,
    Text50,
    Text51,
    Text52,
    Text53,
    Text54,
    Text55,
    Text56,
    Text57,
    Text58,
    Text59,
    Text60,
    Text61,
    Text62,
    Text63,
    Text64,
    Text65,
    Text66,
    Text67,
    Text68,
    Text69,
    Text70,
    Text71,
    Text72,
    Text73,
    Text74,
    Text75,
    Text76,
    Text77,
    Text78,
    Text79,
    Text80,
    Text81,
    Text82,
    Text83,
    Text84,
    Text85,
    Text86,
    Text87,
    Text88,
    Text89,
    Text90,
    Text91,
    Text92,
    Text93,
    Text94,
    Text95,
    Text96,
    Text97,
    Text98,
    Text99,
    Text100,
    Notebook_QuitGame,
    Text102,
    Text103,
    Text104,
    Text105,
    Text106,
    Text107,
    Text108,
    Text109,
    MainMenu_Load,
    MainMenu_Credits,
    MainMenu_Training,
    Text113,
    Text114,
    Text115,
    Text116,
    Text117,
    MainMenu,
    Text119,
    Text120,
    Text121,
    Text122,
    Text123,
    Text124,
    Text125,
    Text126,
    Text127,
    Text128,
    Text129,
    Text130,
    Text131,
    Text132,
    Text133,
    Text134,
    Text135,
    Text136,
    Text137,
    Text138,
    Text139,
    Text140,
    Text141,
    Text142,
    Text143,
    Text144,
    Text145,
    Text146,
    Text147,
    Text148,
    Text149,
    Text150,
    Text151,
    Text152,
    Text153,
    Text154,
    Text155,
    Text156,
    Text157,
    Text158,
    Text159,
    Text160,
    Text161,
    Text162,
    Text163,
    Text164,
    Text165,
    Text166,
    Text167,
    Text168,
    Text169,
    Text170,
    Text171,
    Text172,
    Text173,
    Text174,
    Text175,
    Text176,
    Text177,
    Text178,
    Text179,
    Text180,
    Text181,
    Text182,
    Text183,
    Text184,
    Text185,
    Text186,
    Text187,
    Text188,
    Text189,
    Text190,
};


class StringTable
{
public:
    static bool load();
    static QString get(String string);

private:
    static QStringList m_table;
};


} // namespace txt


#endif // TXT_STRINGTABLE_H
