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
    addInlet(); // all nodes have at least one inlet for properties
}

bool Node::isInitiated() const
{
    return initiated_;
}

bool Node::init()
{
    if (isInitiated())
        return false;
    else
    {
        initiated_ = true; // very important!
        onInit();
        std::map<std::string, Message>::const_iterator iter;
        for (iter = properties_.begin(); iter != properties_.end(); ++iter)
        {
            // Updates properties, etc.
            onAttributeChanged((*iter).first.c_str(), (*iter).second);
        }
        return true;
    }
}

void Node::onInit()
{
    // pass
}

std::vector<Outlet::ptr> Node::getOutlets()
{
    return outlets_;
}

void Node::onInletTriggered(Inlet *sink, const Message &message)
{
    //std::cout << __FUNCTION__ << std::endl;
    unsigned int inlet = getInletIndex(sink);
    bool is_a_attribute = false;
    if (inlet == 0 && message.getSize() >= 3)
    {
        // ArgumentType type0;
        // ArgumentType type1;
        // if (! message.getArgumentType(0, type0))
        // {
        //     std::cout << "could not get arg type for index " << 0 << std::endl;
        //     // TODO: abort
        // }
        // if (! message.getArgumentType(1, type1))
        // {
        //     std::cout << "could not get arg type for index " << 1 << std::endl;
        //     // TODO: abort
        // }
        //if (type0 == STRING && type1 == STRING && 
        if (message.getTypes().compare(0, 2, "ss") == 0)
        {
            if (message.getString(0).compare("set") == 0)
            {
                try
                {
                    Message attribute = message.cloneRange(2, message.getSize() - 1);
                    std::string name = message.getString(1);
                    setAttribute(name.c_str(), attribute);
                    // std::cout << "Node::" << __FUNCTION__ << ": set attribute " << name << ": " << attribute << std::endl;
                    is_a_attribute = true;
                }
                catch (const BadIndexException &e)
                {
                    std::cerr << "Node(" << getTypeName() << ":" << getInstanceName() << ")::" << __FUNCTION__ << ": " << e.what();
                }
                catch (const BadArgumentTypeException &e)
                {
                    std::cerr << "Node(" << getTypeName() << ":" << getInstanceName() << ")::" << __FUNCTION__ << ": " << e.what();
                }
            }
        }
    }
    if (! is_a_attribute)
    {
        processMessage(inlet, message);
    }
}

unsigned int Node::getInletIndex(Inlet *sink) const throw(BadIndexException)
{
    unsigned int index = 0;
    std::vector<Inlet::ptr>::const_iterator iter;
    for (iter = inlets_.begin(); iter != inlets_.end(); ++iter)
    {
        if ((*iter).get() == sink)
            return index;
        ++index;
    }
    std::ostringstream os;
    os << "Node(" << getTypeName() << ":" << getInstanceName() << ")::" << __FUNCTION__ << ": " << ": No such inlet pointer";
    throw BadIndexException(os.str().c_str());
}

std::vector<Inlet::ptr> Node::getInlets()
{
    return inlets_;
}

bool Node::addOutlet(Outlet::ptr source)
{
    if (! hasOutlet(source.get()))
    {
        outlets_.push_back(source);
        return true;
    }
    return false;
}

bool Node::addInlet(Inlet::ptr sink)
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
    return addInlet(Inlet::ptr(new Inlet()));
}

bool Node::addOutlet()
{
    return addOutlet(Outlet::ptr(new Outlet()));
}

bool Node::hasInlet(Inlet *sink)
{
    std::vector<Inlet::ptr>::iterator iter;
    for (iter = inlets_.begin(); iter != inlets_.end(); ++iter)
    {
        if ((*iter).get() == sink)
        {
            return true;
        }
    }
    return false;
}

bool Node::hasOutlet(Outlet *source)
{
    std::vector<Outlet::ptr>::iterator iter;
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
    if (! isInitiated())
        init();
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

Inlet *Node::getInlet(unsigned int number) const
{
    if (number >= getNumberOfInlets())
    {
        std::cout << "Node::" << __FUNCTION__ << ": Inlet " << number << "too big for node." << std::endl;
        return 0;
    }
    return inlets_[number].get();
}

Outlet *Node::getOutlet(unsigned int number) const
{
    if (number >= getNumberOfInlets())
    {
        std::cout << "Node::" << __FUNCTION__ << ": Outlet " << number << "too big for node." << std::endl;
        return 0;
    }
    return outlets_[number].get();
}

Outlet::ptr Node::getOutletSharedPtr(unsigned int number) const throw(BadIndexException)
{
    if (number >= getNumberOfInlets())
    {
        std::ostringstream os;
        os << "Node(" << getTypeName() << ":" << getInstanceName() << ")::" << __FUNCTION__ << ": " << ": Bad outlet index: " << number;
        throw BadIndexException(os.str().c_str());
    }
    return outlets_[number];
}

const Message &Node::getAttribute(const char *name) const throw(BadIndexException)
{
    std::map<std::string, Message>::const_iterator iter = properties_.find(std::string(name));
    if (iter == properties_.end())
    {
        std::ostringstream os;
        os << "Node(" << getTypeName() << ":" << getInstanceName() << ")::" << __FUNCTION__ << ": " << ": No such attribute: " << name;
        throw (BadIndexException(os.str().c_str()));
    }
    else
        return (*iter).second;
}

const Message &Node::getArguments() const
{
    return arguments_;
}
void Node::setArguments(const Message &message)
{
    arguments_ = message;
    onSetArguments(message);
}

bool Node::hasAttribute(const char *name) const
{
    return (properties_.find(std::string(name)) != properties_.end());
}

void Node::addAttribute(const char *name, const Message &attribute) throw(BadIndexException)
{
    if (hasAttribute(name))
    {
        std::ostringstream os;
        os << "Node::" << __FUNCTION__ << ": Already has attribute: " << name;
        throw (BadIndexException(os.str().c_str()));
    }
    properties_[std::string(name)] = attribute;
}

void Node::setAttribute(const char *name, const Message &value) throw(BadIndexException, BadArgumentTypeException)
{
    Message current = getAttribute(name); // might throw BadIndexException
    //std::cout << "Node::" << __FUNCTION__ << ": " << name << " = " << value << std::endl;
    if (current.getTypes().compare(value.getTypes()) == 0)
    {
        properties_[std::string(name)] = value;
        if (isInitiated())
            onAttributeChanged(name, value);
    }
    else
    {
        std::ostringstream os;
        os << "Node::" << __FUNCTION__ << ": Attribute " << name << ": Bad type " << value.getTypes() << " while expecting " << current.getTypes();
        throw (BadArgumentTypeException(os.str().c_str()));
    }
}

std::vector<std::string> Node::getAttributesNames() const
{
    std::vector<std::string> ret;
    std::map<std::string, Message>::const_iterator iter;
    for (iter = properties_.begin(); iter != properties_.end(); ++iter)
    {
        ret.push_back((*iter).first);
    }
    return ret;
}

bool Node::message(unsigned int inlet, const Message &message)
{
    if (inlet >= getNumberOfInlets())
    {
        std::cerr << "Node::" << __FUNCTION__ << ": Inlet " << inlet << "too big for node." << std::endl;
        return false;
    }
    if (isInitiated())
    {
        Inlet *inletPtr = getInlet(inlet);
        inletPtr->trigger(message);
        return true;
    }
    else
    {
        std::cerr << "Warning: Called " << __FUNCTION__ << "() on non-initialized Node of type " << getTypeName() << " in inlet " << inlet << ": " << message << std::endl;
        return false;
    }
}

void Node::output(unsigned int outlet, const Message &message) const throw(BadIndexException)
{
    Outlet::ptr source = getOutletSharedPtr(outlet);
    source->trigger(message);
}

void Node::setTypeName(const char *typeName)
{
    typeName_ = std::string(typeName);
}

const std::string &Node::getTypeName() const
{
    return typeName_;
}

void Node::setInstanceName(const char *instanceName)
{
    instanceName_ = std::string(instanceName);
}

const std::string &Node::getInstanceName() const
{
    return instanceName_;
}

void Node::enableHandlingReceiveSymbol(const char *selector)
{
    handledReceiveSymbol_ = selector;
}

bool Node::handlesReceiveSymbol(const char *selector) const
{
    return handledReceiveSymbol_ == selector;
}

} // end of namespace


