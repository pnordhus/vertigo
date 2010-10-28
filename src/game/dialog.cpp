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

#include "chapter.h"
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
    m_finished(false),
    m_changeChapter(-1)
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
    m_isSmallTalk = file.value("Type").toString() == "smalltalk";

    file.endGroup();

    foreach (const QString &section, file.childGroups().filter(QRegExp("^Message", Qt::CaseInsensitive))) {
        file.beginGroup(section);
        const int id = file.value("Id").toInt();
        const QString type = file.value("Type").toString();

        Message message;
        if (type == "addtask") {
            message.type = Message::AddTask;
            message.value = file.value("TaskCode").toInt();
        } else if (type == "subtask") {
            message.type = Message::RemoveTask;
            message.value = file.value("TaskCode").toInt();
        } else if (type == "chapter") {
            message.type = Message::ChangeChapter;
            message.value = file.value("ChapterCode").toInt();
        } else if (type == "addperson") {
            message.type = Message::AddDialog;
            message.value = file.value("Name").toInt();
        } else if (type == "replaceapproachmovie") {
            message.type = Message::ReplaceApproachMovie;
            message.value = file.value("Station").toInt();
            message.name = file.value("Name").toString();
        }

        m_messages.insertMulti(id, message);
        file.endGroup();
    }

    foreach (const QString &section, file.childGroups().filter(QRegExp("^Precondition", Qt::CaseInsensitive))) {
        file.beginGroup(section);

        const QString type = file.value("Type").toString().toLower();

        Precondition precondition;
        if (type == "numofsmalltalk") {
            precondition.type = Precondition::NumSmallTalks;
            precondition.amount = file.value("Amount").toInt();
        }

        m_preconditions.append(precondition);

        file.endGroup();
    }

    loadTree(baseName + ".dia");
    loadStrings(baseName + ".sen");

    select();
}


void Dialog::select()
{
    m_optionIndex = 0;
    if (m_option) {
        if (m_option->message > 0) {
            if (m_option->message < 9999000)
                emit addMessage(m_option->message);

            foreach (const Message &message, m_messages.values(m_option->message)) {
                switch (message.type) {
                case Message::None:
                    break;

                case Message::AddTask:
                    emit addTask(message.value);
                    break;

                case Message::RemoveTask:
                    emit removeTask(message.value);
                    break;

                case Message::ChangeChapter:
                    m_changeChapter = message.value;
                    break;

                case Message::AddDialog:
                    emit addDialog(message.value);
                    break;

                case Message::ReplaceApproachMovie:
                    emit replaceApproachMovie(message.value, message.name + ".mvi");
                    break;
                }
            }
        }
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

    if (m_finished) {
        if (m_changeChapter != -1)
            emit changeChapter(m_changeChapter);
        emit close();
    }

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
        const QSet<int>& messages = Chapter::get()->messages();
        foreach (const Option &option, entry.options) {
            if (messages.contains(option.onlyIf) && (messages & option.notIf).isEmpty())
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
            qint32 message;
            qint32 next;
            qint32 onlyIf1;
            qint32 onlyIf2;
            qint32 notIf1;
            qint32 notIf2;

            stream >> text >> message >> next;
            stream >> onlyIf1 >> onlyIf2 >> notIf1 >> notIf2;

            if (entry.options.isEmpty() || text != -1) {
                Option option;
                option.text = text;
                option.next = next;
                option.message = message;
                if (onlyIf1 != -1)
                    option.onlyIf << onlyIf1;
                if (onlyIf2 != -1)
                    option.onlyIf << onlyIf2;
                if (notIf1 != -1)
                    option.notIf << notIf1;
                if (notIf2 != -1)
                    option.notIf << notIf2;
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
    if (m_area != area || m_station != station || m_room != room)
        return false;

    return testPreconditions();
}


bool Dialog::matchesEnCom(int area, int station, bool room) const
{
    if (m_area != 0 && m_area != area)
        return false;
    if (m_station != 0 && m_station != station)
        return false;
    if (m_room != 1 && room)
        return false;
    if (m_person != 99)
        return false;

    return testPreconditions();
}


bool Dialog::testPreconditions() const
{
    foreach (const Precondition &precondition, m_preconditions) {
        switch (precondition.type) {
        case Precondition::None:
            break;

        case Precondition::NumSmallTalks:
            if (Chapter::get()->numSmallTalks() < precondition.amount)
                return false;
            break;
        }
    }

    return true;
}


} // namespace game
