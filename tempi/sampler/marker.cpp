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

#include "tempi/sampler/marker.h"

namespace tempi {
namespace sampler {

Marker::Marker(Identifier id) :
    id_(id)
{
    // pass
}

Identifier Marker::getIdentifier() const
{
    return id_;
}

const std::string &Marker::getName() const
{
    return name_;
}


void Marker::setName(const std::string &name)
{
    name_ = name;
}

} // end of namespace
} // end of namespace

