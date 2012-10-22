/*
 * Copyright (C) 2011 Alexandre Quessy
 * Copyright (C) 2011 Michal Seta
 * Copyright (C) 2012 Nicolas Bouillot
 * Copyright (C) 2012 Emmanuel Durand
 *
 * This file is part of Tempi-plugins-base.
 *
 * This program is free software; you can redistither version 3 of the License, or
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
#include "plugins-base/flow/routenode.h"
#include "tempi/utils.h"
#include "tempi/log.h"

namespace tempi {
namespace plugins_base {

RouteNode::RouteNode() :
    Node()
{
    addAttribute(Attribute::ptr(new Attribute("selectors", Message(), "List of string that first atom must match in order to be output via the corresponding outlet.", false)));
    Logger::log(DEBUG, "[route] constructor: selectors = ()");
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
        if (Logger::isEnabledFor(WARNING))
        {
            std::ostringstream os;
            os << "[route] processMessage: First atom is not a string: " << message;
            Logger::log(WARNING, os);
        }
        return;
    }
    std::string selector = message.getString(0);
    Message ret = message.cloneRange(1, message.getSize() - 1);

    if (Logger::isEnabledFor(DEBUG))
    {
        std::ostringstream os;
        os << "[route] processMessage: result is : " << ret;
        Logger::log(DEBUG, os);
    }
    if (std::find(selectors_.begin(), selectors_.end(), selector) != selectors_.end())
        output(selector.c_str(), ret);
    else
    {
        if (Logger::isEnabledFor(WARNING))
        {
            std::ostringstream os;
            os << "[route] processMessage: No selector named like first atom:  " << message;
            Logger::log(WARNING, os.str().c_str());
        }
    }
}

void RouteNode::printOutletsInfo() const
{
    if (Logger::isEnabledFor(DEBUG))
    {
        std::ostringstream os;
        os << "[route]: selectors it stored:";
        std::vector<std::string>::const_iterator iter;
        for (iter = selectors_.begin(); iter != selectors_.end(); ++iter)
            os << " " << (*iter);
        Logger::log(DEBUG, os.str().c_str());
    }
    if (Logger::isEnabledFor(DEBUG))
    {
        std::ostringstream os;
        os << "[route]: actual outlets:";
        std::vector<std::string> outlets = this->listOutlets();
        std::vector<std::string>::const_iterator iter;
        for (iter = outlets.begin(); iter != outlets.end(); ++iter)
            os << " " << (*iter);
        Logger::log(DEBUG, os.str().c_str());
    }
}

bool RouteNode::onNodeAttributeChanged(const char *name, const Message &value)
{
    if (! utils::stringsMatch("selectors", name))
        return true;
    if (Logger::isEnabledFor(DEBUG))
    {
        std::ostringstream os;
        os << "[route] " << __FUNCTION__ << ": name=\"" << name << "\" value=" << value;
        Logger::log(DEBUG, os.str().c_str());
    }
    this->printOutletsInfo();

    std::vector<std::string> new_outlets_list;
    unsigned int size = value.getSize();
    for (unsigned int i = 0; i < size; ++i)
    {
        if (value.indexMatchesType(i, 's'))
        {
            std::string s = value.getString(i);
            if (hasOutlet(s.c_str()) && 
                utils::find_in_vector<std::string>(selectors_, s))
            {
                if (Logger::isEnabledFor(DEBUG))
                {
                    std::ostringstream os;
                    os << "[route] " << __FUNCTION__ << ": Already have selector named " << s;
                    Logger::log(DEBUG, os.str().c_str());
                }
                new_outlets_list.push_back(s);
            }
            else
            {
                if (Logger::isEnabledFor(DEBUG))
                {
                    std::ostringstream os;
                    os << "[route] " << __FUNCTION__ << ": new_outlets_list.push_back(" << s << ")";
                    Logger::log(DEBUG, os.str().c_str());
                }
                new_outlets_list.push_back(s);
            }
        }
    }
    // remote outlets that should no longer be there:
    std::vector<std::string>::const_iterator iter;
    for (iter = selectors_.begin(); iter != selectors_.end(); iter ++)
    {
        if (! utils::find_in_vector<std::string>(new_outlets_list, (*iter)))
        {
            if (Logger::isEnabledFor(DEBUG))
            {
                std::ostringstream os;
                os << "[route]: remove outlet " << (*iter) << std::endl;
                Logger::log(DEBUG, os.str().c_str());
            }

            selectors_.erase(std::find(selectors_.begin(), selectors_.end(), (*iter)));
            removeOutlet((*iter).c_str());
        }
    }
    // add outlets that should be there:
    for (iter = new_outlets_list.begin(); iter != new_outlets_list.end(); iter ++)
    {
        if (! utils::find_in_vector<std::string>(selectors_, (*iter)))
        {
            if (Logger::isEnabledFor(DEBUG))
            {
                std::ostringstream os;
                os << "[route]: add outlet " << (*iter) << std::endl;
                Logger::log(DEBUG, os.str().c_str());
            }

            selectors_.push_back((*iter));
            addOutlet((*iter).c_str(), "Output for the message starting with a string of the same name.");
        }
    }
    this->printOutletsInfo();
    return true;
}

} // end of namespace
} // end of namespace

