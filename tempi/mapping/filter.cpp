/*
 * Copyright (C) 2011 Alexandre Quessy
 * 
 * This file is part of Tempi.
 * 
 * Tempi is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
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

#include "tempi/mapping/filter.h"
#include "tempi/utils.h"
#include <iostream>

namespace tempi {
namespace mapping {

Filter::Filter() :
    Node()
{
    addInlet("0");
    addOutlet("0");
}

void Filter::processMessage(const char *inlet, const Message &message)
{
    if (utils::stringsMatch(inlet, "0"))
    {
        getOutlet("0")->trigger(filter(message));
    }
}

} // end of namespace
} // end of namespace

