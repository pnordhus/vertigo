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
    m_id(id),
    m_option(NULL),
    m_finished(false)
{
    m_fontTop.load("gfx:fnt/dpsmamon.fnt", 0xffb89c00, 0xffc0c400, true);
    m_fontBottom.load("gfx:fnt/dpsmamon.fnt", 0xff00a8d0, 0xff00a8d0, true);
    m_fontHighlight.load("gfx:fnt/dpsmamon.fnt", 0xff00e4f8, 0xff00e4f8, true);

    const QString baseName = QString("txt:dia/%1/%2").arg(id / 1000, 3, 10, QChar('0')).arg(id, 6, 10, QChar('0'));

    txt::DesFile file(baseName + ".des");

    file.beginGroup("Person");

    m_name = file.value("Name").toString();

    QRegExp reg("(\\d*)\\/(\\d*)\\/(\\d*)\\/(\\d*)");
    if (reg.indexIn(file.value("Position").toString()) >= 0) {
        m_area = reg.cap(1).toInt();
        m_station = reg.cap(2).toInt();
        m_room = reg.cap(3).toInt();
        m_person = reg.cap(4).toInt();
    }

    m_female = file.value("Sex").toString() == "f";

    file.endGroup();

    loadTree(baseName + ".dia");
    loadStrings(baseName + ".sen");

    select();
}


void Dialog::select()
{
    m_optionIndex = 0;
    if (m_option) {
        switch (m_option->next) {
        case RemoveDialog:
            emit remove(m_id);
            break;
        }

        if (m_option->next > 0) {
            m_current = m_option->next;
            m_optionIndex = m_current;
        }
    }

    if (m_finished)
        emit close();

    if (!m_option || m_finished) {
        m_current = 1;
        m_optionIndex = 1;
        m_finished = false;
    }

    m_option = NULL;
}


void Dialog::draw()
{
    const Entry &entry = m_entries[m_current];

    m_rect = QRect();
    m_option = NULL;

    m_fontTop.draw(m_strings[entry.text], QSize(width(), 36), false, true);

    int y = 57;
    if (m_optionIndex == 0) {
        y += drawOption(y, NULL);
    } else {
        foreach (const Option &option, entry.options) {
            y += drawOption(y, &option);
        }
    }
}


int Dialog::drawOption(int y, const Option* option)
{
    QString text;
    if (option && option->text >= 0)
        text = m_strings[option->text];
    else {
        text = txt::StringTable::get(txt::DialogEnd);
        m_finished = true;
    }

    QRect rect = m_fontBottom.draw(text, QPoint(0, y), QSize(width(), -1), false, false);
    rect.setWidth(width());
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
        select();
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

            if (entry.options.isEmpty() || text != -1) {
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


bool Dialog::matches(int area, int station, int room) const
{
    return m_area == area && m_station == station && m_room == room;
}


} // namespace game
