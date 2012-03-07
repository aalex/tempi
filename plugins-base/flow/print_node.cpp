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
#include "plugins-base/flow/print_node.h"
#include "tempi/utils.h"
#include "tempi/log.h"

namespace tempi {
namespace plugins_base {

PrintNode::PrintNode() :
    Node()
{
    setShortDocumentation("Prints incoming messages to the text console.");
    //addOutlet();
    addInlet("0", "Print messages received from this inlet.");

    Message prefix_prop = Message("s", "print: ");
    addAttribute(Attribute::ptr(new Attribute("prefix", prefix_prop, "Prefix when printing incoming messages to the console.")));

    Message enabled_prop = Message("b", true);
    addAttribute(Attribute::ptr(new Attribute("enabled", enabled_prop)));
}

void PrintNode::processMessage(const char *inlet, const Message &message)
{
    if (getAttributeValue("enabled").getBoolean(0))
    {
        std::ostringstream os;
        os << "[print]: Printing since enabled is true.";
        Logger::log(DEBUG, os.str().c_str());
        std::cout << getAttributeValue("prefix").getString(0) << message << std::endl;
    }
}

void PrintNode::onNodeAttibuteChanged(const char *name, const Message &value)
{
    // pass
}

} // end of namespace
} // end of namespace

