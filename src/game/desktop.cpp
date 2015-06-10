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
#include "desktop.h"
#include "gfx/image.h"
#include "gfx/font.h"
#include "sfx/soundsystem.h"
#include "txt/desfile.h"
#include <QKeyEvent>


namespace game {


Desktop::Desktop(const QString &name) :
    m_notebook([this]() { hideNotebook(); }),
    m_room(NULL),
    m_dialog(NULL),
    m_enCom(NULL),
    m_departure(NULL),
    m_depot(NULL),
    m_miniMovie("gfx:mvi/desktop", [this]() {}),
    m_first(true)
{
    const gfx::ColorTable colorTable("gfx:pal/gui/border.pal");

    txt::DesFile file("dat:world/" + name + ".des");

    file.setSection("Station");

    const QString background = file.value("BackGround").toString();
    const QString backgroundSound = file.value("Sound").toString();
    const QString nameSound = "st" + file.value("SFV").toString().right(4);
    QImage image = gfx::Image::loadPCX("gfx:pic/bground/" + background + ".pcx").toRgb565();
    m_miniMovie.setColorTable(image.colorTable());
    m_background.fromImage(image);
    m_backgroundSound.load("sfx:snd/bground/" + backgroundSound + ".pcl", "sfx:snd/bground/" + backgroundSound + ".pcr");
    m_nameSound.load("sfx:snd/names/" + nameSound + ".pcm");
    m_approachMovie = QString("gfx:mvi/approach/%1.mvi").arg(file.value("ApproachMovie").toString());
    m_name = file.value("Name").toString();

    m_lblBackground.setTexture(m_background);
    setRootWidget(&m_lblBackground);

    ui::Label *label;

    label = new ui::Label(&m_lblBackground);
    label->setFont(gfx::Font::Large);
    label->setPosition(8, 8);
    label->setText(m_name);

    label = new ui::Label(&m_lblBackground);
    label->setFont(gfx::Font::Small);
    label->setPosition(8, 10 + gfx::Font(gfx::Font::Large).height());
    label->setText(file.value("Description").toString());

    m_btnNotebook = new ui::Button([this]() { showNotebook(); }, &m_lblBackground);
    m_btnNotebook->setTexture(gfx::Image::loadPCX("gfx:pic/notebook/nbklein.pcx"));
    m_btnNotebook->setPosition(572, 424);

    m_notebook.hide();

    m_widgetRooms = new ui::Label(&m_lblBackground);
    foreach (const QString &section, file.sections().filter(QRegExp("^room"))) {
        file.setSection(section);

        QRegExp reg("^room(\\d*)$");
        reg.indexIn(section);
        const int index = reg.cap(1).toInt();

        const QString name = file.value("Name").toString();
        const QString arrow = file.value("Arrow").toString();
        const int width = gfx::Font(gfx::Font::Medium).width(name);

        const bool left = arrow.endsWith("Left");
        const bool top = arrow.startsWith("Top");

        char arrowX = 'l';
        char arrowY = 't';
        QPoint offset(11, 3);
        QPoint offsetArrow;

        if (left) {
            offsetArrow.setX(-10);
            offset.setX(-1 - width);
            arrowX = 'r';
        }

        if (top) {
            offsetArrow.setY(-10);
            offset.setY(-3);
            arrowY = 'b';
        }

        offset += offsetArrow;

        ui::Label *lblArrow = new ui::Label(m_widgetRooms);
        lblArrow->setTexture(gfx::Image::load(QString("gfx:img/desktop/gui/arr%1%2lar.img").arg(arrowY).arg(arrowX), colorTable));
        lblArrow->setPosition(offsetArrow.x() + file.value("X").toInt(), offsetArrow.y() + file.value("Y").toInt());

        ui::Button *button = new ui::Button([this, index]() { showRoom(index); }, m_widgetRooms);
        button->setOffset(0);
        button->setFont(gfx::Font::Medium);
        button->setText(name);
        button->setPosition(offset.x() + file.value("X").toInt(), offset.y() + file.value("Y").toInt());

        RoomEntry room;
        room.name = file.value("room").toString();
        room.title = name;
        m_rooms.insert(index, room);
    }

    m_miniMovie.load(file);
}


Desktop::~Desktop()
{
    delete m_enCom;
    delete m_dialog;
    delete m_departure;
    delete m_room;
    delete m_depot;
}


void Desktop::activate()
{
    m_miniMovie.start();
    m_backgroundSound.playLoop();

    if (m_first) {
        m_first = false;
        m_nameSound.play();
        checkDialogs();
    }
}


void Desktop::deactivate()
{
    m_backgroundSound.stop();
    sfx::SoundSystem::get()->sound(sfx::NotebookBackground)->stop();
}


void Desktop::draw()
{
    m_miniMovie.update(m_background);
    setupOrthographicMatrix(640, 480);
    m_lblBackground.doDraw();
    if (m_room)
        m_room->doDraw();
    if (m_dialog)
        m_dialog->doDraw();
    if (m_enCom)
        m_enCom->doDraw();
    if (m_departure)
        m_departure->doDraw();
    if (m_depot)
        m_depot->doDraw();
    m_notebook.doDraw();
}


void Desktop::showNotebook()
{
    m_btnNotebook->hide();
    m_notebook.show();
    setRootWidget(&m_notebook);

    m_backgroundSound.setVolume(0.3f);
    sfx::SoundSystem::get()->sound(sfx::NotebookShow)->play();
    sfx::SoundSystem::get()->sound(sfx::NotebookBackground)->playLoop();
}


void Desktop::hideNotebook()
{
    m_btnNotebook->show();
    m_notebook.hide();
    setRootWidget(&m_lblBackground);

    sfx::SoundSystem::get()->sound(sfx::NotebookBackground)->stop();
    sfx::SoundSystem::get()->sound(sfx::NotebookHide)->play();
    m_backgroundSound.setVolume(1.0f);
}


void Desktop::showRoom(int index)
{
    Q_ASSERT(!m_room);
    m_room = new Room(index, m_rooms[index].title, m_rooms[index].name,
        [this]() { hideRoom(); },
        [this](Dialog *dialog) { showDialog(dialog); },
        [this](Dialog *dialog) { showEnCom(dialog); },
        [this]() { showDeparture(); },
        [this]() { showDepot(); },
        [this]() { hideCursor(); }
    );
    setRootWidget(m_room);
    m_btnNotebook->hide();
    m_room->setPosition((640 - m_room->width()) / 2, (480 - m_room->height()) / 2);
    m_backgroundSound.setVolume(0.3f);
}


void Desktop::hideRoom()
{
    Q_ASSERT(m_room);
    setRootWidget(&m_lblBackground);
    m_room->deleteLater();
    m_room = NULL;
    m_btnNotebook->show();
    m_backgroundSound.setVolume(1.0f);
    checkDialogs();
}


void Desktop::showDialog(Dialog *dialog)
{
    Q_ASSERT(m_room);
    Q_ASSERT(!m_dialog);

    m_dialog = new DialogFrame(dialog, m_room->name(), [this]() { hideDialog(); });
    setRootWidget(m_dialog);
    m_dialog->setPosition((640 - m_dialog->width()) / 2, (480 - m_dialog->height()) / 2);
}


void Desktop::hideDialog()
{
    Q_ASSERT(m_room);
    Q_ASSERT(m_dialog);
    setRootWidget(m_room);
    m_room->restart();
    m_dialog->deleteLater();
    m_dialog = NULL;
}


void Desktop::showEnCom(Dialog *dialog)
{
    Q_ASSERT(!m_enCom);

    m_enCom = new EnCom(dialog, [this]() { hideEnCom(); });
    setRootWidget(m_enCom);

    m_btnNotebook->hide();
    m_backgroundSound.setVolume(0.3f);
    sfx::SoundSystem::get()->sound(sfx::EnComShow)->play();
    sfx::SoundSystem::get()->sound(sfx::NotebookBackground)->playLoop();
}


void Desktop::hideEnCom()
{
    Q_ASSERT(m_enCom);
    if (m_room) {
        setRootWidget(m_room);
        m_room->restart();
    } else {
        setRootWidget(&m_lblBackground);
    }

    m_enCom->deleteLater();
    m_enCom = NULL;

    m_btnNotebook->show();
    sfx::SoundSystem::get()->sound(sfx::NotebookBackground)->stop();
    sfx::SoundSystem::get()->sound(sfx::EnComHide)->play();
    m_backgroundSound.setVolume(1.0f);

    checkDialogs();
}


void Desktop::showDeparture()
{
    Q_ASSERT(m_room);
    Q_ASSERT(!m_departure);
    m_departure = new Departure([this]() { hideDeparture(); });
    setRootWidget(m_departure);
    showCursor();
    m_room->hide();
}


void Desktop::hideDeparture()
{
    Q_ASSERT(m_room);
    Q_ASSERT(m_departure);
    setRootWidget(m_room);
    m_room->restart();
    m_room->show();
    m_departure->deleteLater();
    m_departure = NULL;
}


void Desktop::showDepot()
{
    Q_ASSERT(m_room);
    Q_ASSERT(!m_depot);
    m_depot = new Depot([this]() { hideDepot(); });
    setRootWidget(m_depot);
    m_depot->setPosition((640 - m_depot->width()) / 2, (480 - m_depot->height()) / 2);
    setClick(false);
}


void Desktop::hideDepot()
{
    Q_ASSERT(m_room);
    Q_ASSERT(m_depot);
    setRootWidget(m_room);
    m_room->restart();
    m_depot->deleteLater();
    m_depot = NULL;
    setClick(true);
}


void Desktop::checkDialogs()
{
    QList<Dialog*> dialogs = Chapter::get()->dialogsDirect();
    if (!dialogs.isEmpty()) {
        Dialog* dialog = dialogs.first();
        showRoom(dialog->room());
        showDialog(dialog);
        return;
    }

    dialogs = Chapter::get()->dialogsEnCom(false);
    if (!dialogs.isEmpty())
        showEnCom(dialogs.first());
}


} // namespace game
