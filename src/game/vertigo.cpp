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
#include <QDir>
#include <QFileDialog>
#include <QSettings>
#include <QTimer>


namespace game {


Vertigo::Vertigo() :
    m_window(NULL),
    m_mainMenu(NULL),
    m_intro(NULL),
    m_chapter(NULL)
{

}


Vertigo::~Vertigo()
{
    delete m_chapter;
    delete m_intro;
    delete m_mainMenu;
    delete m_window;
}


bool Vertigo::start()
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

    m_window = new Window;

    m_mainMenu = new MainMenu(false);
    connect(m_mainMenu, SIGNAL(startGame()), SLOT(startGame()));

    m_window->setRenderer(m_mainMenu);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), SLOT(update()));
    timer->start(20);

    m_window->show();
    return true;
}


void Vertigo::update()
{
    m_window->update();
}


void Vertigo::startGame()
{
    m_mainMenu->deleteLater();
    m_mainMenu = NULL;

    Q_ASSERT(m_intro == NULL);
    m_intro = new Movie;
    connect(m_intro, SIGNAL(finished()), SLOT(introFinished()));

    m_window->setRenderer(m_intro);
    m_intro->play("gfx:mvi/film/d02.mvi");
}


void Vertigo::endGame()
{
    m_chapter->deleteLater();
    m_chapter = NULL;
    Q_ASSERT(m_mainMenu == NULL);
    m_mainMenu = new MainMenu(true);
    connect(m_mainMenu, SIGNAL(startGame()), SLOT(startGame()));
    m_window->setRenderer(m_mainMenu);
}


void Vertigo::introFinished()
{
    m_window->setRenderer(NULL);
    delete m_intro;
    m_intro = NULL;

    Q_ASSERT(m_chapter == NULL);
    m_chapter = new Chapter;
    connect(m_chapter, SIGNAL(setRenderer(Renderer*)), m_window, SLOT(setRenderer(Renderer*)));
    connect(m_chapter, SIGNAL(endGame()), SLOT(endGame()));
    m_chapter->loadChapter(1);
}


} // namespace game
