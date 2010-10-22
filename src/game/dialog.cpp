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

#include "dialog.h"
#include "gfx/colortable.h"
#include "txt/desfile.h"
#include "txt/stringtable.h"
#include <QFile>


namespace game {


Dialog::Dialog(int id, ui::Widget *parent) :
    ui::Widget(parent),
    m_option(NULL)
{
    m_fontTop.load("gfx:fnt/dpsmamon.fnt", gfx::ColorTable() << 0xffb89c00, true);
    m_fontBottom.load("gfx:fnt/dpsmamon.fnt", gfx::ColorTable() << 0xff00a8d0, true);
    m_fontHighlight.load("gfx:fnt/dpsmamon.fnt", gfx::ColorTable() << 0xff00e4f8, true);

    const QString baseName = QString("txt:dia/%1/%2").arg(id / 1000, 3, 10, QChar('0')).arg(id, 6, 10, QChar('0'));

    loadTree(baseName + ".dia");
    loadStrings(baseName + ".sen");

    txt::DesFile file(baseName + ".des");

    file.beginGroup("Person");

    const QString personName = file.value("Name").toString();

    file.endGroup();

    select(1);
}


void Dialog::clicked()
{
    const int next = sender()->property("next").toInt();
    select(next);
}


void Dialog::select(int index)
{
    if (index >= 0)
        m_current = index;
    m_optionIndex = index;
}


void Dialog::draw()
{
    const Entry &entry = m_entries[m_current];

    m_fontTop.draw(m_strings[entry.text], QSize(width(), 36), false, true);

    int y = 57;
    if (entry.options.isEmpty()) {
        y += drawOption(y, NULL);
    }

    foreach (const Option &option, entry.options) {
        y += drawOption(y, &option);
    }
}


int Dialog::drawOption(int y, const Option* option)
{
    QString text;
    if (option && option->text >= 0)
        text = m_strings[option->text];
    else
        text = txt::StringTable::get(txt::DialogEnd);

    QRect rect = m_fontBottom.draw(text, QPoint(0, y), QSize(width(), -1), false, false);
    if (mapToGlobal(rect).contains(m_mousePos)) {
        m_fontHighlight.draw(text, QPoint(0, y), QSize(width(), -1), false, false);
        m_rect = rect;
        m_option = option;
    }

    return rect.height() + 4;
}


void Dialog::mouseMoveEvent(const QPoint &pos)
{
    m_mousePos = pos;
}


bool Dialog::mousePressEvent(const QPoint &pos, Qt::MouseButton button)
{
    if (button == Qt::LeftButton && mapToGlobal(m_rect).contains(pos)) {
        if (!m_option)
            emit close();
        else
            select(m_option->next);
        return true;
    }

    return false;
}


void Dialog::loadTree(const QString &filename)
{
    QFile file(filename);
    file.open(QFile::ReadOnly);

    QDataStream stream(&file);
    stream.setByteOrder(QDataStream::LittleEndian);

    quint32 numEntries;
    stream >> numEntries;
    for (unsigned int i = 0; i < numEntries; i++) {
        quint32 number;
        qint32 text;
        quint8 x, y;
        stream >> number >> x >> y >> text;

        Entry entry;
        entry.text = text;

        for (int x = 0; x < 4; x++) {
            qint32 text;
            qint32 next;

            stream >> text;
            stream.skipRawData(4);
            stream >> next;
            stream.skipRawData(4 * 4);

            if (text != -1) {
                Option option;
                option.text = text;
                option.next = next;
                entry.options << option;
            }
        }

        m_entries.insert(number, entry);
    }
}


void Dialog::loadStrings(const QString &filename)
{
    QFile file(filename);
    file.open(QFile::ReadOnly);

    QRegExp regexp("#(\\d*).*\\{(.*)\\}");
    while (!file.atEnd())  {
        QString line = file.readLine();
        if (file.error() != QFile::NoError)
            break;

        if (regexp.indexIn(line) != -1) {
            const int index = regexp.cap(1).toInt();
            const QString text = regexp.cap(2);
            m_strings.insert(index, text);
        }
    }
}


} // namespace game
