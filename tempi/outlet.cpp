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

#include "tempi/outlet.h"
#include <iostream>

namespace tempi
{

Outlet::Outlet(const char *name, const char *documentation)
{
    name_ = std::string(name);
    documentation_ = std::string(documentation);
}

void Outlet::trigger(const Message &message)
{
    //std::cout << __FUNCTION__ << std::endl;
    on_triggered_signal_(message);
}

std::string Outlet::getName() const
{
    return name_;
}

std::string Outlet::getDocumentation() const
{
    return documentation_;
}

} // end of namespace

