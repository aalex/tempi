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

#include "tempi/table/table_writer.h"
#include <iostream>

namespace tempi {
namespace table {

TableWriter::TableWriter(Table *table)
{
    table_ = table;
}

void TableWriter::setTable(Table *table)
{
    table_ = table;
}

Table *TableWriter::getTable()
{
    return table_;
}

bool TableWriter::write(unsigned int index, const Message &message)
{
    if (table_)
        return table_->set(index, message);
    else
    {
        std::cout << "This TableWriter has an invalid Table. Cannot add event." << std::endl;
        return false;
    }
}

bool TableWriter::append(const Message &message)
{
    if (table_)
    {
        table_->append(message);
        return true;
    }
    else
    {
        std::cout << "This TableWriter has an invalid Table. Cannot append event." << std::endl;
        return false;
    }
}

} // end of namespace
} // end of namespace

