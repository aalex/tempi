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

#include "tempi/config.h"
#ifdef HAVE_LIBMAPPER

#include "plugins-base/mapper/mapperinputnode.h"
#include "tempi/utils.h"
#include "tempi/log.h"
#include <iostream>

namespace tempi {
namespace plugins_base {

MapperInputNode::MapperInputNode() :
    Node(),
    device_("tempi", 9000)
{
    setShortDocumentation("Receives libmapper messages.");
    setLongDocumentation("");
    addOutlet("incoming");
    addOutlet("add_input");
    // TODO: allow to specify port and name
}

bool MapperInputNode::onNodeAttributeChanged(const char *name, const Message &value)
{
    // TODO
}


void MapperInputNode::processMessage(const char *inlet, const Message &message)
{
    if (utils::stringsMatch(inlet, "add_input"))
    {
        if (message.getTypes() == "sci") // name, type, length
        {
            if (message.getChar(1) == 'f')
            {
                device_.addFloatInput(message.getString(0).c_str(), message.getInt(2));
            }
            else if (message.getChar(1) == 'i')
            {
                device_.addIntInput(message.getString(0).c_str(), message.getInt(2));
            }
            else
            {
                std::cout << __FILE__ << ": Wrong type for libmapper input.\n";
            }
        }
        else
        {
            std::cout << __FILE__ << ": Wrong typetag for message.\n";
        }
    }
}

void MapperInputNode::doTick()
{
    std::vector<Message> messages;
    if (device_.poll(messages))
    {
        std::vector<Message>::iterator iter;
        for (iter = messages.begin(); iter != messages.end(); ++iter)
        {
            output("incoming", *iter);
        }
    }
}

} // end of namespace
} // end of namespace

#endif // HAVE_LIBMAPPER

