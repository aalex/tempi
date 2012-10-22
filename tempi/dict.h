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
 * @file
 * The Dict class.
 */
#ifndef __TEMPI_DICT_H__
#define __TEMPI_DICT_H__

#include <map>
#include "tempi/message.h"

namespace tempi {
/**
 * @namespace tempi::dict
 * Dictionnaries which are key-value pairs.
 */
namespace dict {

/**
 * A Dict contains key-value whose keys strings and values Messages. 
 */
class Dict
{
    public:
        Dict();
        /**
         * Clears this Dict.
         */
        void reset();
        /**
         * Adds a value to this Dict.
         */
        bool set(const char *key, const Message &value);
        /**
         * Retrieves a value from this Dict.
         */
        bool get(const char *key, Message &message) const;
        std::vector<std::string> listKeys() const;
        bool has(const char *key) const;
        bool remove(const char *key);
    private:
        typedef std::map<std::string, Message> Iter;
        typedef std::map<std::string, Message>::const_iterator ConstIter;
        std::map<std::string, Message> messages_;
};

} // end of namespace
} // end of namespace

#endif // ifndef

