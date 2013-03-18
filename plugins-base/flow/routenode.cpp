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
#include <boost/lexical_cast.hpp>
#include "tempi/utils.h"
#include "tempi/log.h"

namespace tempi {
namespace plugins_base {

const char * const RouteNode::INPUT_INLET = "0";
const char * const RouteNode::SELECTORS_ATTR = "selectors";

RouteNode::RouteNode() :
    Node()
{
    addAttribute(Attribute::ptr(new Attribute(SELECTORS_ATTR, Message(), "List of string that first atom must match in order to be output via the corresponding outlet.", false)));
    if (Logger::isEnabledFor(DEBUG))
    {
        Logger::log(DEBUG, "[RouteNode] constructor: selectors = ()");
    }
    addInlet(INPUT_INLET);
    setShortDocumentation("The RouteNode routes messages to its different outlets according to the first string in each message.");
}

void RouteNode::processMessage(const char *inlet, const Message &message)
{
    static const unsigned int selector_index = 0;
    std::string selector;

    if (! utils::stringsMatch(inlet, INPUT_INLET))
        return;
    if (message.getSize() < selector_index)
    {
        std::ostringstream os;
        os << "[RouteNode] processMessage: (node \"" << this->getName() << "\") Not enough atoms in message: " << message;
        Logger::log(ERROR, os);
        return;
    }

    if (message.indexMatchesType(selector_index, 's'))
    {
        selector = message.getString(selector_index);
    }
    else if (message.indexMatchesType(selector_index, 'i'))
    {
        selector = boost::lexical_cast<std::string>(message.getInt(selector_index));
    }
    else if (message.indexMatchesType(selector_index, 'f'))
    {
        selector = boost::lexical_cast<std::string>(message.getFloat(selector_index));
    }
    else
    {
        if (Logger::isEnabledFor(WARNING))
        {
            std::ostringstream os;
            os << "[RouteNode] processMessage: (node \"" << this->getName() << "\") First atom is not the right type: " << message;
            Logger::log(WARNING, os);
        }
        return;
    }
    Message ret = message.cloneRange(selector_index + 1, message.getSize() - (selector_index + 1));

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
        if (Logger::isEnabledFor(NOTICE))
        {
            std::ostringstream os;
            os << "[RouteNode] processMessage: No selector named " << selector;
            Logger::log(NOTICE, os);
        }
    }
}

void RouteNode::printOutletsInfo() const
{
    if (Logger::isEnabledFor(DEBUG))
    {
        std::ostringstream os;
        os << "[RouteNode]: selectors it stored:";
        std::vector<std::string>::const_iterator iter;
        for (iter = selectors_.begin(); iter != selectors_.end(); ++iter)
            os << " " << (*iter);
        Logger::log(DEBUG, os);
    }
    if (Logger::isEnabledFor(DEBUG))
    {
        std::ostringstream os;
        os << "[RouteNode]: actual outlets:";
        std::vector<std::string> outlets = this->listOutlets();
        std::vector<std::string>::const_iterator iter;
        for (iter = outlets.begin(); iter != outlets.end(); ++iter)
            os << " " << (*iter);
        Logger::log(DEBUG, os);
    }
}

bool RouteNode::onNodeAttributeChanged(const char *name, const Message &value)
{
    if (! utils::stringsMatch("selectors", name))
        return true;
    if (Logger::isEnabledFor(DEBUG))
    {
        std::ostringstream os;
        os << "[RouteNode] " << __FUNCTION__ << ": name=\"" << name << "\" value=" << value;
        Logger::log(DEBUG, os);
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
                    os << "[RouteNode] " << __FUNCTION__ << ": Already have selector named " << s;
                    Logger::log(DEBUG, os);
                }
                new_outlets_list.push_back(s);
            }
            else
            {
                if (Logger::isEnabledFor(DEBUG))
                {
                    std::ostringstream os;
                    os << "[RouteNode] " << __FUNCTION__ << ": new_outlets_list.push_back(" << s << ")";
                    Logger::log(DEBUG, os);
                }
                new_outlets_list.push_back(s);
            }
        }
    }
    // remove outlets that should no longer be there:
    std::vector<std::string>::const_iterator iter;
    std::vector<std::string> to_delete;
    for (iter = selectors_.begin(); iter != selectors_.end(); iter ++)
    {
        std::string outlet_name = (*iter);
        if (! utils::find_in_vector<std::string>(new_outlets_list, outlet_name))
        {
            if (Logger::isEnabledFor(DEBUG))
            {
                std::ostringstream os;
                os << "[RouteNode]: remove outlet \"" << outlet_name << "\"";
                Logger::log(DEBUG, os);
            }
            to_delete.push_back(outlet_name);
            this->removeOutlet(outlet_name.c_str());
        }
    }

    for (iter = to_delete.begin(); iter != to_delete.end(); iter++)
    {
        selectors_.erase(std::find(selectors_.begin(), selectors_.end(), (*iter)));
    }
    // add outlets that should be there:
    for (iter = new_outlets_list.begin(); iter != new_outlets_list.end(); iter ++)
    {
        if (! utils::find_in_vector<std::string>(selectors_, (*iter)))
        {
            if (Logger::isEnabledFor(DEBUG))
            {
                std::ostringstream os;
                os << "[RouteNode]: add outlet " << (*iter);
                Logger::log(DEBUG, os);
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

