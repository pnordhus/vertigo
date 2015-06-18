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

#ifndef UTIL_EVENT_H
#define UTIL_EVENT_H

#include <functional>
#include <vector>

namespace util {

template<class... Args>
class event
{
public:
    event() { };
    event(const std::function<void(Args...)> &func) { connect(func); };
    event(std::function<void(Args...)> &&func) { connect(std::move(func)); };

public:
    void connect(const std::function<void(Args...)> &func)
    {
        m_handlers.push_back(func);
    }

    void connect(std::function<void(Args...)> &&func)
    {
        m_handlers.push_back(std::move(func));
    }

    void fire(Args... args) const
    {
        for (auto &handler : m_handlers)
            handler(args...);
    }

public:
    void operator+= (const std::function<void(Args...)> &func) { connect(func); }
    void operator+= (std::function<void(Args...)> &&func) { connect(std::move(func)); }
    void operator() (Args... args) const { fire(args...); }

private:
    std::vector<std::function<void(Args...)>> m_handlers;
};

} // namespace util

#endif // UTIL_EVENT_H
