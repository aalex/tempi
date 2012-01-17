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

#include "tempi/base/anynode.h"
#include "tempi/utils.h"

namespace tempi {
namespace base {

AnyNode::AnyNode() :
    Node()
{
    addAttribute("value", Message(), "Holds any message to store.", false);
    addOutlet("0", "Value.");
    addInlet("0", "Bang to output value. Any other message will set and output value.");
}

void AnyNode::processMessage(const char *inlet, const Message &message)
{
    if (message.getTypes() == "") // bang only outputs the value
    {
        // pass
    }
    else // any message with some type tags sets the value and outputs it
    {
        setAttribute("value", message);
    }
    if (utils::stringsMatch(inlet, "0"))
        output("0", getAttributeValue("value"));
}

} // end of namespace
} // end of namespace

