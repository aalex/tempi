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

#include <boost/bind.hpp>
#include "tempi/node.h"
#include "tempi/sink.h"
#include "tempi/sharedptr.h"
#include <iostream>

namespace tempi
{

Node::Node()
{
    // pass
}

std::vector<std::tr1::shared_ptr<Source> > Node::getOutlets()
{
    return outlets_;
}

void Node::onInletTriggered(Source *source, boost::any data)
{
    // TODO
    std::cout << __FUNCTION__ << std::endl;
    processMessage(source, data);
}

std::vector<std::tr1::shared_ptr<Sink> > Node::getInlets()
{
    return inlets_;
}

bool Node::addOutlet(std::tr1::shared_ptr<Source> source)
{
    if (! hasOutlet(source.get()))
    {
        outlets_.push_back(source);
        return true;
    }
    return false;
}

bool Node::addInlet(std::tr1::shared_ptr<Sink> sink)
{
    if (! hasInlet(sink.get()))
    {
        inlets_.push_back(sink);
        sink.get()->on_triggered_signal_.connect(boost::bind(&Node::onInletTriggered, this, _1, _2));
        return true;
    }
    return false;
}

bool Node::addInlet()
{
    addInlet(std::tr1::shared_ptr<Sink>(new Sink()));
}
bool Node::addOutlet()
{
    addOutlet(std::tr1::shared_ptr<Source>(new Source()));
}

bool Node::hasInlet(Sink *sink)
{
    typename std::vector<std::tr1::shared_ptr<Sink> >::iterator iter;
    for (iter = inlets_.begin(); iter != inlets_.end(); ++iter)
    {
        if ((*iter).get() == sink)
        {
            return true;
        }
    }
    return false;
}

bool Node::hasOutlet(Source *source)
{
    typename std::vector<std::tr1::shared_ptr<Source> >::iterator iter;
    for (iter = outlets_.begin(); iter != outlets_.end(); ++iter)
    {
        if ((*iter).get() == source)
        {
            return true;
        }
    }
    return false;
}

} // end of namespace

