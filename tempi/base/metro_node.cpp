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

#include "tempi/base/metro_node.h"
#include "tempi/timeposition.h"

namespace tempi {
namespace base {

MetroNode::MetroNode() :
    Node()
{
    Message running = Message("b", false);
    addAttribute("running", running);

    Message interval = Message("i", 1000);
    addAttribute("interval", interval, "Interval in milliseconds."); // ms

    addOutlet("0");
}

void MetroNode::onAttributeChanged(const char *name, const Message &value)
{
    const static std::string running("running");
//  const static std::string interval("interval");
    //std::cout << "MetroNode::" << __FUNCTION__ << ": " << name << " = " << value << std::endl;
    if (running == name)
    {
        if (value.getBoolean(0))
        {
            Message message = Message(""); // bang
            output("0", message);
            startMetro();
        }
        //else
        //    startMetro(); // we restart it anyways
    }
}

void MetroNode::startMetro()
{
    timer_.reset();
}

void MetroNode::doTick()
{
    using timeposition::from_ms;
    using timeposition::to_ms;
    //std::cout << "MetroNode::" << __FUNCTION__ << " running:" << getAttributeValue("running").getBoolean(0) << std::endl;
    if (getAttributeValue("running").getBoolean(0))
    {
        TimePosition interval = from_ms((unsigned long long) getAttributeValue("interval").getInt(0));
        TimePosition elapsed = timer_.elapsed();
        //std::cout << "MetroNode::" << __FUNCTION__ << " interval:" << to_ms(interval) << " elapsed:" << elapsed << std::endl;
        if (elapsed >= interval)
        {
            Message message = Message(""); // bang
            //std::cout << "TICK" << std::endl;
            output("0", message);
            timer_.reset();
        }
    }
}

} // end of namespace
} // end of namespace

