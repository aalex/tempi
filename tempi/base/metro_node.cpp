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

#include "tempi/base/metro_node.h"

namespace tempi { namespace base {

MetroNode::MetroNode() :
    Node()
{
    Message running = Message("b", false);
    addProperty("running", running);

    Message interval = Message("i", 1000);
    addProperty("interval", interval); // ms

    addOutlet();
}

void MetroNode::onPropertyChanged(const char *name, const Message &value)
{
    const static std::string running("running");
//  const static std::string interval("interval");
//    std::cout << "MetroNode::" << __FUNCTION__ << ": " << name << " = " << value << std::endl;
    if (running == name)
    {
        if (value.getBoolean(0))
            startMetro();
        else
            startMetro(); // we restart it anyways
    }
}


void MetroNode::startMetro()
{
    timer_.reset();
}

void MetroNode::doTick()
{
    //std::cout << "MetroNode::" << __FUNCTION__ << " running:" << getProperty("running").getBoolean(0) << std::endl;
    if (getProperty("running").getBoolean(0))
    {
        TimePosition interval = timeposition::from_ms((unsigned long long) getProperty("interval").getInt(0));
        TimePosition elapsed = timer_.elapsed();
        //std::cout << "MetroNode::" << __FUNCTION__ << " interval:" << interval << " elapsed:" << elapsed << std::endl;
        if (elapsed >= interval)
        {
            Message message = Message(""); // bang
            output(0, message);
            timer_.reset();
        }
    }
}

} // end of namespace
} // end of namespace

