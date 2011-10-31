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

#include "tempi/osc/oscsendernode.h"
#include "tempi/utils.h"
#include <iostream>

namespace tempi {
namespace osc {

OscSenderNode::OscSenderNode() :
    Node()
{
    Message host_port;
    host_port.appendString("localhost");
    host_port.appendInt(0);
    addProperty("host_port", host_port);
}

void OscSenderNode::processMessage(unsigned int inlet, const Message &message)
{
    // TODO: send osc message
    if (osc_sender_.get() == 0)
        std::cerr << "OscSenderNode::" << __FUNCTION__ << ": OscSender has no address nor port specified." << std::endl;
    else
        osc_sender_->sendMessage(message);
}

void OscSenderNode::onPropertyChanged(const char *name, const Message &value)
{
    //std::cout << "OscSenderNode::" << __FUNCTION__ << "(" << name << ", " << value << ")" << std::endl;
    if (utils::stringsMatch("host_port", name))
    {
        osc_sender_.reset(new OscSender(value.getString(0).c_str(), value.getInt(1)));
    }
}

} // end of namespace
} // end of namespace

