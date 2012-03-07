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

#include "tempi/table.h"

namespace tempi {
namespace table {

Table::Table()
{
    // pass
}

void Table::reset()
{
    messages_.clear();
}

unsigned int Table::getSize() const
{
    return messages_.size();
}

void Table::append(const Message &message)
{
    messages_.push_back(message);
}

bool Table::set(unsigned int index, const Message &message)
{
    if (index >= messages_.size())
        return false;
    messages_[index] = message;
    return true;
}

bool Table::get(unsigned int index, Message &message) const
{
    if (index >= messages_.size())
        return false;
    message = messages_[index];
    return true;
}

void Table::setSize(unsigned int size)
{
    messages_.resize(size);
}

} // end of namespace
} // end of namespace

