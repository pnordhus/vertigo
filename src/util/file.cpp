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

#include "file.h"

#include "error.h"

#include "SDL.h"

namespace util {

File::File()
{

}

File::File(const std::string &name) :
    m_name(m_dataPath + name)
{
#ifdef _MSC_VER
	wchar_t *nameUTF16 = reinterpret_cast<wchar_t*>(SDL_iconv_string("UTF-16LE", "UTF-8", m_name.c_str(), m_name.size() + 1));
	m_stream.open(nameUTF16, std::ios_base::in | std::ios_base::binary);
	SDL_free(nameUTF16);
#else
	m_stream.open(m_name, std::ios_base::in | std::ios_base::binary);
#endif
    ASSERT(m_stream.good());
}

void File::skipBytes(std::size_t numBytes)
{
    m_stream.seekg(numBytes, std::ios_base::cur);
}

void File::seek(std::size_t offset)
{
    m_stream.seekg(offset, std::ios_base::beg);
}

void File::setDataPath(const std::string &path)
{
    m_dataPath = path + '/';
}

std::string File::m_dataPath;

} // namespace util
