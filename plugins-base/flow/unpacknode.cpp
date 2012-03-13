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
#include "plugins-base/flow/unpacknode.h"
#include "tempi/utils.h"
#include "tempi/log.h"

namespace tempi {
namespace plugins_base {

const char * const UnpackNode::INPUT_INLET = "input";
const char * const UnpackNode::TYPES_ATTR = "types";

UnpackNode::UnpackNode() :
    Node(),
    current_number_of_outlets_(0)
{
    addAttribute(Attribute::ptr(new Attribute(TYPES_ATTR, Message(), "List of atom types. An outlet is created for each of these. Outlets are numbers from 0 to n-1", false)));
    Logger::log(DEBUG, "[unpack] constructor: types = (s:\"\")");
    addInlet(INPUT_INLET, "Incoming messages go here. Their type should match the expected type tags.");
    setShortDocumentation("Splits messages into atoms.");
}

void UnpackNode::processMessage(const char *inlet, const Message &message)
{
    if (! utils::stringsMatch(inlet, INPUT_INLET))
        return;
    if (message.getSize() == 0)
        return;
    std::string expected = this->getAttributeValue(TYPES_ATTR).getString(0);
    if (! message.typesMatch(expected.c_str()))
    {
        std::ostringstream os;
        os << "[unpack] processMessage: type tags dont match expected " << expected << ": " << message;
        Logger::log(WARNING, os);
        return;
    }
    for (unsigned int i = message.getSize() - 1; i >= 0; --i)
    {
        // output last atom first. (to last outlet)
        Message ret = message.cloneRange(i, i);
        {
            std::ostringstream os;
            os << "[unpack] " << __FUNCTION__ << ": output " << ret << " to " << i << "th outlet.";
            Logger::log(INFO, os);
        }
        std::ostringstream name_os;
        name_os << i;
        output(name_os.str().c_str(), ret);
        if (i == 0)
            break; // IMPORTANT!
    }
}

void UnpackNode::printOutletsInfo() const
{
    {
        std::ostringstream os;
        os << "[unpack]: actual outlets:";
        std::vector<std::string> outlets = this->listOutlets();
        std::vector<std::string>::const_iterator iter;
        for (iter = outlets.begin(); iter != outlets.end(); ++iter)
            os << " " << (*iter);
        Logger::log(DEBUG, os.str().c_str());
    }
}

bool UnpackNode::onNodeAttributeChanged(const char *name, const Message &value)
{
    if (! utils::stringsMatch(TYPES_ATTR, name))
        return true; // true means it's OK to change that attribute
    {
        std::ostringstream os;
        os << "[unpack] " << __FUNCTION__ << ": name=\"" << name << "\" value=" << value;
        Logger::log(DEBUG, os);
    }
    this->printOutletsInfo();

    std::string typeTags = value.getString(0);
    if (! utils::isValidType(typeTags.c_str()))
    {
        std::ostringstream os;
        os << "[unpack] " << __FUNCTION__ << ": Invalid type tags: " << typeTags;
        Logger::log(ERROR, os);
        return false;
    }
    unsigned int size = typeTags.size();
    if (size > current_number_of_outlets_)
    {
        for (unsigned int i = current_number_of_outlets_; i < size; ++i)
        {
            std::ostringstream name_os;
            name_os << i;
            std::ostringstream doc;
            doc << "Output for the " << i << "th atom";
            {
                std::ostringstream os;
                os << "[unpack] " << __FUNCTION__ << ": add outlet " << i;
                Logger::log(INFO, os);
            }
            addOutlet(name_os.str().c_str(), doc.str().c_str());
        }
    }
    else if (size < current_number_of_outlets_)
    {
        for (unsigned int i = size; i < current_number_of_outlets_; ++i)
        {
            std::ostringstream name_os;
            name_os << i;
            {
                std::ostringstream os;
                os << "[unpack] " << __FUNCTION__ << ": remove outlet " << i;
                Logger::log(INFO, os);
            }
            removeOutlet(name_os.str().c_str());
        }
    }
    current_number_of_outlets_ = size;
    this->printOutletsInfo();
    return true;
}

} // end of namespace
} // end of namespace

