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
#include "plugins-base/string/stringsplitnode.h"
#include "tempi/utils.h"
#include "tempi/log.h"

namespace tempi {
namespace plugins_base {

const char * const StringSplitNode::STRING_INLET = "string";
const char * const StringSplitNode::CHARACTERS_OUTLET = "chars";

StringSplitNode::StringSplitNode() :
    Node()
{
    this->setShortDocumentation("Splits a string into its characters.");
    this->addInlet(STRING_INLET, "String to split.");
    this->addOutlet(CHARACTERS_OUTLET, "Message containing all the characters of the strings, as chars.");
}

void StringSplitNode::processMessage(const char *inlet, const Message &message)
{
    if (! utils::stringsMatch(inlet, STRING_INLET))
        return;
    if (! message.typesMatch("s"))
    {
        std::ostringstream os;
        os << "StringSplitNode." << __FUNCTION__ << ": Expected a single string as incoming message but got " <<
            message;
        Logger::log(WARNING, os);
        return;
    }
    Message ret;
    std::string s = message.getString(0);
    for (unsigned int i = 0; i < s.size(); ++i)
    {
        ret.appendChar(s[i]);
    }
    this->output(CHARACTERS_OUTLET, ret);
}

} // end of namespace
} // end of namespace

