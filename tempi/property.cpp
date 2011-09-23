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

#include "tempi/property.h"
#include <iostream>
#include <sstream>

namespace tempi
{

namespace types
{

std::string getPropertyTypeForAny(const boost::any &value)
{
    if (value.type() == typeid(_f))
        return "f";
    if (value.type() == typeid(_ff))
        return "ff";
    if (value.type() == typeid(_fff))
        return "fff";
    if (value.type() == typeid(_ffff))
        return "ffff";
    else
    {
        std::ostringstream os;
        os << __FUNCTION__ << ": Type not supported: " << value.type().name();
        throw BadArgumentTypeException(os.str().c_str());
    }
}

} // end of namespace


Property::Property(const char *name, boost::any value, const char *description) :
    name_(name),
    value_(value),
    description_(description)
{}

std::string Property::getTypeName() const
{
    return types::getPropertyTypeForAny(value_);
}

const std::type_info &Property::getType() const
{
    return value_.type();
}

bool Property::typeMatches(const char *type) const
{
    return getTypeName() == type;
}

const std::string &Property::getName() const
{
    return name_;
}

const std::string &Property::getDescription() const
{
    return description_;
}

Property::OnChanged &Property::getOnChangedSignal()
{
    return value_changed_signal_;
}

} // end of namespace

