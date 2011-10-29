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

#include <iostream>
#include "tempi/base/print_node.h"
#include "tempi/utils.h"

namespace tempi { namespace base {

PrintNode::PrintNode() :
    Node(),
    prefix_("print: "),
    enabled_(true)
{
    //addOutlet();

    Message prefix_prop = Message("s", prefix_.c_str());
    addProperty("prefix", prefix_prop);

    Message enabled_prop = Message("b", enabled_);
    addProperty("enabled", enabled_prop);
}

void PrintNode::processMessage(unsigned int inlet, const Message &message)
{
    if (enabled_)
        std::cout << prefix_ << message << std::endl;
}

void PrintNode::onPropertyChanged(const char *name, const Message &value)
{
    if (utils::stringsMatch(name, "prefix"))
        prefix_ = value.getString(0);
    else if (utils::stringsMatch(name, "enabled"))
        enabled_ = value.getBoolean(0);
}

} // end of namespace
} // end of namespace

