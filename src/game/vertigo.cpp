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

#include "mainmenu.h"
#include "movie.h"
#include "vertigo.h"
#include "window.h"
#include "txt/stringtable.h"
#include <QDir>
#include <QSettings>
#include <QTimer>


namespace game {


Vertigo::Vertigo()
{
    QSettings s;

    QDir::addSearchPath("data", s.value("datadir").toString());
    QDir::addSearchPath("gfx", "data:gfx");
    QDir::addSearchPath("sfx", "data:sfx");
    QDir::addSearchPath("txt", "data:txt");
    QDir::addSearchPath("vfx", "data:vfx");

    txt::StringTable::load();

    m_window = new Window;

    m_mainMenu = new MainMenu;
    connect(m_mainMenu, SIGNAL(startGame()), SLOT(startGame()));

    m_movie = new Movie;
    connect(m_movie, SIGNAL(finished()), SLOT(movieFinished()));

    m_window->setRenderer(m_mainMenu);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), SLOT(update()));
    timer->start(20);
}


Vertigo::~Vertigo()
{
    delete m_movie;
    delete m_mainMenu;
    delete m_window;
}


void Vertigo::start()
{
    m_window->show();
}


void Vertigo::update()
{
    m_window->update();
}


void Vertigo::startGame()
{
    m_window->setRenderer(m_movie);
    m_movie->play("gfx:mvi/film/d02.mvi");
}


void Vertigo::movieFinished()
{
    m_window->setRenderer(m_mainMenu);
}


} // namespace game
