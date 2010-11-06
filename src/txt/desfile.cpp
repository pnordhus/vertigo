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

#include "desfile.h"
#include <QDir>
#include <QFile>
#include <QStringList>
#include <QTextStream>


QString trimEnd(const QString& str)
{
    int i;
    for (i = str.length() - 1; i >= 0 && str[i].isSpace(); i--);
    QString res = str;
    res.truncate(i + 1);
    return res;
}


namespace txt {


DesFile::DesFile()
{
    setSection("Global");
}


DesFile::DesFile(const QString &filename)
{
    load(filename);
}


bool DesFile::load(const QString &filename)
{
    m_sections.clear();
    setSection("Global");

    QFile file(filename);
    if (!file.open(QFile::ReadOnly))
        return false;

    QRegExp regSection("\\[(.*)\\]");
    QRegExp regKey("(.*)=(.*)");

    while (!file.atEnd()) {
        const QString line = file.readLine();

        if (regSection.indexIn(line) >= 0)
            setSection(regSection.cap(1).trimmed().toLower());

        if (regKey.indexIn(line) >= 0) {
            const QString key = regKey.cap(1).trimmed().toLower();
            if (!key.startsWith(";") && !key.startsWith("/"))
                setValue(key, regKey.cap(2).trimmed());
        }

        if (line.trimmed() == "{")
        {
            QStringList text;
            QString textLine = trimEnd(file.readLine());
            while (!file.atEnd() && textLine != "}")
            {
                text.append(textLine);
                textLine = trimEnd(file.readLine());
            }
            setValue("{text}", text);
        }
    }

    setSection("Global");
    return true;
}

bool DesFile::save(const QString &filename) const
{
    QDir dir;
    dir.mkpath(QFileInfo(filename).path());

    QFile file(filename);
    if (!file.open(QFile::WriteOnly))
        return false;

    QTextStream stream(&file);

    QMapIterator<QString, QVariantMap> itSections(m_sections);
    while (itSections.hasNext()) {
        itSections.next();
        if (itSections.value().isEmpty())
            continue;

        stream << "[" << itSections.key() << "]" << endl;

        QMapIterator<QString, QVariant> itValues(itSections.value());
        while (itValues.hasNext()) {
            itValues.next();

            stream << itValues.key() << " = " << itValues.value().toString() << endl;
        }

        stream << endl;
    }

    return true;
}


void DesFile::setSection(const QString &section)
{
    m_section = &m_sections[section.toLower()];
}


bool DesFile::contains(const QString &key) const
{
    return m_section->contains(key.toLower());
}


QVariant DesFile::value(const QString &key, const QVariant &defaultValue) const
{
    return m_section->value(key.toLower(), defaultValue);
}


void DesFile::setValue(const QString &key, const QVariant &value)
{
    m_section->insert(key.toLower(), value);
}


QStringList DesFile::sections() const
{
    return m_sections.keys();
}


QStringList DesFile::keys() const
{
    return m_section->keys();
}


} // namespace txt
