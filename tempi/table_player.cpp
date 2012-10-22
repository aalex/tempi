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
#include "tempi/table_player.h"
#include <iostream>

namespace tempi { namespace table {

TablePlayer::TablePlayer(Table *table) :
    table_(table),
    speed_(1)
{
    // pass
}

void TablePlayer::setTable(Table *table)
{
    table_ = table;
}

Table *TablePlayer::getTable() const
{
    return table_;
}

unsigned int TablePlayer::getIndex() const
{
    return index_;
}

bool TablePlayer::setIndex(unsigned int index)
{
    if (table_->getSize() == 0)
    {
        if (index == 0)
        {
            index_ = 0;
            return true;
        }
        else
        {
            index_ = 0;
            return false;
        }
    }
    else if (index >= table_->getSize())
    {
        index_ = table_->getSize();
        return false;
    }
    else
    {
        index_ = index;
        return true;
    }
}

bool TablePlayer::read(Message &message)
{
    if (! table_)
    {
        std::cout << "TablePlayer::" << __FUNCTION__ <<  "This table player has an invalid Table. Cannot read events." << std::endl;
        return false;
    }
    else if (table_->getSize() == 0)
    {
        //std::cout << "No point to read." << std::endl;
        return false;
    }
    else
    {
        table_->get(index_, message);
        index_ = (index_ + speed_) % table_->getSize();
        return true;
    }
}

void TablePlayer::setSpeed(unsigned int factor)
{
    speed_ = factor;
}

unsigned int TablePlayer::getSpeed() const
{
    return speed_;
}

} // end of namespace
} // end of namespace

