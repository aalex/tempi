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
#include <iostream>

namespace tempi
{

Node::Node()
{
    // pass
}

std::vector<std::tr1::shared_ptr<Source> > Node::getSources()
{
    return sources_;
}

void Node::onSinkTriggered(Source *source, boost::any data)
{
    // TODO
    std::cout << __FUNCTION__ << std::endl;
    processTrigger(source, data);
}

std::vector<std::tr1::shared_ptr<Sink> > Node::getSinks()
{
    return sinks_;
}

bool Node::addSource(std::tr1::shared_ptr<Source> source)
{
    if (! hasSource(source.get()))
    {
        sources_.push_back(source);
        return true;
    }
    return false;
}

bool Node::addSink(std::tr1::shared_ptr<Sink> sink)
{
    if (! hasSink(sink.get()))
    {
        sinks_.push_back(sink);
        sink.get()->on_triggered_signal_.connect(boost::bind(&Node::onSinkTriggered, this, _1, _2));
        return true;
    }
    return false;
}

bool Node::hasSink(Sink *sink)
{
    typename std::vector<std::tr1::shared_ptr<Sink> >::iterator iter;
    for (iter = sinks_.begin(); iter != sinks_.end(); ++iter)
    {
        if ((*iter).get() == sink)
        {
            return true;
        }
    }
    return false;
}

bool Node::hasSource(Source *source)
{
    typename std::vector<std::tr1::shared_ptr<Source> >::iterator iter;
    for (iter = sources_.begin(); iter != sources_.end(); ++iter)
    {
        if ((*iter).get() == source)
        {
            return true;
        }
    }
    return false;
}

} // end of namespace

