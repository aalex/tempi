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
    Node()
{
    Message value = Message();
    addProperty("value", value);
}

void AnyNode::onSetArguments(const Message &message)
{
    setProperty("value", message);
}

void AnyNode::processMessage(unsigned int inlet, const Message &message)
{
    // bang outputs the value.
    // A new message replaces the value and 
    if (message.getTypes() != "")
        setProperty("value", message);
    if (inlet == 0)
        output(0, getProperty("value"));
}

} // end of namespace
} // end of namespace

