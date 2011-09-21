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

#include "tempi/filter.h"
#include <iostream>

namespace tempi
{

Filter::Filter() :
    Node()
{
    addOutlet(std::tr1::shared_ptr<Source>(new Source()));
    addInlet(std::tr1::shared_ptr<Sink>(new Sink()));
}

Sink *Filter::getInlet()
{
    return getInlets()[0].get();
}

Source *Filter::getOutlet()
{
    return getOutlets()[0].get();
}

void Filter::processMessage(const Message &message)
{
    getOutlet()->trigger(filter(message));
}

} // end of namespace

