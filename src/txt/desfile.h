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

#ifndef TXT_DESFILE_H
#define TXT_DESFILE_H


#include <QVariant>


namespace txt {


class DesFile
{
public:
    DesFile();
    DesFile(const QString &filename);

public:
    bool load(const QString &filename);
    bool save(const QString &filename) const;
    void setSection(const QString &section);
    bool contains(const QString &key) const;
    QVariant value(const QString &key, const QVariant &defaultValue = QVariant()) const;
    void setValue(const QString &key, const QVariant &value);
    QString valueText(const QString &key) const;
    QStringList sections() const;
    QStringList keys() const;

private:
    QMap<QString, QVariantMap> m_sections;
    QVariantMap *m_section;
};


} // namespace txt


#endif // TXT_DESFILE_H
