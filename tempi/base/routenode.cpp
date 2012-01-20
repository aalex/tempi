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
#include "tempi/base/routenode.h"

namespace tempi {
namespace base {

RouteNode::RouteNode() :
    Node()
{
    addAttribute("selectors", Message(), "List of string that first atom must match in order to be output via the corresponding outlet.", false);
    addInlet("0");
    setDocumentation("The RouteNode routes messages to its different outlets according to the first string in each message.");
}

void RouteNode::processMessage(const char *inlet, const Message &message)
{
    Message ret = message;
    ret.prependMessage(getAttributeValue("value"));
    output("0", ret);
}

void RouteNode::onAttributeChanged(const char *name, const Message &value)
{
    std::vector<std::string> new_outlets;

    std::vector<std::string>::const_iterator iter;
    for (iter = selectors_.begin(); iter != selectors_.end(); selectors_ ++)
    {
        //TODO: removeOutlet((*iter).c_str());
    }
    unsigned int size = value.size();
    for (unsigned int i = 0; i < size; ++i)
    {
        if (value.indexMatchesType(i, 's'))
        {
            std::string s = value.getString(i);
            if (hasOutlet(s.c_str() && selectors_.find(s) != selectors_.end()))
            {
                std::cerr << "RouteNode::" << __FUNCTION__ << "(): Already got outlet with that name: " << s << std::endl;
            }
            else
                new_outlets.push_back(s);
        }
    }
    // remote outlets that should no longer be there:
    std::vector<std::string>::const_iterator iter;
    for (iter = selectors_.begin(); iter != selectors_.end(); iter ++)
    {
        if (new_outlets.find((*iter)) == new_outlets.end())
        {
            selectors_.erase(selectors_.find((*iter)));
            removeOutlet((*iter).c_str());
        }
    }
    // add outlets that should be there:
    for (iter = new_outlets.begin(); iter != new_outlets.end(); iter ++)
    {
        if (selectors_.find((*iter)) != selectors_.end())
        {
            selectors_.push_back((*iter));
            addOutlet((*iter).c_str(), "Output for the message starting with a string of the same name.");
        }
    }
}

} // end of namespace
} // end of namespace

