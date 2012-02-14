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

#include <iostream>
#include "tempi/base/receivenode.h"

namespace tempi { namespace base {

ReceiveNode::ReceiveNode() :
    Node()
{
    Message receiveSymbol;
    receiveSymbol.appendString("");
    addAttribute(Attribute::ptr(new Attribute("symbol", receiveSymbol, "Receive symbol")));
}

void ReceiveNode::processMessage(const char *inlet, const Message &message)
{
    // if (inlet == 0)
    //    output(message);
}

std::string ReceiverNode::getReceiveSymbol() const
{
    return getAttributeValue("symbol").getString(0);
}

} // end of namespace
} // end of namespace

