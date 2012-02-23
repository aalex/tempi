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

#include "plugins-base/flow/counternode.h"
#include "tempi/utils.h"
#include <iostream>

namespace tempi { namespace base {

const char * const CounterNode::PROP_INCREMENT = "increment";
const char * const CounterNode::PROP_COUNT = "count";

CounterNode::CounterNode() :
    Node(),
    increment_(1)
{
    setShortDocumentation("Counts from its start value, incrementing or decrementing the count when it gets a bang.");
    addOutlet("0", "Count.");
    addInlet("0", "Bangs increase and output the count. Integers sets and outputs the count.");
    Message increment = Message("i", 1);
    addAttribute(Attribute::ptr(new Attribute(PROP_INCREMENT, increment)));
    Message count = Message("i", 0);
    addAttribute(Attribute::ptr(new Attribute(PROP_COUNT, count, "Current count value.")));
}

void CounterNode::processMessage(const char *inlet, const Message &message)
{
    // TODO: check if inlet == "0"
    // bang outputs and increments
    if (message.getTypes() == "")
    {
        int count = getAttributeValue(PROP_COUNT).getInt(0);
        int increment = getAttributeValue(PROP_INCREMENT).getInt(0);
        output("0", getAttributeValue(PROP_COUNT));
        count += increment;
        setAttributeValue(PROP_COUNT, Message("i", count));
    }
    // int replaces the value and outputs
    else if (message.getTypes() == "i")
    {
        count_ = message.getInt(0);
        setAttributeValue(PROP_COUNT, message);
        output("0", getAttributeValue(PROP_COUNT));
    }
    else
    {
        std::cerr << "CounterNode::" << __FUNCTION__ << ": No method for " << message << std::endl;
    }
}

void CounterNode::onAttributeChanged(const char *name, const Message &value)
{
    if (utils::stringsMatch("increment", name))
    {
        increment_ = value.getInt(0);
    }
}

} // end of namespace
} // end of namespace
