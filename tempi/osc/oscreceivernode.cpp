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

#include "tempi/osc/oscreceivernode.h"
#include "tempi/utils.h"
#include <iostream>

namespace tempi { namespace osc {

OscReceiverNode::OscReceiverNode() :
    Node()
{
    addOutlet();
    Message port = Message("i", 0);
    addProperty("port", port);
}

void OscReceiverNode::onPropertyChanged(const char *name, const Message &value)
{
    //std::cout << "OscReceiverNode::" << __FUNCTION__ << "(" << name << ", " << value << ")" << std::endl;
    if (utils::stringsMatch("port", name))
    {
        //std::cout << "OscReceiver::" << __FUNCTION__ << " listen on port " << value.getInt(0) << std::endl;
        osc_receiver_.reset(new OscReceiver((unsigned int) value.getInt(0)));
    }
}

void OscReceiverNode::doTick()
{
    if (osc_receiver_.get() == 0)
    {
        std::cerr << "OscReceiverNode::" << __FUNCTION__ << " not initialized. Please specifiy a port number." << std::endl;
        return;
    }
    std::vector<Message> messages = osc_receiver_->poll();
    std::vector<Message>::iterator iter;
    for (iter = messages.begin(); iter != messages.end(); ++iter)
    {
        output(0, *iter);
    }
}

} // end of namespace
} // end of namespace

