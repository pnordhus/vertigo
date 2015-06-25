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

#include "vertigo.h"

#include "hud/hud.h"
#include "game/boat.h"
#include "fight/scenario.h"
#include "txt/desfile.h"

#include <QCoreApplication>

namespace game {

Vertigo::Vertigo() :
    m_mainMenu(
       [this](QString name) { startGame(name); },
       [this](QString name) { loadGame(name); },
       [this]() { m_window.close(); }
    )
{
}

void Vertigo::start(const QString &scenarioName)
{
    std::unique_ptr<hud::HUD> hud;
    std::unique_ptr<game::Boat> boat;
    std::unique_ptr<fight::Scenario> scenario;

    if (scenarioName.isEmpty()) {
        m_window.setRenderer(&m_mainMenu);
    } else {
        hud.reset(new hud::HUD());
        hud->eventSuccess() += [this]() { m_window.close(); };
        boat.reset(new game::Boat(txt::DesFile("dat:story/ch1.des")));
        hud->load(boat.get());
        scenario.reset(new fight::Scenario(scenarioName));
        hud->start(scenario.get());
        m_window.setRenderer(hud.get());
    }

    m_window.show();
    while (m_window.isVisible()) {
        qApp->processEvents();
        util::DeferredDeletable::clear();
    }
}

void Vertigo::createChapter()
{
    m_chapter.reset(new Chapter(m_name,
        [this](Renderer *renderer) { m_window.setRenderer(renderer); },
        [this]() { endGame(); }
    ));
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
    m_intro.reset(new Movie([this]() { introFinished(); }));
    m_window.setRenderer(m_intro.get());
    m_intro->play("gfx:mvi/film/d02.mvi");
}

void Vertigo::loadGame(const QString &name)
{
    createChapter();
    m_chapter->load(name);
}

void Vertigo::endGame()
{
    m_chapter.release()->deleteLater();
    m_window.setRenderer(&m_mainMenu);
}

void Vertigo::introFinished()
{
    m_window.setRenderer(NULL);
    m_intro.release()->deleteLater();

    createChapter();
    m_chapter->loadChapter(1);
}

} // namespace game
