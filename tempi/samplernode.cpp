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

#include <boost/any.hpp>
#include <boost/bind.hpp>
#include "tempi/filter.h"
#include "tempi/sharedptr.h"
#include <iostream>

namespace tempi
{

Filter::Filter() :
    Node()
{
    addOutlet();
    addInlet();
}

Sink *Filter::getInlet()
{
    return getInlets()[0].get();
}

Source *Filter::getOutlet()
{
    return getOutlets()[0].get();
}

void Filter::processMessage(Source *source, boost::any data)
{
    getOutlet()->trigger(filter(data));
}

} // end of namespace

    App *app = (App *) user_data;
    try
    {
        //std::cout << __FUNCTION__ << std::endl;
        boost::any *any = app->player_.get()->readLoop();
        if (any)
        {
            tempi::_ff *value = boost::any_cast<tempi::_ff>(any);
            //std::cout << "Read " << value.get<0>() << ", " << value.get<1>() << std::endl;
            clutter_actor_set_position(app->rectangle_, value->get<0>(), value->get<1>());
        }
    }
    catch (const boost::bad_any_cast &e)
    {
        std::cout << "bad any cast exception" << std::endl;
        return;
    }
