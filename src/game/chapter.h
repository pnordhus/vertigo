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

#ifndef GAME_CHAPTER_H
#define GAME_CHAPTER_H


#include "area.h"
#include "briefing.h"
#include "desktop.h"
#include "dialog.h"
#include "mission.h"
#include "movie.h"
#include "task.h"
#include "boat.h"


namespace game {


class Chapter : public QObject
{
    Q_OBJECT

public:
    Chapter();
    ~Chapter();

signals:
    void setRenderer(Renderer*);
    void endGame();

public slots:
    void quit();
    void toggleMovieAutopilot();
    void toggleMovieApproach();
    void toggleMovieHarbour();

public:
    void save(int slot, const QString &name) const;
    void load(int slot);
    void loadChapter(int chapter);
    void setStation(int station);
    Area* area() const { return m_area; }
    const QMap<int, Station>& stations() const { return m_stations; }
    int currentStation() const { return m_currentStation; }
    Dialog* dialog(int dialogId) const;
    QList<Dialog*> dialogs(int room);
    QList<Dialog*> dialogsEnCom(bool room);
    QList<Dialog*> dialogsDirect();
    const QSet<int>& messages() const { return m_messages; }
    int numSmallTalks() const { return m_numSmallTalks; }
    bool movieAutopilot() const { return m_movieAutopilot; }
    bool movieApproach() const { return m_movieApproach; }
    bool movieHarbour() const { return m_movieHarbour; }
    const QSet<int>& playedMovies() const { return m_playedMovies; }
    void playMovie(int movie);
    const QList<Mission*>& missions() const { return m_missions; }
    const QStringList& successfulMissions() const { return m_successfulMissions; }
    void startMission(const QString &name);
    const Mission* mission() const { return m_mission; }
    QList<Task> tasks();
    Boat* boat() const { return m_boat; }

private:
    void load(const QString &filename);
    void playApproach(bool autopilot);
    void playMovies();
    void startMission();

private slots:
    void movieFinished();
    void finishDialog(int dialogId);
    void addMessage(int message);
    void addTask(int task);
    void removeTask(int task);
    void changeChapter(int chapter);
    void addDialog(int dialogId);
    void removeDialog(int dialogId);
    void addCredit(int credit);
    void enableStation(int station);
    void disableStation(int station);
    void addMission(const QString &mission, int station);
    void replaceApproachMovie(int station, const QString &movie);
    void finishMission();

public:
    static Chapter* get() { Q_ASSERT(m_singleton); return m_singleton; }
    static QMap<int, QString> savedGames();

private:
    int m_code;
    Area *m_area;
    Desktop *m_desktop;
    Briefing *m_briefing;
    Movie *m_movie;
    QStringList m_movies;
    QMap<int, QString> m_approachMovieReplacement;
    QMap<int, Station> m_stations;
    int m_currentStation;
    QString m_currentStationName;
    QSet<int> m_playedMovies;
    QMap<int, Dialog*> m_pendingDialogues;
    static Chapter *m_singleton;
    QSet<int> m_messages;
    QList<int> m_runningTasks;
    QList<Mission*> m_missions;
    QStringList m_successfulMissions;
    int m_credits;
    int m_numSmallTalks;
    bool m_movieAutopilot;
    bool m_movieApproach;
    bool m_movieHarbour;
    Mission *m_mission;
    txt::DesFile m_tasksFile;
    bool m_save;
    Boat *m_boat;
};


} // namespace game


#endif // GAME_CHAPTER_H
