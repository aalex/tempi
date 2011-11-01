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

#include "tempi/base/counternode.h"
#include "tempi/utils.h"

namespace tempi { namespace base {

CounterNode::CounterNode() :
    Node(),
    count_(0),
    increment_(1)
{
    addOutlet();
    Message increment = Message("i", 1);
    addProperty("increment", increment);
}
void CounterNode::processMessage(unsigned int inlet, const Message &message)
{
    if (message.getTypes() == "i")
    {
        count_ = message.getInt(0);
        output(0, message);
    }
    else
    {
        Message result = Message("i", count_);
        output(0, result);
        count_ += increment_;
    }
}

void CounterNode::onPropertyChanged(const char *name, const Message &value)
{
    if (utils::stringsMatch("increment", name))
    {
        increment_ = value.getInt(0);
    }
}

} // end of namespace
} // end of namespace

