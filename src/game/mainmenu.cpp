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
#include "gfx/colortable.h"
#include "gfx/image.h"
#include "txt/stringtable.h"
#include "ui/button.h"
#include <QGLContext>
#include <QKeyEvent>

namespace game {

MainMenu::MainMenu(std::function<void(QString)> funcStartGame, std::function<void(QString)> funcLoadGame, std::function<void()> funcQuit) :
    m_state(Invalid),
    m_lblVersion(&m_title),
    m_lblBar1(&m_title),
    m_lblBar2(&m_title),
    m_lblMain(&m_title),
    m_lblMainTitle(&m_lblMain),
    m_btnMainNew([this]() { showNew(); }, &m_lblMain),
    m_btnMainLoad([this]() { showLoad(); }, &m_lblMain),
    m_btnMainQuit(funcQuit, &m_lblMain),
    m_lblNew(&m_title, false),
    m_lblNewTitle(&m_lblNew),
    m_btnNewBack([this]() { showMain(); }, &m_lblNew),
    m_lblLoad(&m_title, false),
    m_lblLoadTitle(&m_lblLoad),
    m_btnLoadBack([this]() { showMain(); }, &m_lblLoad),
    m_lblLoading(&m_title, false),
    m_lblLoadingTitle(&m_lblLoading),
    m_lblLoadingWait(&m_lblLoading),
    m_cursor(false),
    m_funcStartGame(std::move(funcStartGame)),
    m_funcLoadGame(std::move(funcLoadGame))
{
    const gfx::ColorTable colorTable("gfx:pal/gui/border.pal");

    gfx::Texture texBar(gfx::Image::load("gfx:img/title/horline.img", colorTable));
    gfx::Font fontSmall("gfx:fnt/dpsmall.fnt", colorTable);
    gfx::Font fontMedium("gfx:fnt/dpmedium.fnt", colorTable);
    gfx::Font fontLarge("gfx:fnt/dplarge.fnt", colorTable);

    m_presents.setTexture(gfx::Image::loadPCX("gfx:pic/title/present.pcx"));
    m_title.setTexture(gfx::Image::loadPCX("gfx:pic/title/title.pcx"));

    m_backgroundSound.load("sfx:snd/bground/b02.pcl", "sfx:snd/bground/b02.pcr");

    m_lblVersion.setFont(fontSmall);
    m_lblVersion.setPosition(2, 2);
    m_lblVersion.setText("Vertigo 0.1");
    m_lblBar1.setTexture(texBar);
    m_lblBar1.setPosition(140, 256);
    m_lblBar2.setTexture(texBar);
    m_lblBar2.setPosition(140, 448);

    m_lblMainTitle.setFont(fontMedium);
    m_lblMainTitle.setPosition(145, 256 - fontMedium.height() - 2);
    m_lblMainTitle.setText(txt::StringTable::get(txt::MainMenu));

    m_btnMainNew.setFont(fontLarge);
    m_btnMainNew.setPosition(0, 288);
    m_btnMainNew.setWidth(640);
    m_btnMainNew.setAlignment(ui::Label::AlignHCenter);
    m_btnMainNew.setText(txt::StringTable::get(txt::MainMenu_NewGame));

    m_btnMainLoad.setFont(fontLarge);
    m_btnMainLoad.setPosition(0, 308);
    m_btnMainLoad.setWidth(640);
    m_btnMainLoad.setAlignment(ui::Label::AlignHCenter);
    m_btnMainLoad.setText(txt::StringTable::get(txt::MainMenu_Load));

    m_btnMainQuit.setFont(fontLarge);
    m_btnMainQuit.setPosition(0, 408);
    m_btnMainQuit.setWidth(640);
    m_btnMainQuit.setAlignment(ui::Label::AlignHCenter);
    m_btnMainQuit.setText(txt::StringTable::get(txt::MainMenu_QuitGame));

    m_lblNewTitle.setFont(gfx::Font::Medium);
    m_lblNewTitle.setPosition(145, 256 - gfx::Font(gfx::Font::Medium).height() - 2);
    m_lblNewTitle.setText(txt::StringTable::get(txt::MainMenu_NewGame));

    m_btnNewBack.setPosition(0, 428);
    m_btnNewBack.setWidth(640);
    m_btnNewBack.setAlignment(ui::Button::AlignHCenter);
    m_btnNewBack.setFont(gfx::Font::Large);
    m_btnNewBack.setText(txt::StringTable::get(txt::MainMenu));

    m_lblLoadTitle.setFont(gfx::Font::Medium);
    m_lblLoadTitle.setPosition(145, 256 - gfx::Font(gfx::Font::Medium).height() - 2);
    m_lblLoadTitle.setText(txt::StringTable::get(txt::MainMenu_Load));

    m_btnLoadBack.setPosition(0, 428);
    m_btnLoadBack.setWidth(640);
    m_btnLoadBack.setAlignment(ui::Button::AlignHCenter);
    m_btnLoadBack.setFont(gfx::Font::Large);
    m_btnLoadBack.setText(txt::StringTable::get(txt::MainMenu));

    m_lblLoadingTitle.setFont(gfx::Font::Large);
    m_lblLoadingTitle.setPosition(0, 316);
    m_lblLoadingTitle.setWidth(640);
    m_lblLoadingTitle.setAlignment(ui::Label::AlignHCenter);
    m_lblLoadingTitle.setText(txt::StringTable::get(txt::MainMenu_Loading));

    m_lblLoadingWait.setFont(gfx::Font::Large);
    m_lblLoadingWait.setPosition(0, 336);
    m_lblLoadingWait.setWidth(640);
    m_lblLoadingWait.setAlignment(ui::Label::AlignHCenter);
    m_lblLoadingWait.setText(txt::StringTable::get(txt::MainMenu_PleaseWait));

    changeState(Presents);
}

void MainMenu::draw()
{
    Menu::draw();

    if (m_lblNew.isVisible()) {
        if (m_time.elapsed() > 300) {
            m_time.start();
            m_cursor = !m_cursor;
        }

        Rect rect = gfx::Font(gfx::Font::Large).draw(m_name, Rect(0, 328, 640, -1), true, false);
        if (m_cursor)
            gfx::Font(gfx::Font::Large).draw("_", rect.pos() + Point(rect.width, 2));
    }
    if (m_state == Loading)
    {
        QGLWidget *win = window();
        if (win->doubleBuffer())
            win->swapBuffers();
        m_funcLoadGame(m_loadingGame);
        changeState(Title);
        if (win->doubleBuffer())
            win->swapBuffers();
    }
}

void MainMenu::activate()
{
    m_backgroundSound.playLoop();
}

void MainMenu::deactivate()
{
    m_backgroundSound.stop();
}

void MainMenu::changeState(State state)
{
    switch (state) {
    case Presents:
        setRootWidget(&m_presents);
        m_state = state;
        break;

    case Title:
        setRootWidget(&m_title);
        showMain();
        m_state = state;
        break;

    case Loading:
        setRootWidget(&m_title);
        showLoading();
        m_state = state;
        break;

    default:
        qFatal("Cannot change state!");
    }
}

void MainMenu::mousePressEvent(QMouseEvent *event)
{
    Menu::mousePressEvent(event);

    switch (m_state) {
    case Presents:
        changeState(Title);
        break;

    default:
        ; // nothing to do
    }

    if (event->button() == Qt::RightButton) {
        showMain();
    }
}

void MainMenu::keyPressEvent(QKeyEvent *event)
{
    Menu::keyPressEvent(event);

    if (m_lblNew.isVisible()) {
        if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {
            m_funcStartGame(m_name.trimmed());
            showMain();
        }

        if (event->key() == Qt::Key_Backspace)
            m_name.chop(1);

        QString text = event->text().toLower().toLatin1();
        text.remove(QRegExp("[^a-z\\-0-9 ]*"));
        if (!text.isEmpty())
            m_name += text;

        if (m_name.length() > 20)
            m_name.chop(m_name.length() - 20);
    }
}

void MainMenu::showMain()
{
    m_lblMain.show();
    m_lblNew.hide();
    m_lblLoad.hide();
    m_lblLoading.hide();
}

void MainMenu::showNew()
{
    m_lblMain.hide();
    m_lblNew.show();
    m_name = "dead-eye";
}

void MainMenu::showLoad()
{
    m_lblMain.hide();
    m_lblLoad.show();

    int y = 282;

    m_lblLoadSave.clear();
    m_btnLoadSave.clear();

    QList<Chapter::SavedGame> games = Chapter::savedGames();
    qSort(games);
    foreach (const Chapter::SavedGame &game, games) {
        QString name = game.name;

        m_btnLoadSave.emplace_back([this, name]() { changeState(Loading); m_loadingGame = name; }, &m_lblLoad);
        ui::Button &button = m_btnLoadSave.back();
        button.setPosition(0, y);
        button.setWidth(640);
        button.setAlignment(ui::Button::AlignHCenter);
        button.setFont(gfx::Font::Large);
        button.setText(QString("%1: %2").arg(game.name, game.station));

        y += 13;

        m_lblLoadSave.emplace_back(&m_lblLoad);
        ui::Label &label = m_lblLoadSave.back();
        label.setPosition(0, y);
        label.setWidth(640);
        label.setAlignment(ui::Button::AlignHCenter);
        label.setFont(gfx::Font::Small);
        label.setText(game.time.toString(Qt::DefaultLocaleShortDate));

        y += 20;
    }
}


void MainMenu::showLoading()
{
    m_lblLoad.hide();
    m_lblLoading.show();
}


} // namespace game
