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

#ifndef GAME_NOTEBOOK_H
#define GAME_NOTEBOOK_H


#include "gfx/font.h"
#include "ui/button.h"
#include "txt/stringtable.h"


namespace game {


class Notebook : public ui::Label
{
    Q_OBJECT

public:
    Notebook();

signals:
    void close();

private:
    ui::Label* createLabel(ui::Widget *parent, txt::String text, float posY);
    ui::Button* createButton(ui::Widget *parent, txt::String text, float posY);

private:
    void mousePressEvent(const QPointF &pos, Qt::MouseButton button);

private:
    gfx::Font m_fontGreen;
    gfx::Font m_fontYellow;
    ui::Label *m_lblMain;
};


} // namespace game


#endif // GAME_NOTEBOOK_H
