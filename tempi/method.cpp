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

#include "tempi/method.h"
#include <boost/bind.hpp>
#include <iostream>

namespace tempi {

Method::Method(
        const char *name,
        const char *short_documentation,
        const char *long_documentation,
        const char *type) :
    Documented(name, short_documentation, long_documentation),
    type_(type),
    type_strict_(true)
{}

Method::Method(
        const char *name, 
        const char *short_documentation,
        const char *long_documentation) :
    Documented(name, short_documentation, long_documentation),
    type_("*"),
    type_strict_(false)
{}

bool Method::isTypeStrict() const
{
    return type_strict_;
}

Method::~Method()
{
    signal_.disconnect_all_slots();
}

std::string Method::getType() const
{
    return type_;
}

Method::Signal &Method::getSignal()
{
    return signal_;
}

bool Method::call(const Message &message)
{
    if (isTypeStrict())
    {
        if (message.typesMatch(getType().c_str()))
        {
            return signal_(message);
            //return true;
        }
        else
        {
            std::cerr << "Bad type " << message.getTypes() << " for signal " << getName() << " while expected " << getType() << std::endl;
            return false;
        }
    }
    else
    {
        return signal_(message);
        //return true;
    }
}

} // end of namespace

