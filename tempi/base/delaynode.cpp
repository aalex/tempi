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

#include "tempi/base/delaynode.h"
#include "tempi/utils.h"

namespace tempi {
namespace base {

DelayNode::DelayNode() :
    Node()
{
    addAttribute(Attribute::ptr(new Attribute("delay", Message("l", 0), "Delay in milliseconds. All messages will be delayed by that much.")));
    addInlet("0", "Any message will be sent from outlet 0 after a given delay.");
    addInlet("clear", "Clears all the stored messages.");
    addOutlet("0", "Delayed messages. Save as those received from inlet 0, but later.");
}

void DelayNode::processMessage(const char *inlet, const Message &message)
{
    if (utils::stringsMatch(inlet, "0"))
    {
        if (getAttributeValue("delay").getLong(0) == 0)
        {
            output("0", message);
        }
        else
        {
            TimePosition now = timer_.now();
            events_.push_back(Event(now, message));
        }
    }
    else if (utils::stringsMatch(inlet, "clear"))
    {
        events_.clear();
    }
}

void DelayNode::doTick()
{
    using timeposition::from_ms;
    using timeposition::to_ms;
    int count = 0;
    TimePosition now = timer_.now();
    // std::cout << "delay " << getAttributeValue("delay").getLong(0) << std::endl;
    TimePosition delay = from_ms(
        (unsigned long long) getAttributeValue("delay").getLong(0));
    TimePosition thresh = now - delay;

    // std::cout << "delay =  " << to_ms(delay) << std::endl;
    // std::cout << "now =    " << to_ms(now) << std::endl;
    // std::cout << "thresh = " << to_ms(thresh) << std::endl;

    std::vector<Event>::const_iterator iter;
    for (iter = events_.begin(); iter != events_.end(); ++iter)
    {
        TimePosition pos = (*iter).get<0>();
        //std::cout << __FUNCTION__ << " " << pos << " <= " << thresh << std::endl;
        if (pos <= thresh)
        {
            //std::cout << "output " << pos << " " << (*iter).get<1>() << std::endl;
            output("0", (*iter).get<1>());
            count++;
        }
        else
            break;
    }
    if (count != 0)
    {
        events_.erase(events_.begin(), events_.begin() + count);
    }
}

} // end of namespace
} // end of namespace

