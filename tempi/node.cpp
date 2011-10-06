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

#include "tempi/node.h"
#include <boost/bind.hpp>
#include <iostream>
#include <sstream>

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

void Node::onInletTriggered(Sink *sink, const Message &message)
{
    //std::cout << __FUNCTION__ << std::endl;
    unsigned int index = getInletIndex(sink);
    processMessage(index, message);
}

unsigned int Node::getInletIndex(Sink *sink) const throw(BadIndexException)
{
    unsigned int index = 0;
    std::vector<std::tr1::shared_ptr<Sink> >::const_iterator iter;
    for (iter = inlets_.begin(); iter != inlets_.end(); ++iter)
    {
        if ((*iter).get() == sink)
            return index;
        ++index;
    }
    std::ostringstream os;
    os << "Node::" << __FUNCTION__ << ": No such inlet pointer";
    throw BadIndexException(os.str().c_str());
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
        sink.get()->getOnTriggeredSignal().connect(boost::bind(&Node::onInletTriggered, this, _1, _2));
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
    std::vector<std::tr1::shared_ptr<Sink> >::iterator iter;
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
    std::vector<std::tr1::shared_ptr<Source> >::iterator iter;
    for (iter = outlets_.begin(); iter != outlets_.end(); ++iter)
    {
        if ((*iter).get() == source)
        {
            return true;
        }
    }
    return false;
}

void Node::tick()
{
    doTick();
}

void Node::doTick()
{
    // pass
}

unsigned int Node::getNumberOfInlets() const
{
    return inlets_.size();
}

unsigned int Node::getNumberOfOutlets() const
{
    return inlets_.size();
}

Sink *Node::getInlet(unsigned int number) const
{
    if (number >= getNumberOfInlets())
    {
        std::cout << "Node::" << __FUNCTION__ << ": Inlet " << number << "too big for node." << std::endl;
        return 0;
    }
    return inlets_[number].get();
}

Source *Node::getOutlet(unsigned int number) const
{
    if (number >= getNumberOfInlets())
    {
        std::cout << "Node::" << __FUNCTION__ << ": Outlet " << number << "too big for node." << std::endl;
        return 0;
    }
    return outlets_[number].get();
}

std::tr1::shared_ptr<Source> Node::getOutletSharedPtr(unsigned int number) const throw(BadIndexException)
{
    if (number >= getNumberOfInlets())
    {
        std::ostringstream os;
        os << "Node::" << __FUNCTION__ << ": Bad outlet index: " << number;
        throw BadIndexException(os.str().c_str());
    }
    return outlets_[number];
}

const Message &Node::getProperty(const char *name) const throw(BadIndexException)
{
    std::map<std::string, Message>::const_iterator iter = properties_.find(std::string(name));
    if (iter == properties_.end())
    {
        std::ostringstream os;
        os << "Node::" << __FUNCTION__ << ": No such property: " << name;
        throw (BadIndexException(os.str().c_str()));
    }
    else
        return (*iter).second;
}

bool Node::hasProperty(const char *name) const
{
    return (properties_.find(std::string(name)) != properties_.end());
}

void Node::addProperty(const char *name, const Message &property) throw(BadIndexException)
{
    if (hasProperty(name))
    {
        std::ostringstream os;
        os << "Node::" << __FUNCTION__ << ": Already has property: " << name;
        throw (BadIndexException(os.str().c_str()));
    }
    properties_[std::string(name)] = property;
}

void Node::setProperty(const char *name, const Message &value) throw(BadIndexException, BadArgumentTypeException)
{
    Message current = getProperty(name);
    if (current.getTypes() == value.getTypes())
    {
        if (value == current)
        {
            //std::cerr << "Node::" << __FUNCTION__ << ": Not changing value." << std::endl;
        }
        else
        {
            properties_[std::string(name)] = value;
            onPropertyChanged(name, value);
        }
    }
    else
    {
        std::ostringstream os;
        os << "Node::" << __FUNCTION__ << ": Property " << name << ": Bad type " << value.getTypes() << " while expecting " << current.getTypes();
        throw (BadIndexException(os.str().c_str()));
    }
}

} // end of namespace


