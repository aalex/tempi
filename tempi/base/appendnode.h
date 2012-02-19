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

/**
 * @file
 * The AppendNode class.
 */

#ifndef __TEMPI_BASE_APPENDNODE_H__
#define __TEMPI_BASE_APPENDNODE_H__

#include <iostream>
#include "tempi/utils.h"

namespace tempi {
namespace base {

/**
 * The AppendNode appends some atoms to a message.
 */
class AppendNode : public Node
{
    public:
        AppendNode() :
            Node()
        {
            setShortDocumentation("Appends some atoms to a message");
            addAttribute(Attribute::ptr(new Attribute("value", Message(), "Holds any message to append.", false)));
            addOutlet("0", "Resulting messages with appended atoms.");
            addInlet("0", "Incoming messages.");
        }
    private:
        virtual void processMessage(const char *inlet, const Message &message)
        {
            if (! utils::stringsMatch(inlet, "0"))
                return;
            Message ret = getAttributeValue("value");
            if (message.getTypes() != "")
                ret.prependMessage(message);
            output("0", ret);
        }
};


} // end of namespace
} // end of namespace

#endif // ifndef
