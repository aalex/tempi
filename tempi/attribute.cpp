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

#include "tempi/attribute.h"
#include <iostream>

namespace tempi {

Attribute::Attribute(const char *name, const Message &value, const char *doc, bool type_strict)
{
    value_ = value;
    name_ = std::string(name);
    documentation_ = std::string(documentation_);
    type_strict_ = type_strict;
}

std::string Attribute::getName() const
{
    return name_;
}

std::string Attribute::getDocumentation() const
{
    return documentation_;
}

const Message &Attribute::getValue()
{
    return value_;
}

bool Attribute::setValue(const Message &value)
{
    // TODO: check here if same type or not
    value_ = value;
    return true;
}

bool Attribute::isTypeStrict() const
{
    return type_strict_;
}

} // end of namespace

