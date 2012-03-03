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

#include "plugins-base/abstraction/outletnode.h"
#include "tempi/utils.h"
#include "tempi/log.h"
#include <iostream>

namespace tempi {
namespace plugins_base {

OutletNode::OutletNode() :
    Node()
{
    setShortDocumentation("Outlet in an abstraction.");
    addInlet("incoming");
}

void OutletNode::flush(std::vector<Message> &messages)
{
    bool got_some = true;
    while (got_some)
    {
        Message message;
        got_some = queue_.try_pop(message);
        if (got_some)
            messages.push_back(message);
    }
}

void OutletNode::processMessage(const char *inlet, const Message &message)
{
    if (utils::stringsMatch(inlet, "incoming"))
        queue_.push(message);
}

} // end of namespace
} // end of namespace

