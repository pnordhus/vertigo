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
#include "sfx/soundsystem.h"
#include "txt/desfile.h"
#include "txt/stringtable.h"
#include <QFile>


namespace game {


Dialog::Dialog(int chapter, int id, ui::Widget *parent,
    std::function<void(int)> funcRemove,
    std::function<void(int)> funcAddMessage,
    std::function<void(int)> funcAddTask,
    std::function<void(int)> funcRemoveTask,
    std::function<void(int)> funcChangeChapter,
    std::function<void(int)> funcAddDialog,
    std::function<void(int)> funcRemoveDialog,
    std::function<void(int)> funcAddCredit,
    std::function<void(int)> funcEnableStation,
    std::function<void(int)> funcDisableStation,
    std::function<void(QString, int)> funcAddMission,
    std::function<void(int, QString)> funcReplaceApproachMovie,
    std::function<void()> funcGameOver
) :
    ui::Widget(parent),
    m_id(id),
    m_option(NULL),
    m_finished(false),
    m_remove(false),
    m_changeChapter(-1),
    m_gameOver(false),
    m_funcRemove(std::move(funcRemove)),
    m_funcAddMessage(std::move(funcAddMessage)),
    m_funcAddTask(std::move(funcAddTask)),
    m_funcRemoveTask(std::move(funcRemoveTask)),
    m_funcChangeChapter(std::move(funcChangeChapter)),
    m_funcAddDialog(std::move(funcAddDialog)),
    m_funcRemoveDialog(std::move(funcRemoveDialog)),
    m_funcAddCredit(std::move(funcAddCredit)),
    m_funcEnableStation(std::move(funcEnableStation)),
    m_funcDisableStation(std::move(funcDisableStation)),
    m_funcAddMission(std::move(funcAddMission)),
    m_funcReplaceApproachMovie(std::move(funcReplaceApproachMovie)),
    m_funcGameOver(std::move(funcGameOver))
{
    const QString baseName = QString("txt:dia/%1/%2").arg(chapter, 3, 10, QChar('0')).arg(id, 6, 10, QChar('0'));

    txt::DesFile file(baseName + ".des");

    file.setSection("Person");

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

    foreach (const QString &section, file.sections().filter(QRegExp("^message"))) {
        file.setSection(section);
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
        } else if (type == "subperson") {
            message.type = Message::RemoveDialog;
            message.value = file.value("Name").toInt();
        } else if (type == "credit") {
            message.type = Message::AddCredit;
            message.value = file.value("amount").toInt();
        } else if (type == "enablestation") {
            message.type = Message::EnableStation;
            message.value = file.value("station").toInt();
        } else if (type == "disablestation") {
            message.type = Message::DisableStation;
            message.value = file.value("station").toInt();
        } else if (type == "mission2station") {
            message.type = Message::AddMissionStation;
            message.value = file.value("station").toInt();
            message.name = file.value("mission").toString();
        } else if (type == "mission2area") {
            message.type = Message::AddMissionArea;
            message.name = file.value("mission").toString();
        } else if (type == "replaceapproachmovie") {
            message.type = Message::ReplaceApproachMovie;
            message.value = file.value("Station").toInt();
            message.name = file.value("Name").toString();
        } else if (type == "boat") {
            message.type = Message::Boat;
            message.value = file.value("Boat").toInt();
        } else if (type == "endgame") {
            message.type = Message::GameOver;
        }

        m_messages.insertMulti(id, message);
    }

    foreach (const QString &section, file.sections().filter(QRegExp("^precondition"))) {
        file.setSection(section);

        const QString type = file.value("Type").toString().toLower();

        Precondition precondition;
        if (type == "numofsmalltalk") {
            precondition.type = Precondition::NumSmallTalks;
            precondition.amount = file.value("Amount").toInt();
        } else if (type == "mission") {
            precondition.type = Precondition::Mission;
            precondition.name = file.value("Name").toString();
        }

        m_preconditions.append(precondition);
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
            if (m_option->message < 9999000) {
                m_funcAddMessage(m_option->message);
            }

            foreach (const Message &message, m_messages.values(m_option->message)) {
                switch (message.type) {
                case Message::None:
                    break;

                case Message::AddTask:
                    m_funcAddTask(message.value);
                    break;

                case Message::RemoveTask:
                    m_funcRemoveTask(message.value);
                    break;

                case Message::ChangeChapter:
                    m_changeChapter = message.value;
                    break;

                case Message::AddDialog:
                    m_funcAddDialog(message.value);
                    break;

                case Message::RemoveDialog:
                    m_funcRemoveDialog(message.value);
                    break;

                case Message::AddCredit:
                    m_funcAddCredit(message.value);
                    break;

                case Message::EnableStation:
                    m_funcEnableStation(message.value);
                    break;

                case Message::DisableStation:
                    m_funcDisableStation(message.value);
                    break;

                case Message::AddMissionStation:
                    m_funcAddMission(message.name, message.value);
                    break;

                case Message::AddMissionArea:
                    m_funcAddMission(message.name, -1);
                    break;

                case Message::ReplaceApproachMovie:
                    m_funcReplaceApproachMovie(message.value, message.name + ".mvi");
                    break;

                case Message::Boat:
                    // 2048 + message.value
                    break;

                case Message::GameOver:
                    m_gameOver = true;
                    break;
                }
            }
        }

        if (!m_gameOver) {
            switch (m_option->next) {
            case RemoveDialog:
                m_remove = true;
                m_funcRemove(m_id);
                break;
            }
        }

        if (m_option->next > 0) {
            m_current = m_option->next;
            m_optionIndex = m_current;
        }
    }

    if (m_finished) {
        if (m_changeChapter != -1)
            m_funcChangeChapter(m_changeChapter);
        if (m_gameOver) {
            m_funcGameOver();
        } else {
            funcClose();
        }
    }

    if (!m_option || m_finished) {
        m_current = 1;
        m_optionIndex = 1;
        m_finished = false;
    }

    m_option = NULL;
    m_rect = QRect();
}


void Dialog::draw()
{
    const Entry &entry = m_entries[m_current];

    m_rect = QRect();
    const Option *lastOption = m_option;
    m_option = NULL;

    gfx::Font(gfx::Font::DialogTop).draw(m_strings[entry.text], util::Rect(0, 0, width(), 36), false, true);

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
    if (m_option != NULL && m_option != lastOption)
        sfx::SoundSystem::get()->sound(sfx::Woop)->play();
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

    util::Rect rectTmp = gfx::Font(gfx::Font::DialogBottom).draw(text, util::Rect(0, y, width(), -1), false, false);
    QRect rect(rectTmp.x, rectTmp.y, rectTmp.width, rectTmp.height);
    rect.setWidth(width());
    if (mapToGlobal(rect).contains(m_mousePos)) {
        gfx::Font(gfx::Font::DialogHighlight).draw(text, util::Rect(0, y, width(), -1), false, false);
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
    if (m_area != area || m_station != station || m_room != room || m_person >= 20)
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


bool Dialog::matchesDirect(int area, int station) const
{
    if (m_area != 0 && m_area != area)
        return false;
    if (m_station != 0 && m_station != station)
        return false;
    if (m_person != 20)
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

        case Precondition::Mission:
            if (!Chapter::get()->successfulMissions().contains(precondition.name))
                return false;
            break;
        }
    }

    return true;
}


} // namespace game
