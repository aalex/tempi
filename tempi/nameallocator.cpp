/*
 * Copyright (C) 2011 Alexandre Quessy
 *
 * This file is part of Tempi.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software ither version 3 of the License, or
 * (at your option) any later version.
 * 
 * Tempi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with Tempi.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

#include "tempi/nameallocator.h"
#include "tempi/log.h"
#include <algorithm> // std::find

namespace tempi
{

std::string NameAllocator::allocateName(const char *prefix)
{
    int i = 0;
    while (true)
    {
        std::ostringstream tmp;
        tmp << prefix << i;
        std::string name = tmp.str();
        if (hasName(name.c_str()))
        {
            std::ostringstream os;
            os << "NameAllocator: Already has " << name;
            Logger::log(DEBUG, os.str().c_str());
        }
        else
        {
            names_.push_back(name);
            {
                std::ostringstream os;
                os << "NameAllocator: Allocating " << name;
                Logger::log(DEBUG, os.str().c_str());
            }
            return name;
        }
        ++i; // important!
    }
}

bool NameAllocator::freeName(const char *name)
{
    if (hasName(name))
    {
        {
            std::ostringstream os;
            os << "NameAllocator: Freeing " << name;
            Logger::log(DEBUG, os.str().c_str());
        }
        names_.erase(std::find(names_.begin(), names_.end(),
            std::string(name)));
        return true;
    }
    else
        return false;
}

bool NameAllocator::hasName(const char *name)
{
    return std::find(names_.begin(), names_.end(), std::string(name))
        != names_.end();
}

std::vector<std::string> NameAllocator::listNames() const
{
    return names_;
}

} // end of namespace

