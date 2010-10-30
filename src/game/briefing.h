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

#ifndef GAME_BRIEFING_H
#define GAME_BRIEFING_H


#include "menu.h"
#include "gfx/texture.h"
#include "sfx/sound.h"
#include "ui/label.h"
#include <QTime>


namespace game {


class Briefing : public Menu
{
    Q_OBJECT

private:
    enum State { Init, Text, Targets, Hints, Arrow, PressKey };

public:
    Briefing();

signals:
    void startEngine();

private:
    void activate();
    void deactivate();
    void draw();
    void keyPressEvent(QKeyEvent *);

private:
    gfx::Font m_font;
    sfx::Sound m_backgroundSound;
    sfx::Sound m_openSound;
    sfx::Sound m_woopSound;
    ui::Label m_background;
    ui::Label *m_lblMain;
    ui::Label *m_lblMap;
    ui::Label *m_lblArrow;
    ui::Label *m_lblPressKey;

    QTime m_time;
    State m_state;
    bool m_toggleState;
    int m_nextLine;
};


} // namespace game


#endif // GAME_BRIEFING_H
