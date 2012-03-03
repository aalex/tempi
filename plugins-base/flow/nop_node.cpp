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
#include "plugins-base/flow/nop_node.h"

namespace tempi {
namespace plugins_base {

NopNode::NopNode() :
    Node()
{
    this->setShortDocumentation("Does no operation on a message, and just passes it to its outlet.");
    this->addOutlet("0", "The default outlet.");
    this->addInlet("0", "The default inlet.");
}

void NopNode::processMessage(const char *inlet, const Message &message)
{
    this->output("0", message);
}

} // end of namespace
} // end of namespace

