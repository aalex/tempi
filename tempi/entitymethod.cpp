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

#include "tempi/entitysignal.h"
#include "tempi/entitymethod.h"
#include "tempi/log.h"
#include <sstream>

namespace tempi {

EntityMethod::EntityMethod(
        const char *name,
        const char *short_documentation,
        const char *long_documentation,
        const char *arguments_type,
        const char *return_type) :
    Documented(name, short_documentation, long_documentation),
    arguments_type_(arguments_type),
    return_type_(return_type)
{}

EntityMethod::~EntityMethod()
{
}

std::string EntityMethod::getArgumentsType() const
{
    return arguments_type_;
}

std::string EntityMethod::getReturnType() const
{
    return return_type_;
}

bool EntityMethod::call(const Message &arguments, Message & return_value)
{
    if (this->getArgumentsType() != "*")
    {
        if (! arguments.typesMatch(this->getArgumentsType().c_str()))
        {
            std::ostringstream os;
            os << "Bad type " << arguments.getTypes() << " for method " << this->getName() <<
                " while expected " << this->getArgumentsType() << std::endl;
            Logger::log(ERROR, os);
            return false;
        }
    }
    return this->doCall(arguments, return_value);
}

} // end of namespace

