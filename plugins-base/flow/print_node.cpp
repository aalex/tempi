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

namespace tempi { namespace base {

PrintNode::PrintNode() :
    Node(),
    prefix_("print: "),
    enabled_(true)
{
    setShortDocumentation("Prints incoming messages to the text console.");
    //addOutlet();
    addInlet("0", "Print messages received from this inlet.");

    Message prefix_prop = Message("s", prefix_.c_str());
    addAttribute(Attribute::ptr(new Attribute("prefix", prefix_prop, "Prefix when printing incoming messages to the console.")));

    Message enabled_prop = Message("b", enabled_);
    addAttribute(Attribute::ptr(new Attribute("enabled", enabled_prop)));
}

void PrintNode::processMessage(const char *inlet, const Message &message)
{
    if (enabled_)
        std::cout << prefix_ << message << std::endl;
}

void PrintNode::onAttributeChanged(const char *name, const Message &value)
{
    if (utils::stringsMatch(name, "prefix"))
        prefix_ = value.getString(0);
    else if (utils::stringsMatch(name, "enabled"))
        enabled_ = value.getBoolean(0);
}

} // end of namespace
} // end of namespace

