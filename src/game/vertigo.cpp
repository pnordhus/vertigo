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
#include "mainmenu.h"
#include "vertigo.h"
#include "window.h"
#include "txt/stringtable.h"
#include <QApplication>
#include <QDir>
#include <QFileDialog>
#include <QSettings>
#include <QTimer>
#include "fight/scenario.h"


namespace game {


Vertigo::Vertigo() :
    m_window(NULL),
    m_mainMenu(NULL),
    m_intro(NULL),
    m_chapter(NULL),
    m_soundSystem(NULL),
    m_fontManager(NULL)
{

}


Vertigo::~Vertigo()
{
    delete m_chapter;
    delete m_intro;
    delete m_mainMenu;
    delete m_window;
    delete m_soundSystem;
    delete m_fontManager;
}


bool Vertigo::start(const QString &scenario)
{
    QDir::addSearchPath("dat", "data:dat");
    QDir::addSearchPath("gfx", "data:gfx");
    QDir::addSearchPath("sfx", "data:sfx");
    QDir::addSearchPath("txt", "data:txt");
    QDir::addSearchPath("vfx", "data:vfx");

    QSettings s;
    QDir::setSearchPaths("data", QStringList() << s.value("datadir").toString());
    while (!txt::StringTable::load()) {
        const QString dir = QFileDialog::getExistingDirectory(NULL, "Select Schleichfahrt / Archimedean Dynasty directory", s.value("datadir").toString());
        if (dir.isNull())
            return false;
        s.setValue("datadir", dir);
        QDir::setSearchPaths("data", QStringList() << s.value("datadir").toString());
    }

    m_soundSystem = new sfx::SoundSystem;

    m_window = new Window;
    m_fontManager = new gfx::FontManager;

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), SLOT(update()));
    timer->start(20);

    if (scenario.isEmpty()) {
        createMainMenu(false);
    } else {
        fight::Scenario *s = new fight::Scenario(scenario, QApplication::quit);
        m_window->setRenderer(s);
    }

    m_window->show();
    return true;
}


void Vertigo::createMainMenu(bool skipToTitle)
{
    Q_ASSERT(!m_mainMenu);
    m_mainMenu = new MainMenu(skipToTitle);
    connect(m_mainMenu, SIGNAL(startGame(QString)), SLOT(startGame(QString)));
    connect(m_mainMenu, SIGNAL(loadGame(QString)), SLOT(loadGame(QString)));
    connect(m_mainMenu, SIGNAL(quit()), m_window, SLOT(close()));
    m_window->setRenderer(m_mainMenu);
}


void Vertigo::createChapter()
{
    Q_ASSERT(m_chapter == NULL);
    m_chapter = new Chapter(m_name);
    connect(m_chapter, SIGNAL(setRenderer(Renderer*)), m_window, SLOT(setRenderer(Renderer*)));
    connect(m_chapter, SIGNAL(endGame()), SLOT(endGame()));
}


void Vertigo::update()
{
    m_window->update();
}


void Vertigo::startGame(const QString &name)
{
    if (name.isEmpty())
        return;

    foreach (const Chapter::SavedGame &game, Chapter::savedGames()) {
        if (game.name.toLower() == name)
            return;
    }

    m_mainMenu->deleteLater();
    m_mainMenu = NULL;
    m_name = name;

    Q_ASSERT(m_intro == NULL);
    m_intro = new Movie;
    connect(m_intro, SIGNAL(finished()), SLOT(introFinished()));

    m_window->setRenderer(m_intro);
    m_intro->play("gfx:mvi/film/d02.mvi");
}


void Vertigo::loadGame(const QString &name)
{
    m_mainMenu->deleteLater();
    m_mainMenu = NULL;

    createChapter();
    m_chapter->load(name);
}


void Vertigo::endGame()
{
    m_chapter->deleteLater();
    m_chapter = NULL;
    createMainMenu(true);
}


void Vertigo::introFinished()
{
    m_window->setRenderer(NULL);
    delete m_intro;
    m_intro = NULL;

    createChapter();
    m_chapter->loadChapter(1);
}


} // namespace game
