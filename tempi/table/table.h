/*
 * Copyright (C) 2011 Alexandre Quessy
 * 
 * This file is part of Tempi.
 * 
 * Tempi is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
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
 * @file
 * The Table class.
 */
#ifndef __TEMPI_TABLE_H__
#define __TEMPI_TABLE_H__

#include <vector>
#include "tempi/message.h"

namespace tempi
{
namespace table
{

/**
 * A Table contains indexed Message objects. 
 */
class Table
{
    public:
        Table();
        /**
         * Clears this Table.
         */
        void reset();
        /**
         * Returns the number of indices in this Table.
         */
        unsigned int getSize() const;
        /**
         * Resizes this Table. Put empty messages in new slots.
         */
        void setSize(unsigned int size);
        /**
         * Adds an event to this Table.
         */
        void append(const Message &message);
        bool set(unsigned int index, const Message &message);
        bool get(unsigned int index, Message &message) const;

    private:
        std::vector<Message> messages_;
};

} // end of namespace
} // end of namespace

#endif // ifndef

