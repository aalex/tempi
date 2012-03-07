/*
 * Copyright (C) 2011 Alexandre Quessy
 * Copyright (C) 2011 Michal Seta
 * Copyright (C) 2012 Nicolas Bouillot
 *
 * This file is part of Tempi.
 *
 * This program is free software: you can redistribute it and/or
 * modify it under the terms of, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Tempi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Tempi.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "tempi/dict.h"

namespace tempi {
namespace dict {

Dict::Dict()
{
    // pass
}

void Dict::reset()
{
    messages_.clear();
}

bool Dict::set(const char *key, const Message &value)
{
    messages_[std::string(key)] = value;
    return true;
}

bool Dict::has(const char *key) const
{
    return (messages_.find(std::string(key)) != messages_.end());
}

bool Dict::get(const char *key, Message &value) const
{
    if (! this->has(key))
    {
        return false;
    }
    else
    {
        ConstIter iter = messages_.find(std::string(key));
        value = (*iter).second;
        return true;
    }
}

bool Dict::remove(const char *key)
{
    if (has(key))
    {
        //ConstIter iter = messages_.find(std::string(key));
        //messages_.erase(iter);
        messages_.erase(std::string(key));
        return true;
    }
    else
        return false;
}

std::vector<std::string> Dict::listKeys() const
{
    std::vector<std::string> ret;
    ConstIter iter;
    for (iter = messages_.begin(); iter != messages_.end(); ++iter)
    {
        ret.push_back((*iter).first);
    }
    return ret;
}

} // end of namespace
} // end of namespace

