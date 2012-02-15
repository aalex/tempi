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
#include "tempi/base/prependnode.h"
#include "tempi/utils.h"

namespace tempi {
namespace base {

PrependNode::PrependNode() :
    Node()
{
    setShortDocumentation("Prepends some atoms to a message");
    addAttribute(Attribute::ptr(new Attribute("value", Message(), "Holds any message to prepend.", false)));
    addOutlet("0", "Resulting messages with prepent atoms.");
    addInlet("0", "Incoming messages.");
}

void PrependNode::processMessage(const char *inlet, const Message &message)
{
    if (! utils::stringsMatch(inlet, "0"))
        return;
    Message ret = message;
    //std::cout << "[prepend]: " << getAttributeValue("value") << " " << message << std::endl;
    if (getAttributeValue("value").getTypes() != "")
    {
        ret.prependMessage(getAttributeValue("value"));
    }
    //std::cout << "[prepend]: " << " result is " << ret << std::endl;
    output("0", ret);
}

} // end of namespace
} // end of namespace

