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

#include "plugins-base/osc/oscreceivernode.h"
#include "tempi/utils.h"
#include "tempi/log.h"
#include <iostream>

namespace tempi {
namespace osc {

OscReceiverNode::OscReceiverNode() :
    Node()
{
    addOutlet("0");
    Message port = Message("i", 0);
    addAttribute(Attribute::ptr(new Attribute("port", port, "Receive OSC messages on this port number.")));
}

void OscReceiverNode::onAttributeChanged(const char *name, const Message &value)
{
    std::ostringstream os;
    os << "OscReceiverNode::" << __FUNCTION__ << "(\"" << name << "\", " << value << ")" << std::endl;
    Logger::log(DEBUG, os.str().c_str());
    if (utils::stringsMatch("port", name))
    {
        unsigned int portNumber = value.getInt(0);
        {
            std::ostringstream os;
            os << "OscReceiver::" << __FUNCTION__ << " listen on port " << portNumber << std::endl;
            Logger::log(WARNING, os.str().c_str());
        }
        if (portNumber == 0)
            osc_receiver_.reset((OscReceiver *) 0);
        else
            osc_receiver_.reset(new OscReceiver(portNumber));
    }
}

void OscReceiverNode::doTick()
{
    if (osc_receiver_.get() == 0)
    {
        // std::cerr << "OscReceiverNode::" << __FUNCTION__ << "(): OscReceiver is NULL. Cannot poll incoming OSC messages. Please specifiy a port number." << std::endl;
        return;
    }
    std::vector<Message> messages = osc_receiver_->poll();
    std::vector<Message>::iterator iter;
    for (iter = messages.begin(); iter != messages.end(); ++iter)
    {
        output("0", *iter);
    }
}

} // end of namespace
} // end of namespace

