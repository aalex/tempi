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
#include "plugins-base/flow/triggernode.h"
#include "tempi/utils.h"
#include "tempi/log.h"

namespace tempi {
namespace plugins_base {

const char * const TriggerNode::INPUT_INLET = "input";
const char * const TriggerNode::TYPES_ATTR = "types";

TriggerNode::TriggerNode() :
    Node()
{
    addAttribute(Attribute::ptr(new Attribute(TYPES_ATTR, Message("s", ""), "List of types to output. \"a\" means anythings, which forwards the incoming message. \"b\": is for bang: an empty message.")));
    Logger::log(DEBUG, "[trigger] constructor: types = (s:\"\")");
    addInlet(INPUT_INLET, "Incoming messages go here.");
    setShortDocumentation("Sequences messages in right-to-left order and converts messages to empty one, if needed.");
}

void TriggerNode::processMessage(const char *inlet, const Message &message)
{
    if (! utils::stringsMatch(inlet, INPUT_INLET))
        return;
    std::string types = this->getAttributeValue(TYPES_ATTR).getString(0);
    if (Logger::isEnabledFor(INFO))
    {
        std::ostringstream os;
        os << "[trigger]: current types are " << types;
        Logger::log(INFO, os);
    }
    Message empty;
    for (int i = (int) types.size() - 1; i >= 0; --i)
    {
        // output last atom first. (to last outlet)
        bool is_bang = types[(unsigned int) i] == 'b';
        if (Logger::isEnabledFor(INFO))
        {
            std::ostringstream os;
            os << "[trigger] " << __FUNCTION__ << ": output ";
            if (is_bang)
                os << empty;
            else
                os << message;
            os << " to " << i << "th outlet.";
            Logger::log(INFO, os);
        }
        std::ostringstream name_os;
        name_os << i;
        if (is_bang)
            output(name_os.str().c_str(), empty);
        else
            output(name_os.str().c_str(), message);
    }
}

void TriggerNode::printOutletsInfo() const
{
    if (Logger::isEnabledFor(DEBUG))
    {
        std::ostringstream os;
        os << "[trigger]: actual outlets:";
        std::vector<std::string> outlets = this->listOutlets();
        std::vector<std::string>::const_iterator iter;
        for (iter = outlets.begin(); iter != outlets.end(); ++iter)
            os << " " << (*iter);
        Logger::log(DEBUG, os);
    }
}

bool TriggerNode::onNodeAttributeChanged(const char *name, const Message &value)
{
    std::string types = value.getString(0);
    int new_size = (int) types.size();
    for (int i = 0; i < new_size; ++i)
    {
        char c = types[(unsigned int) i];
        if (c != 'b' && c != 'a')
        {
            std::ostringstream os;
            os << "[trigger] " << __FUNCTION__ << ": Invalid types: " << value;
            Logger::log(ERROR, os);
            return false;
        }
    }
    this->printOutletsInfo();

    int current_number_of_outlets = (int) this->getAttributeValue(TYPES_ATTR).getString(0).size();

    if (new_size > current_number_of_outlets)
    {
        for (int i = current_number_of_outlets; i < new_size; ++i)
        {
            std::ostringstream name_os;
            name_os << i;
            std::ostringstream doc;
            doc << "Output number " << i;
            if (Logger::isEnabledFor(INFO))
            {
                std::ostringstream os;
                os << "[trigger] " << __FUNCTION__ << ": add outlet " << i;
                Logger::log(INFO, os);
            }
            addOutlet(name_os.str().c_str(), doc.str().c_str());
        }
    }
    else if (new_size < current_number_of_outlets)
    {
        for (int i = new_size; i < current_number_of_outlets; ++i)
        {
            std::ostringstream name_os;
            name_os << i;
            if (Logger::isEnabledFor(INFO))
            {
                std::ostringstream os;
                os << "[trigger] " << __FUNCTION__ << ": remove outlet " << i;
                Logger::log(INFO, os);
            }
            removeOutlet(name_os.str().c_str());
        }
    }
    this->printOutletsInfo();
    return true;
}

} // end of namespace
} // end of namespace

