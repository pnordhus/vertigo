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


MainMenu::MainMenu(bool skipToTitle, std::function<void(QString)> funcStartGame, std::function<void(QString)> funcLoadGame, std::function<void()> funcQuit) :
    m_state(Invalid),
    m_lblNew(NULL),
    m_lblLoad(NULL),
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

    ui::Label *label;

    label = new ui::Label(&m_title);
    label->setFont(fontSmall);
    label->setPosition(2, 2);
    label->setText("Vertigo 0.1");

    label = new ui::Label(&m_title);
    label->setTexture(texBar);
    label->setPosition(140, 256);

    label = new ui::Label(&m_title);
    label->setTexture(texBar);
    label->setPosition(140, 448);

    {
        m_lblMain = new ui::Label(&m_title);

        label = new ui::Label(m_lblMain);
        label->setFont(fontMedium);
        label->setPosition(145, 256 - fontMedium.height() - 2);
        label->setText(txt::StringTable::get(txt::MainMenu));

        ui::Button *button;
        button = new ui::Button([this]() { showNew(); }, m_lblMain);
        button->setFont(fontLarge);
        button->setPosition(0, 288);
        button->setWidth(640);
        button->setAlignment(ui::Label::AlignHCenter);
        button->setText(txt::StringTable::get(txt::MainMenu_NewGame));

        button = new ui::Button([this]() { showLoad(); }, m_lblMain);
        button->setFont(fontLarge);
        button->setPosition(0, 308);
        button->setWidth(640);
        button->setAlignment(ui::Label::AlignHCenter);
        button->setText(txt::StringTable::get(txt::MainMenu_Load));

        button = new ui::Button(funcQuit, m_lblMain);
        button->setFont(fontLarge);
        button->setPosition(0, 408);
        button->setWidth(640);
        button->setAlignment(ui::Label::AlignHCenter);
        button->setText(txt::StringTable::get(txt::MainMenu_QuitGame));
    }

    if (skipToTitle)
        changeState(Title);
    else
        changeState(Presents);
}


void MainMenu::draw()
{
    Menu::draw();

    if (m_lblNew) {
        if (m_time.elapsed() > 300) {
            m_time.start();
            m_cursor = !m_cursor;
        }

        QRect rect = gfx::Font(gfx::Font::Large).draw(m_name, QPoint(0, 328), QSize(640, -1), true, false);
        if (m_cursor)
            gfx::Font(gfx::Font::Large).draw("_", rect.topRight() + QPoint(0, 2));
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
        if (m_lblNew && m_lblNew->isVisible())
            hideNew();
        if (m_lblLoad && m_lblLoad->isVisible())
            hideLoad();
    }
}


void MainMenu::keyPressEvent(QKeyEvent *event)
{
    Menu::keyPressEvent(event);

    if (m_lblNew && m_lblNew->isVisible()) {
        if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {
            m_funcStartGame(m_name.trimmed());
        }

        if (event->key() == Qt::Key_Backspace)
            m_name.chop(1);

        QString text = event->text().toLower().toAscii();
        text.remove(QRegExp("[^a-z\\-0-9 ]*"));
        if (!text.isEmpty())
            m_name += text;

        if (m_name.length() > 20)
            m_name.chop(m_name.length() - 20);
    }
}


void MainMenu::showNew()
{
    m_lblMain->hide();

    Q_ASSERT(!m_lblNew);
    m_lblNew = new ui::Label(&m_title);
    m_name = "dead-eye";

    ui::Label *label = new ui::Label(m_lblNew);
    label->setFont(gfx::Font::Medium);
    label->setPosition(145, 256 - gfx::Font(gfx::Font::Medium).height() - 2);
    label->setText(txt::StringTable::get(txt::MainMenu_NewGame));

    ui::Button *button = new ui::Button([this]() { hideNew(); }, m_lblNew);
    button->setPosition(0, 428);
    button->setWidth(640);
    button->setAlignment(ui::Button::AlignHCenter);
    button->setFont(gfx::Font::Large);
    button->setText(txt::StringTable::get(txt::MainMenu));
}


void MainMenu::hideNew()
{
    Q_ASSERT(m_lblNew);
    m_lblNew->deleteLater();
    m_lblNew = NULL;
    m_lblMain->show();
}


void MainMenu::showLoad()
{
    m_lblMain->hide();

    Q_ASSERT(!m_lblLoad);
    m_lblLoad = new ui::Label(&m_title);

    ui::Label *label = new ui::Label(m_lblLoad);
    label->setFont(gfx::Font::Medium);
    label->setPosition(145, 256 - gfx::Font(gfx::Font::Medium).height() - 2);
    label->setText(txt::StringTable::get(txt::MainMenu_Load));

    int y = 282;

    QList<Chapter::SavedGame> games = Chapter::savedGames();
    qSort(games);
    foreach (const Chapter::SavedGame &game, games) {
        QString name = game.name;
        ui::Button *button = new ui::Button([this, name]() { m_funcLoadGame(name); }, m_lblLoad);
        button->setPosition(0, y);
        button->setWidth(640);
        button->setAlignment(ui::Button::AlignHCenter);
        button->setFont(gfx::Font::Large);
        button->setText(QString("%1: %2").arg(game.name, game.station));

        y += 13;

        ui::Label *label = new ui::Label(m_lblLoad);
        label->setPosition(0, y);
        label->setWidth(640);
        label->setAlignment(ui::Button::AlignHCenter);
        label->setFont(gfx::Font::Small);
        label->setText(game.time.toString(Qt::DefaultLocaleShortDate));

        y += 20;
    }

    ui::Button *button = new ui::Button([this]() { hideLoad(); }, m_lblLoad);
    button->setPosition(0, 428);
    button->setWidth(640);
    button->setAlignment(ui::Button::AlignHCenter);
    button->setFont(gfx::Font::Large);
    button->setText(txt::StringTable::get(txt::MainMenu));
}


void MainMenu::hideLoad()
{
    Q_ASSERT(m_lblLoad);
    m_lblLoad->deleteLater();
    m_lblLoad = NULL;
    m_lblMain->show();
}


} // namespace game
