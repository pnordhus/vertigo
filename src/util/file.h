/***************************************************************************
 *  Copyright (C) 2015  Philipp Nordhus                                    *
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

#ifndef UTIL_FILE_H
#define UTIL_FILE_H

#include "error.h"

#include <fstream>

namespace util {

class File
{
public:
    File();
    File(const std::string &name);

    std::size_t position()
    {
        return m_stream.tellg();
    }

    void skipBytes(std::size_t numBytes);
    void seek(std::size_t offset);

    void read(char *buffer, std::size_t size)
    {
        m_stream.read(buffer, size);
        ASSERT(m_stream.good());
    }

    char getChar()
    {
        auto c = m_stream.get();
        ASSERT(c != std::ifstream::traits_type::eof());
        return c;
    }

    File &operator >> (std::uint32_t &value) {
        readLittleEndian(reinterpret_cast<char*>(&value), sizeof(value));
        return *this;
    }

    File &operator >> (std::int32_t &value) {
        readLittleEndian(reinterpret_cast<char*>(&value), sizeof(value));
        return *this;
    }

private:
    void readLittleEndian(char *buffer, std::size_t size) {
        for (int i = 0; i < size; i++) {
            auto c = m_stream.get();
            ASSERT(c != std::ifstream::traits_type::eof());
            buffer[i] = static_cast<char>(c);
        }
    }

private:
    std::string m_name;
    std::ifstream m_stream;

public:
    static void setDataPath(const std::string &path);

private:
    static std::string m_dataPath;
};

} // namespace util

#endif // UTIL_FILE_H
