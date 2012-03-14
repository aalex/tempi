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
#include "plugins-base/string/stringjoinnode.h"
#include "tempi/utils.h"
#include "tempi/log.h"

namespace tempi {
namespace plugins_base {

const char * const StringJoinNode::ATOMS_INLET = "atoms";
const char * const StringJoinNode::STRING_OUTLET = "string";
const char * const StringJoinNode::SEPARATOR_ATTR = "separator";

StringJoinNode::StringJoinNode() :
    Node()
{
    this->setShortDocumentation("Joins atoms to create a string.");
    this->addAttribute(Attribute::ptr(new Attribute(SEPARATOR_ATTR,
        Message("s", ""), "Separator between elements.")));
    this->addInlet(ATOMS_INLET, "Atoms to join.");
    this->addOutlet(STRING_OUTLET, "Resulting text.");
}

void StringJoinNode::processMessage(const char *inlet, const Message &message)
{
    if (! utils::stringsMatch(inlet, ATOMS_INLET))
        return;
    unsigned int size = message.getSize();
    if (size == 0)
    {
        this->output(STRING_OUTLET, Message("s", ""));
        return;
    }
    std::string result;
    std::string separator = this->getAttributeValue(SEPARATOR_ATTR).getString(0);
    for (unsigned int i = 0; i < size; ++i)
    {
        try
        {
            result += utils::argumentToString(message, i);
        }
        catch (const BadIndexException &e)
        {
            std::ostringstream os;
            os << "StringJoinNode." << __FUNCTION__ << ": " << e.what();
            Logger::log(ERROR, os);
            return;
        }
        catch (const BadAtomTypeException &e)
        {
            std::ostringstream os;
            os << "StringJoinNode." << __FUNCTION__ << ": " << e.what();
            Logger::log(ERROR, os);
            return;
        }
        if (i != (size - 1)) // if not last atom
        {
            result += separator;
        }
    }
    this->output(STRING_OUTLET, Message("s", result.c_str()));
}

} // end of namespace
} // end of namespace

