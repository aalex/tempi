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
    addSource(std::tr1::shared_ptr<Source>(new Source()));
    addSink(std::tr1::shared_ptr<Sink>(new Sink()));
}

Sink *Filter::getSink()
{
    return getSinks()[0].get();
}

Source *Filter::getSource()
{
    return getSources()[0].get();
}

void Filter::processTrigger(Source *source, boost::any data)
{
    // TODO
    // ?????
    getSource()->trigger(filter(source, data));
}

} // end of namespace

