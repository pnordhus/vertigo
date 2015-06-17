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

#include <cstdint>
#include <fstream>
#include <vector>

namespace util {

class File
{
public:
    File();
    File(const std::string &name);

    void open(const std::string &name);

    bool atEnd() const
    {
        return m_stream.eof();
    }

    std::size_t position()
    {
        return static_cast<std::size_t>(m_stream.tellg());
    }

    void skipBytes(std::size_t numBytes);
    void seek(std::size_t offset);

    void read(char *buffer, std::size_t size)
    {
        m_stream.read(buffer, size);
        ASSERT(m_stream.good());
    }

    std::vector<char> read(std::size_t size)
    {
        std::vector<char> buffer(size, 0);
        read(buffer.data(), buffer.size());
        return buffer;
    }

    char getChar()
    {
        auto c = m_stream.get();
        ASSERT(c != std::ifstream::traits_type::eof());
        return c;
    }

    template <typename T>
    typename std::enable_if<std::is_integral<T>::value && !std::is_same<T, bool>::value, File&>::type
    operator >> (T &value) {
        typename std::make_unsigned<T>::type tmp = 0;
        for (std::size_t i = 0; i < sizeof(T); ++i) {
            auto c = m_stream.get();
            ASSERT(c != std::ifstream::traits_type::eof());
            tmp = static_cast<decltype(tmp)>(c) << (8 * i) | tmp;
        }
        value = *reinterpret_cast<T*>(&tmp);
        return *this;
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
