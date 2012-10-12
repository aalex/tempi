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
namespace plugins_base {

OscReceiverNode::OscReceiverNode() :
    Node(),
    port_number_(0)
{
    setShortDocumentation("Receives OSC messages.");
    setLongDocumentation("Warning: If UDP port is already in use, it should crash your application."); // FIXME
    addOutlet("incoming");
    Message port = Message("i", 0);
    addAttribute(Attribute::ptr(new Attribute("port", port, "Receive OSC messages on this port number.")));
    addAttribute(Attribute::ptr(new Attribute("listening", Message("b", false), "Internal status of the object.")));
    this->getAttribute("listening")->setMutable(false); // Cannot be changed via messages to the __attr__ inlet
}

bool OscReceiverNode::onNodeAttributeChanged(const char *name, const Message &value)
{
    if (Logger::isEnabledFor(DEBUG))
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
            Logger::log(ERROR, os.str().c_str());
            return false;
        }
        unsigned int portNumber = (unsigned int) tmp;
        if (portNumber == port_number_)
        {
            if (Logger::isEnabledFor(DEBUG))
            {
                std::ostringstream os;
                os << "OscReceiver::" << __FUNCTION__ << " already listening on port " << portNumber;
                Logger::log(DEBUG, os.str().c_str());
            }
            return false;
        }
        port_number_ = portNumber;
        if (Logger::isEnabledFor(INFO))
        {
            std::ostringstream os;
            os << "OscReceiver::" << __FUNCTION__ << " listen on port " << portNumber;
            Logger::log(INFO, os.str().c_str());
        }
        if (portNumber == 0)
        {
            osc_receiver_.reset((osc::OscReceiver *) 0);
            this->getAttribute("listening")->setValue(Message("b", false));
        }
        else
        {
            osc_receiver_.reset(new osc::OscReceiver(portNumber));
            this->getAttribute("listening")->setValue(Message("b", true));
        }
        return true;
    }
    else
        return true;
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
        // std::ostringstream os;
        // os << "OscReceiverNode." << __FUNCTION__ << ": " << (*iter);
        // Logger::log(DEBUG, os.str().c_str());
        output("incoming", *iter);
    }
}

} // end of namespace
} // end of namespace

