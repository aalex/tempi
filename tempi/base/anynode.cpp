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

namespace tempi {
namespace base {

AnyNode::AnyNode() :
    Node()
{
    addAttribute("value", Message(), "Holds any message to store.", false);
    addOutlet();
}

void AnyNode::processMessage(unsigned int inlet, const Message &message)
{
    if (message.getTypes() == "") // bang only outputs the value
    {
        // pass
    }
    else // any message with some type tags sets the value and outputs it
    {
        setAttribute("value", message);
    }
    if (inlet == 0)
        output(0, getAttributeValue("value"));
}

} // end of namespace
} // end of namespace

