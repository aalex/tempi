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
#include "plugins-base/flow/typenode.h"
#include "tempi/utils.h"

namespace tempi {
namespace plugins_base {

const char * const TypeNode::MESSAGE_INLET = "message";
const char * const TypeNode::TYPES_OUTLET = "types";

TypeNode::TypeNode() :
    Node()
{
    this->setShortDocumentation("Outputs the type tags of the messages sent to it. (as a string)");
    this->addInlet(MESSAGE_INLET, "Receives messages and detect its type tags.");
    this->addOutlet(TYPES_OUTLET, "Outputs the type tags of the atoms of the incoming messages. (as a string)");
}

void TypeNode::processMessage(const char *inlet, const Message &message)
{
    if (! utils::stringsMatch(inlet, MESSAGE_INLET))
        return;
    this->output(TYPES_OUTLET, Message("s", message.getTypes().c_str()));
}

} // end of namespace
} // end of namespace

