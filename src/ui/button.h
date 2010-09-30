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

#ifndef UI_BUTTON_H
#define UI_BUTTON_H


#include "label.h"


namespace ui {


class Button : public Label
{
    Q_OBJECT

public:
    Button();

signals:
    void clicked();

protected:
    void draw();
    void mousePressEvent(const QPointF &pos, Qt::MouseButton button);
    void mouseReleaseEvent(const QPointF &pos, Qt::MouseButton button);

private:
    bool m_pressed;
};


} // namespace ui


#endif // UI_BUTTON_H
