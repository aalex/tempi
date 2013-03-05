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
#include "plugins-base/flow/logger_node.h"
#include "tempi/utils.h"
#include "tempi/log.h"

namespace tempi {
namespace plugins_base {

const char * const LoggerNode::LEVEL_ATTR = "level";
const char * const LoggerNode::LOG_INLET = "in";

// FIXME: duplicate of Wrapper::setLogLevel
static bool stringToLogLevel(const std::string &level_str, LogLevel &level)
{
    if (utils::stringsMatch(level_str.c_str(), "DEBUG"))
        level = DEBUG;
    else if (utils::stringsMatch(level_str.c_str(), "INFO"))
        level = INFO;
    else if (utils::stringsMatch(level_str.c_str(), "NOTICE"))
        level = NOTICE;
    else if (utils::stringsMatch(level_str.c_str(), "WARNING"))
        level = WARNING;
    else if (utils::stringsMatch(level_str.c_str(), "CRITICAL"))
        level = CRITICAL;
    else if (utils::stringsMatch(level_str.c_str(), "ERROR"))
        level = ERROR;
    else
    {
        std::ostringstream os;
        os << __FUNCTION__ <<
            "Invalid log level: " << level_str;
        Logger::log(ERROR, os);
        return false;
    }
    return true;
}

LoggerNode::LoggerNode() :
    Node()
{
    setShortDocumentation("Sends incoming messages to Tempi's logging system.");
    addInlet(LOG_INLET, "Logs messages.");
    addAttribute(Attribute::ptr(new Attribute(LEVEL_ATTR, Message("s", "NOTICE"), "Log level. Can be CRITICAL, ERROR, WARNING, NOTICE, INFO, DEBUG")));
}

void LoggerNode::processMessage(const char *inlet, const Message &message)
{
    if (utils::stringsMatch(inlet, LOG_INLET))
    {
        std::string level_str = this->getAttributeValue(LEVEL_ATTR).getString(0);
        LogLevel level;
        bool ok = stringToLogLevel(level_str, level);
        if (ok)
        {
            std::ostringstream os;
            os << message;
            Logger::log(level, os);
        }
        else
        {
            std::ostringstream os;
            os << "LoggerNode." << __FUNCTION__ << ": " <<
            "Invalid log level";
            Logger::log(ERROR, os);
        }
    }
}

bool LoggerNode::onNodeAttributeChanged(const char *name, const Message &value)
{
    if (utils::stringsMatch(name, LEVEL_ATTR))
    {
        LogLevel level;
        std::string level_str = value.getString(0);
        bool ok = stringToLogLevel(level_str, level);
        if (ok)
        {
            return true;
        }
        else
        {
            std::ostringstream os;
            os << "LoggerNode." << __FUNCTION__ << ": " <<
            "Invalid log level";
            Logger::log(ERROR, os);
            return false;
        }
    }
    return true;
}

} // end of namespace
} // end of namespace

