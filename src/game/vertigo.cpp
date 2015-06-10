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
#include "vertigo.h"

#include "fight/scenario.h"

#include <QApplication>

namespace game {

Vertigo::Vertigo() :
    m_mainMenu(
       [this](QString name) { startGame(name); },
       [this](QString name) { loadGame(name); },
       [this]() { m_window.close(); }
    ),
    m_intro(NULL),
    m_chapter(NULL)
{
}

Vertigo::~Vertigo()
{
    delete m_chapter;
    delete m_intro;
}

bool Vertigo::start(const QString &scenario)
{
    if (scenario.isEmpty()) {
        m_window.setRenderer(&m_mainMenu);
    } else {
        fight::Scenario *s = new fight::Scenario(scenario, []() { QApplication::quit(); });
        m_window.setRenderer(s);
    }

    m_window.show();
    return true;
}

void Vertigo::createChapter()
{
    Q_ASSERT(m_chapter == NULL);
    m_chapter = new Chapter(m_name,
        [this](Renderer *renderer) { m_window.setRenderer(renderer); },
        [this]() { endGame(); }
    );
}

void Vertigo::startGame(const QString &name)
{
    if (name.isEmpty())
        return;

    foreach (const Chapter::SavedGame &game, Chapter::savedGames()) {
        if (game.name.toLower() == name)
            return;
    }

    m_name = name;

    Q_ASSERT(m_intro == NULL);
    m_intro = new Movie([this]() { introFinished(); });

    m_window.setRenderer(m_intro);
    m_intro->play("gfx:mvi/film/d02.mvi");
}

void Vertigo::loadGame(const QString &name)
{
    createChapter();
    m_chapter->load(name);
}

void Vertigo::endGame()
{
    m_chapter->deleteLater();
    m_chapter = NULL;
    m_window.setRenderer(&m_mainMenu);
}

void Vertigo::introFinished()
{
    m_window.setRenderer(NULL);
    delete m_intro;
    m_intro = NULL;

    createChapter();
    m_chapter->loadChapter(1);
}

} // namespace game
