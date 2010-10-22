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
#include "room.h"
#include "gfx/colortable.h"
#include "gfx/font.h"
#include "gfx/image.h"
#include "ui/button.h"


namespace game {


Room::Room(int index, const QString &title, const QString &name) :
    m_index(index),
    m_miniMovie("gfx:mvi/room"),
    m_name(name)
{
    txt::DesFile file("dat:world/" + name + ".des");
    file.beginGroup("Room");

    const QString background = file.value("BackGround").toString();
    const QString backgroundSound = file.value("Sound").toString();
    m_backgroundImage = gfx::Image::loadPCX("gfx:pic/room/" + background + ".pcx").toRgb565();
    m_miniMovie.setColorTable(m_backgroundImage.colorTable());
    m_backgroundSound.load("sfx:snd/room/" + backgroundSound + ".pcm");

    file.endGroup();

    setupFrame(m_backgroundImage.size() + QSize(18, 24), title, true);

    ui::Label *label = new ui::Label(this);
    label->setPosition(9, 15);
    label->setTexture(m_background);

    m_miniMovie.load(file);
    m_miniMovie.start();
    connect(&m_miniMovie, SIGNAL(videoFinished()), SIGNAL(showDeparture()));
    connect(&m_miniMovie, SIGNAL(videoFinished()), SLOT(enable()));

    m_backgroundSound.playLoop();

    if (file.contains("Dock/Name")) {
        file.beginGroup("Dock");

        const QString name = file.value("Name").toString();
        const QString sound = file.value("Sound").toString();
        const int rate = file.value("Rate", 0).toInt();
        m_dockSound.load("sfx:snd/room/" + sound + ".pcm", rate);

        ui::Arrow *arrow = new ui::Arrow(file.value("Arrow").toString(), QPoint(file.value("X").toInt(), file.value("Y").toInt()), false, this);
        arrow->setText(name);
        connect(arrow, SIGNAL(clicked(int)), SLOT(showDock()));

        file.endGroup();
    }

    int personId = 1;
    for (;;) {
        file.beginGroup(QString("Person%1").arg(personId));

        if (file.contains("Arrow")) {

            Person *person = new Person;

            person->arrow = new ui::Arrow(file.value("Arrow").toString(), QPoint(file.value("X").toInt(), file.value("Y").toInt()), false, this);
            person->arrow->hide();
            connect(person->arrow, SIGNAL(clicked(int)), SIGNAL(startDialog(int)));

            m_persons.insert(personId, person);
        } else {
            file.endGroup();
            break;
        }

        file.endGroup();
        personId++;
    }

    restart();
}


Room::~Room()
{
    qDeleteAll(m_persons);
}


void Room::restart()
{
    m_background.fromImage(m_backgroundImage);
    const QList<Dialog*> dialogs = Chapter::get()->dialogs(m_index);

    foreach (Person* person, m_persons)
        person->arrow->hide();

    foreach (Dialog* dialog, dialogs) {
        Person *person = m_persons.value(dialog->person());
        if (!person) {
            foreach (person, m_persons) {
                if (person->arrow->isVisible() == false && person->female == dialog->isFemale())
                    break;
            }
        }

        Q_ASSERT(person);
        person->arrow->show();
        person->arrow->setText(dialog->name());
        person->arrow->setValue(dialog->id());
    }
}


void Room::draw()
{
    m_miniMovie.update(m_background);
    ui::Label::draw();
}


bool Room::mousePressEvent(const QPoint &pos, Qt::MouseButton button)
{
    if (button == Qt::RightButton) {
        emit close();
        return true;
    }

    return false;
}


void Room::showDock()
{
    emit hideCursor();
    disable();
    m_dockSound.play();
    m_miniMovie.playOneshot();
}


} // namespace game
