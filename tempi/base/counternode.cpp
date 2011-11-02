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
#include <iostream>

namespace tempi { namespace base {

const char * const CounterNode::PROP_INCREMENT = "increment";
const char * const CounterNode::PROP_COUNT = "count";

CounterNode::CounterNode() :
    Node(),
    increment_(1)
{
    addOutlet();
    Message increment = Message("i", 1);
    addProperty(PROP_INCREMENT, increment);
    Message count = Message("i", 0);
    addProperty(PROP_COUNT, count);
}
void CounterNode::processMessage(unsigned int inlet, const Message &message)
{
    // bang outputs and increments
    if (message.getTypes() == "")
    {
        int count = getProperty(PROP_COUNT).getInt(0);
        int increment = getProperty(PROP_INCREMENT).getInt(0);
        output(0, getProperty(PROP_COUNT));
        count += increment;
        setProperty(PROP_COUNT, Message("i", count));
    }
    // int replaces the value and outputs
    else if (message.getTypes() == "i")
    {
        count_ = message.getInt(0);
        setProperty(PROP_COUNT, message);
        output(0, getProperty(PROP_COUNT));
    }
    else
    {
        std::cerr << "CounterNode::" << __FUNCTION__ << ": No method for " << message << std::endl;
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

