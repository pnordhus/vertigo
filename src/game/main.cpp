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

#include "txt/stringtable.h"

#include <QApplication>
#include <QDir>
#include <QFileDialog>
#include <QSettings>
#include <QTime>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    qsrand(QTime::currentTime().second() * 1000 + QTime::currentTime().msec());

    app.setOrganizationName("Vertigo");
    app.setApplicationName("Vertigo");

    QString scenario;
    if (argc > 1)
        scenario = argv[1];

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

    game::Vertigo vertigo;
    if (!vertigo.start(scenario))
        return 1;

    return app.exec();
}
