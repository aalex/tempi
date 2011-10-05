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

void Node::onInletTriggered(const Message &message)
{
    // TODO
    //std::cout << __FUNCTION__ << std::endl;
    processMessage(message);
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
        sink.get()->getOnTriggeredSignal().connect(boost::bind(&Node::onInletTriggered, this, _1));
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

Property &Node::getProperty(const char *name) throw(BadIndexException)
{
    std::ostringstream os;
    os << "Node::" << __FUNCTION__ << ": Not implemented";
    throw (BadIndexException(os.str().c_str()));

    // std::vector<std::tr1::shared_ptr<Property> >::iterator iter;
    // for (iter = properties_.begin(); iter != properties_.end(); ++iter)
    // {
    //     Property *property = (*iter).get();
    //     if (property->getName() == name)
    //     {
    //         return (*property);
    //     }
    // }
    // std::ostringstream os;
    // os << "Node::" << __FUNCTION__ << ": No such property: " << name;
    // throw (BadIndexException(os.str().c_str()));
}

bool Node::hasProperty(const char *name)
{
    try
    {
        getProperty(name);
        return true;
    }
    catch (const BadIndexException &e)
    {
        return false;
    }
}

void Node::addProperty(std::tr1::shared_ptr<Property> property) throw(BadIndexException)
{
    std::ostringstream os;
    os << "Node::" << __FUNCTION__ << ": Not implemented";
    throw (BadIndexException(os.str().c_str()));

    // std::string name = property.get()->getName();
    // if (hasProperty(name.c_str()))
    // {
    //     std::ostringstream os;
    //     os << "Node::" << __FUNCTION__ << ": Already has property: " << name;
    //     throw (BadIndexException(os.str().c_str()));
    // }
    // properties_.push_back(property);
    // property.get()->getOnChangedSignal().connect(boost::bind(&Node::onPropertyChanged, this, _1));
}

} // end of namespace

