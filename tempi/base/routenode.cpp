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
#include "tempi/utils.h"

namespace tempi {
namespace base {

RouteNode::RouteNode() :
    Node()
{
    addAttribute(Attribute::ptr(new Attribute("selectors", Message(), "List of string that first atom must match in order to be output via the corresponding outlet.", false)));
    addInlet("0");
    setShortDocumentation("The RouteNode routes messages to its different outlets according to the first string in each message.");
}

void RouteNode::processMessage(const char *inlet, const Message &message)
{
    if (! utils::stringsMatch(inlet, "0"))
        return;
    if (message.getSize() == 0)
        return;
    if (! message.indexMatchesType(0, 's'))
    {
        std::cerr << "RouteNode: " << "First atom must be a string!";
        return;
    }
    std::string selector = message.getString(0);
    Message ret = message.cloneRange(1, message.getSize() - 1);
    if (std::find(selectors_.begin(), selectors_.end(), selector) != selectors_.end())
        output(selector.c_str(), ret);
    else
    {
        std::cerr << "RouteNode: No selector named " << selector << std::endl;
    }
}

// static void print_outlets(const std::map<std::string, Outlet::ptr> &outlets)
// {
//     std::cout << "Outlets:\n";
//     std::map<std::string, Outlet::ptr>::const_iterator iter;
//     for (iter = outlets.begin(); iter != outlets.end(); ++iter)
//     {
//         std::cout << " * " << (*iter).first << std::endl;
//     }
// }

void RouteNode::onAttributeChanged(const char *name, const Message &value)
{
    if (! utils::stringsMatch("selectors", name))
        return;
    std::vector<std::string> new_outlets;
    unsigned int size = value.getSize();
    for (unsigned int i = 0; i < size; ++i)
    {
        if (value.indexMatchesType(i, 's'))
        {
            std::string s = value.getString(i);
            if (hasOutlet(s.c_str()) && 
                utils::find_in_vector<std::string>(selectors_, s))
            {
                std::cerr << "RouteNode::" << __FUNCTION__ << "(): Already got outlet with that name: " << s << std::endl;
            }
            else
            {
                //std::cout << "[route] add outlet " << s << " to new_outlets" << std::endl;
                new_outlets.push_back(s);
            }
        }
    }
    // remote outlets that should no longer be there:
    std::vector<std::string>::const_iterator iter;
    for (iter = selectors_.begin(); iter != selectors_.end(); iter ++)
    {
        if (! utils::find_in_vector<std::string>(new_outlets, (*iter)))
        {
            selectors_.erase(std::find(selectors_.begin(), selectors_.end(), (*iter)));
            removeOutlet((*iter).c_str());
            //std::cout << "[route]: rm outlet " << (*iter) << std::endl;
        }
    }
    // add outlets that should be there:
    for (iter = new_outlets.begin(); iter != new_outlets.end(); iter ++)
    {
        if (! utils::find_in_vector<std::string>(selectors_, (*iter)))
        {
            selectors_.push_back((*iter));
            std::cout << "[route]: add outlet " << (*iter) << std::endl;
            addOutlet((*iter).c_str(), "Output for the message starting with a string of the same name.");
        }
    }
    //print_outlets(getOutlets());
}

} // end of namespace
} // end of namespace

