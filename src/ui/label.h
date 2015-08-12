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

#ifndef UI_LABEL_H
#define UI_LABEL_H


#include "widget.h"
#include "gfx/font.h"
#include <QRectF>
#include <QString>


namespace ui {


class Label : public Widget
{
public:
    enum AlignmentFlags
    {
        AlignNone = 0,
        AlignHCenter = 1,
        AlignBottom = 2,
    };
    Q_DECLARE_FLAGS(Alignment, AlignmentFlags);

public:
    Label(Widget *parent = NULL, bool visible = true);
    Label(const gfx::Font &font, Widget *parent = NULL);

public:
    void setAlignment(Alignment alignment);
    void setFont(const gfx::Font &font);
    void setText(const QString &text);
    void setTexture(const gfx::Texture &texture);

public:
    gfx::Texture texture() const { return m_texture; }

protected:
    void draw();

protected:
    gfx::Font m_font;
    util::Rect m_drawRect;
    QString m_text;
    gfx::Texture m_texture;
    Alignment m_alignment;
};


Q_DECLARE_OPERATORS_FOR_FLAGS(Label::Alignment);


} // namespace ui


#endif // UI_LABEL_H
