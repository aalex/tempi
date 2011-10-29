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
 * The TableWriter class.
 */
#ifndef __TEMPI_TABLE_WRITER_H__
#define __TEMPI_TABLE_WRITER_H__

#include "tempi/table/table.h"
#include "tempi/message.h"

namespace tempi {
namespace table {

/**
 * A TableWriter writes events to a Table.
 */
class TableWriter
{
    public:
        TableWriter(Table *track);
        void setTable(Table *track);
        Table *getTable();
        /**
         * Adds an event to the table.
         */
        bool write(unsigned int index, const Message &message);
        bool append(const Message &message);
    private:
        Table *table_;
};

} // end of namespace
} // end of namespace

#endif // ifndef

