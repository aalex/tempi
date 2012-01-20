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

#include "tempi/nodesignal.h"
#include <boost/bind.hpp>
#include <iostream>

namespace tempi
{

NodeSignal::NodeSignal(const char *name, const char *doc, const char *type) :
    name_(name),
    doc_(doc),
    type_(type),
    type_strict_(true)
{}

NodeSignal::NodeSignal(const char *name, const char *doc) :
    name_(name),
    doc_(doc),
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

std::string NodeSignal::getName() const
{
    return name_;
}

std::string NodeSignal::getDocumentation() const
{
    return doc_;
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

