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

#include "tempi/namedobject.h"
#include "tempi/log.h"

namespace tempi {

const char * const NamedObject::DEFAULT_NAME = "default";

NamedObject::NamedObject(
    const char *name
    )
{
    static const std::string EMPTY;
    name_ = DEFAULT_NAME;
    if (EMPTY != name)
        this->setName(name);
}

std::string NamedObject::getName() const
{
    return name_;
}

bool NamedObject::setName(const char *name)
{
    if (! this->validator_.matches(name))
    {
        std::ostringstream os;
        os << "NamedObject::" << __FUNCTION__ << ": Name \"" << name << "\" is not valid. Leave unchanged.";
        Logger::log(ERROR, os);
        return false;
    }
    this->name_ = name;
    return true;
}

} // end of namespace

