/*
 * Copyright (C) 2011 Alexandre Quessy
 *
 * This file is part of Tempi.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software ither version 3 of the License, or
 * (at your option) any later version.
 * 
 * Tempi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with Tempi.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

#include "tempi/nodesignal.h"
#include <boost/bind.hpp>
#include <iostream>

namespace tempi {

NodeSignal::NodeSignal(
        const char *name,
        const char *short_documentation,
        const char *long_documentation,
        const char *type) :
    Documented(name, short_documentation, long_documentation),
    type_(type),
    type_strict_(true)
{}

NodeSignal::NodeSignal(
        const char *name, 
        const char *short_documentation,
        const char *long_documentation) :
    Documented(name, short_documentation, long_documentation),
    type_("*"),
    type_strict_(false)
{}

bool NodeSignal::isTypeStrict() const
{
    return type_strict_;
}

NodeSignal::~NodeSignal()
{
    signal_.disconnect_all_slots();
}

std::string NodeSignal::getType() const
{
    return type_;
}

NodeSignal::Signal &NodeSignal::getSignal()
{
    return signal_;
}

bool NodeSignal::trigger(const Message &message)
{
    if (isTypeStrict())
    {
        if (message.typesMatch(getType().c_str()))
        {
            signal_(message);
            return true;
        }
        else
        {
            std::cerr << "Bad type " << message.getTypes() << " for signal " << getName() << " while expected " << getType() << std::endl;
            return false;
        }
    }
    else
    {
        signal_(message);
        return true;
    }
}

} // end of namespace

