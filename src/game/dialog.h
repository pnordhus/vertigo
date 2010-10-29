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

#ifndef GAME_DIALOG_H
#define GAME_DIALOG_H


#include "gfx/font.h"
#include "ui/widget.h"
#include "sfx/sound.h"
#include <QMap>
#include <QSet>


namespace game {


class Dialog : public ui::Widget
{
    Q_OBJECT

private:
    struct Option
    {
        int text;
        int next;
        int message;
        QSet<int> onlyIf;
        QSet<int> notIf;
    };

    struct Entry
    {
        int text;
        QList<Option> options;
    };

    struct Message
    {
        enum Type
        {
            None,
            AddTask,
            RemoveTask,
            ChangeChapter,
            AddDialog,
            AddCredit,
            EnableStation,
            DisableStation,
            AddMissionStation,
            AddMissionArea,
            ReplaceApproachMovie
        };

        Message() : type(None), value(0) {}

        Type type;
        int value;
        QString name;
    };

    struct Precondition
    {
        enum Type
        {
            None,
            NumSmallTalks,
        };

        Precondition() : type(None), amount(0) {}

        Type type;
        int amount;
    };

    enum Return
    {
        RemoveDialog = -100001,
    };

public:
    Dialog(int id, ui::Widget *parent = NULL);

signals:
    void close();
    void remove(int);
    void addMessage(int);
    void addTask(int);
    void removeTask(int);
    void changeChapter(int);
    void addDialog(int);
    void addCredit(int);
    void replaceApproachMovie(int, const QString&);

public:
    int id() const { return m_id; }
    const QString& name() const { return m_name; }
    bool isFemale() const { return m_female; }
    int person() const { return m_person; }
    bool matches(int area, int station, int room) const;
    bool matchesEnCom(int area, int station, bool room) const;
    bool isSmallTalk() const { return m_isSmallTalk; }

private:
    void draw();
    int drawOption(int y, const Option *option);
    void mouseMoveEvent(const QPoint &pos);
    bool mousePressEvent(const QPoint &pos, Qt::MouseButton button);
    void select();
    void loadTree(const QString &filename);
    void loadStrings(const QString &filename);
    bool testPreconditions() const;

private:
    gfx::Font m_fontTop;
    gfx::Font m_fontBottom;
    gfx::Font m_fontHighlight;
    sfx::Sound m_woopSound;

    int m_id;
    QMap<int, Entry> m_entries;
    QMap<int, QString> m_strings;
    int m_current;
    int m_optionIndex;
    QPoint m_mousePos;
    QRect m_rect;
    const Option *m_option;
    QString m_name;
    bool m_female;
    int m_area;
    int m_station;
    int m_room;
    int m_person;
    bool m_finished;
    QMap<int, Message> m_messages;
    int m_changeChapter;
    bool m_isSmallTalk;
    QList<Precondition> m_preconditions;
};


} // namespace game


#endif // GAME_DIALOG_H
