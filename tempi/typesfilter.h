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

/**
 * The TypesFilter class.
 */

#ifndef __TEMPI_TYPESFILTER_H__
#define __TEMPI_TYPESFILTER_H__

#include <string>
#include "tempi/message.h"

namespace tempi {

/**
 * A types filter can be used to make sure message match a given type.
 * It can convert them to the desired type if possible.
 */
class TypesFilter
{
    public:
        /**
         * Constructor.
         */
        static const char * const WILDCARD;
        TypesFilter(const char *filter="*");
        bool setFilter(const char *filter);
        std::string getFilter() const;
        bool matchesFilter(const Message &message) const;
        bool tryCast(const Message &source, Message &result) const;
        bool isWildcard() const;
        static bool isValidFilter(const std::string &filter);
    private:
        std::string filter_;
        bool is_wildcard_;
};

} // end of namespace

#endif // ifndef

