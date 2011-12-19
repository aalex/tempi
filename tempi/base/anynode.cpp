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

#include "tempi/base/anynode.h"

namespace tempi { namespace base {

AnyNode::AnyNode() :
    Node(),
    value_("")
{
//    Message value = Message();
//    addProperty("value", value);
    addOutlet();
}

void AnyNode::onSetArguments(const Message &message)
{
//    setProperty("value", message);
//    std::cout << "AnyNode::" << __FUNCTION__ << std::endl;
    Message v = message;
    value_ = v;
//    std::cout << "done" << std::endl;
}

void AnyNode::processMessage(unsigned int inlet, const Message &message)
{
//    std::cout << "AnyNode::" << __FUNCTION__ << std::endl;
    // bang outputs the value.
    // A new message replaces the value and 
    if (message.getTypes() != "")
    {
        Message v = message;
        value_ = v;
    }
        //setProperty("value", message);
    if (inlet == 0)
        output(0, value_);
//    std::cout << "done" << std::endl;
}

} // end of namespace
} // end of namespace

