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


#ifndef UI_ARROW_H
#define UI_ARROW_H


#include "button.h"


namespace ui {


class Arrow : public QObject
{
    Q_OBJECT

public:
    Arrow(const QString &dir, const QPoint &pos, bool large, Widget *parent);
    ~Arrow();

public:
    void hide();
    void show();
    void setValue(int value);
    void setText(const QString &text);

signals:
    void clicked(int);

private slots:
    void clicked();

private:
    Label *m_label;
    Button *m_button;
    gfx::Font m_font;
    bool m_left;
    bool m_top;
    QPoint m_position;
    int m_value;
};


} // namespace ui


#endif // UI_ARROW_H
