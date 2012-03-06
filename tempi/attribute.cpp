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

#include "tempi/attribute.h"
#include <iostream>

namespace tempi {

Attribute::Attribute(const char *name, const Message &value, const char *short_doc, bool type_strict) :
    Documented(name, short_doc, "")
{
    value_ = value;
    type_strict_ = type_strict;
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

bool Attribute::getMutable() const
{
    return mutable_;
}

void Attribute::setMutable(bool is_mutable)
{
    mutable_ = is_mutable;
}

} // end of namespace

