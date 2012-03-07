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

/**
 * The NameAllocator class.
 */

#ifndef __TEMPI_NAME_ALLOCATOR_H__
#define __TEMPI_NAME_ALLOCATOR_H__

#include <string>
#include <vector>

namespace tempi
{

/**
 * Allocates names for Entity instances by appending an incremental number to a given string.
 */
class NameAllocator 
{
    public:
        std::string allocateName(const char *prefix);
        bool freeName(const char *name);
        bool hasName(const char *name);
        std::vector<std::string> listNames() const;
    private:
        std::vector<std::string> names_;
};

} // end of namespace

#endif // ifndef

