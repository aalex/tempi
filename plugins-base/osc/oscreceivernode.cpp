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
    Node(),
    port_number_(0)
{
    addOutlet("incoming");
    Message port = Message("i", 0);
    addAttribute(Attribute::ptr(new Attribute("port", port, "Receive OSC messages on this port number.")));
}

void OscReceiverNode::onAttributeChanged(const char *name, const Message &value)
{
    {
    std::ostringstream os;
    os << "OscReceiverNode::" << __FUNCTION__ << "(\"" << name << "\", " << value << ")";
    Logger::log(DEBUG, os.str().c_str());
    }
    if (utils::stringsMatch("port", name))
    {
        int tmp = value.getInt(0);
        if (tmp < 0)
        {
            std::ostringstream os;
            os << "OscReceiverNode::" << __FUNCTION__ << ": Negative port numbers are not supported: " << tmp;
            Logger::log(DEBUG, os.str().c_str());
            return;
        }
        unsigned int portNumber = (unsigned int) tmp;
        if (portNumber == port_number_)
        {
            std::ostringstream os;
            os << "OscReceiver::" << __FUNCTION__ << " already listening on port " << portNumber;
            Logger::log(INFO, os.str().c_str());
            return;
        }
        port_number_ = portNumber;
        {
            std::ostringstream os;
            os << "OscReceiver::" << __FUNCTION__ << " listen on port " << portNumber;
            Logger::log(INFO, os.str().c_str());
        }
        if (portNumber == 0)
            osc_receiver_.reset((OscReceiver *) 0);
        else
            osc_receiver_.reset(new OscReceiver(portNumber));
    }
}

void OscReceiverNode::doTick()
{
    static bool printed_no_port_warning = false;
    if (osc_receiver_.get() == 0)
    {
        if (! printed_no_port_warning)
        {
            std::ostringstream os;
            os << "OscReceiverNode." << __FUNCTION__ << ": Please specifiy a port number.";
            Logger::log(WARNING, os.str().c_str());
            printed_no_port_warning = true;
        }
        return;
    }
    std::vector<Message> messages = osc_receiver_->poll();
    std::vector<Message>::iterator iter;
    for (iter = messages.begin(); iter != messages.end(); ++iter)
    {
        std::ostringstream os;
        os << "OscReceiverNode." << __FUNCTION__ << ": " << (*iter);
        Logger::log(DEBUG, os.str().c_str());
        output("incoming", *iter);
    }
}

} // end of namespace
} // end of namespace

