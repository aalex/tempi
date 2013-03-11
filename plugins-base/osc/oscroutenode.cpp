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
#include "plugins-base/osc/oscroutenode.h"
#include "tempi/utils.h"
#include "tempi/log.h"

namespace tempi {
namespace plugins_base {

// TODO: improve isValidPath
static bool isValidPath(const std::string &path)
{
    return path.size() >= 1 && path[0] == '/'; // FIXME
}

OscRouteNode::OscRouteNode() :
    Node()
{
    setShortDocumentation("Routes messages to its different outlets according to the beginning of the first string in each message.");
    setLongDocumentation("For example, one might route all the messages whose 0th string atom starts with \"/toon\". An outlet named the same way will be created, and the beginning of the 0th string of the messages output at this outlet will be stripped of the prefix we are looking for. In this case, a message starting with \"/toon/frame/add\" would output a message starting by \"/frame/add\" by the outlet named \"/toon\" for the [osc.route] node.");
    addAttribute(Attribute::ptr(new Attribute("paths", Message(), "List of string that first atom must match in order to be output via the corresponding outlet. Use only strings.", false)));
    if (Logger::isEnabledFor(DEBUG))
    {
        Logger::log(DEBUG, "[osc.route] constructor: paths = ()");
    }
    addInlet("incoming", "incoming messages");
}

static std::string stripBeginningOfString(const std::string &text, unsigned int position);

std::string stripBeginningOfString(const std::string &text, unsigned int position)
{
    if (position > text.size())
        return text;
    else
        return std::string(text.begin() + position, text.end());
}

void OscRouteNode::processMessage(const char *inlet, const Message &message)
{
    if (! utils::stringsMatch(inlet, "incoming"))
        return;
    if (message.getSize() == 0)
        return;
    if (! message.indexMatchesType(0, 's'))
    {
        if (Logger::isEnabledFor(DEBUG))
        {
            std::ostringstream os;
            os << "[osc.route] processMessage: First atom is not a string: " << message;
            Logger::log(DEBUG, os);
        }
        return;
    }
    std::string path = message.getString(0);
    Message ret = message.cloneRange(1, message.getSize() - 1);
    bool got_one = false;
    std::vector<std::string>::const_iterator iter;
    for (iter = paths_.begin(); iter != paths_.end(); ++iter)
    {
        if (utils::stringsMatch(path.c_str(), (*iter).c_str()))
        {
            got_one = true;
        }
        else if (utils::stringBeginsWith(path.c_str(), (*iter).c_str()))
        {
            // Remove prefix from path if it matches its beginning, or remove the whole atom if matches it completely.
            ret.prependString(stripBeginningOfString((*iter), (*iter).size()).c_str());
            if (Logger::isEnabledFor(DEBUG))
            {
                std::ostringstream os;
                os << "[osc.route] ";
                os << "Incoming path \"" << path << "\" begins with " << (*iter);
                os << " so we strip it from " << (*iter) << " hence resulting message is " << ret;
                Logger::log(DEBUG, os);
            }
            got_one = true;
        }
        if (got_one)
        {
            if (Logger::isEnabledFor(DEBUG))
            {
                std::ostringstream os;
                os << "[osc.route] ";
                os << " We got a match so we output result: " << ret << " through outlet: " << (*iter);
                Logger::log(DEBUG, os);
            }
            output((*iter).c_str(), ret);
            got_one = false;
        }
        else
        {
            if (Logger::isEnabledFor(DEBUG))
            {
                std::ostringstream os;
                os << "[osc.route] processMessage: Could not find match for message:  " << message;
                Logger::log(DEBUG, os);
            }
        }
    }
}

void OscRouteNode::printOutletsInfo() const
{
    if (Logger::isEnabledFor(DEBUG))
    {
        std::ostringstream os;
        os << "[osc.route]: paths it stores:";
        std::vector<std::string>::const_iterator iter;
        for (iter = paths_.begin(); iter != paths_.end(); ++iter)
            os << " " << (*iter);
        Logger::log(DEBUG, os);
    }
    if (Logger::isEnabledFor(DEBUG))
    {
        std::ostringstream os;
        os << "[osc.route]: actual outlets:";
        std::vector<std::string> outlets = this->listOutlets();
        std::vector<std::string>::const_iterator iter;
        for (iter = outlets.begin(); iter != outlets.end(); ++iter)
            os << " " << (*iter);
        Logger::log(DEBUG, os);
    }
}

bool OscRouteNode::onNodeAttributeChanged(const char *name, const Message &value)
{
    if (! utils::stringsMatch("paths", name))
        return true;
    if (Logger::isEnabledFor(DEBUG))
    {
        std::ostringstream os;
        os << "[osc.route] " << __FUNCTION__ << ": name=\"" << name << "\" value=" << value;
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
                utils::find_in_vector<std::string>(paths_, s))
            {
                if (Logger::isEnabledFor(DEBUG))
                {
                    std::ostringstream os;
                    os << "[osc.route] " << __FUNCTION__ << ": Already have selector named " << s;
                    Logger::log(DEBUG, os);
                }
                new_outlets_list.push_back(s);
            }
            else
            {
                if (isValidPath(s))
                {
                    if (Logger::isEnabledFor(DEBUG))
                    {
                        std::ostringstream os;
                        os << "[osc.route] " << __FUNCTION__ << ": new_outlets_list.push_back(" << s << ")";
                        Logger::log(DEBUG, os);
                    }
                    new_outlets_list.push_back(s);
                }
                else
                {
                    std::ostringstream os;
                    os << "[osc.route] " << __FUNCTION__ << ": Invalid OSC path " << s;
                    Logger::log(ERROR, os);
                }
            }
        }
    }
    // remote outlets that should no longer be there:
    std::vector<std::string>::const_iterator iter;
    for (iter = paths_.begin(); iter != paths_.end(); iter ++)
    {
        if (! utils::find_in_vector<std::string>(new_outlets_list, (*iter)))
        {
            if (Logger::isEnabledFor(DEBUG))
            {
                std::ostringstream os;
                os << "[osc.route]: remove outlet " << (*iter) << std::endl;
                Logger::log(DEBUG, os);
            }

            paths_.erase(std::find(paths_.begin(), paths_.end(), (*iter)));
            removeOutlet((*iter).c_str());
        }
    }
    // add outlets that should be there:
    for (iter = new_outlets_list.begin(); iter != new_outlets_list.end(); iter ++)
    {
        if (! utils::find_in_vector<std::string>(paths_, (*iter)))
        {
            if (Logger::isEnabledFor(DEBUG))
            {
                std::ostringstream os;
                os << "[route]: add outlet " << (*iter) << std::endl;
                Logger::log(DEBUG, os);
            }

            paths_.push_back((*iter));
            addOutlet((*iter).c_str(), "Output for the message whose 0th string starts the same.");
        }
    }
    this->printOutletsInfo();
    return true;
}

} // end of namespace
} // end of namespace

