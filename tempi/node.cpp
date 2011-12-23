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
    addInlet("attributes"); // all nodes have at least one inlet for attributes
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
        std::map<std::string, Attribute::ptr>::const_iterator iter;
        for (iter = attributes_.begin(); iter != attributes_.end(); ++iter)
        {
            // Updates properties, etc.
            onAttributeChanged((*iter).first.c_str(), (*iter).second->getValue());
        }
        return true;
    }
}

void Node::onInit()
{
    // pass
}

std::map<std::string, Outlet::ptr> Node::getOutlets()
{
    return outlets_;
}

void Node::onInletTriggered(Inlet *inlet, const Message &message)
{
    //std::cout << __FUNCTION__ << std::endl;
    bool is_a_attribute = false;
    if (inlet->getName() == "0" && message.getSize() >= 3)
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
        processMessage(inlet->getName().c_str(), message);
    }
}

std::map<std::string, Inlet::ptr> Node::getInlets()
{
    return inlets_;
}

bool Node::addOutlet(Outlet::ptr outlet)
{
    if (! hasOutlet(outlet.get()))
    {
        outlets_[outlet->getName()] = outlet;
        return true;
    }
    return false;
}

bool Node::addInlet(Inlet::ptr inlet)
{
    if (! hasInlet(inlet.get()))
    {
        inlets_[inlet->getName()] = inlet;
        inlet.get()->getOnTriggeredSignal().connect(boost::bind(&Node::onInletTriggered, this, _1, _2));
        return true;
    }
    return false;
}

bool Node::addInlet(const char *name, const char *documentation)
{
    return addInlet(Inlet::ptr(new Inlet(name, documentation)));
}

bool Node::addOutlet(const char *name, const char *documentation)
{
    return addOutlet(Inlet::ptr(new Outlet(name, documentation)));
}

bool Node::hasInlet(Inlet *inlet)
{
    std::map<std::string, Inlet::ptr>::iterator iter;
    for (iter = inlets_.begin(); iter != inlets_.end(); ++iter)
    {
        if ((*iter).second.get() == inlet)
        {
            return true;
        }
    }
    return false;
}

bool Node::hasInlet(const char *name) const
{
    return inlets_.find(std::string(name)) != inlets_.end();
}

bool Node::hasOutlet(const char *name) const
{
    return outlets_.find(std::string(name)) != outlets_.end();
}

bool Node::hasOutlet(Outlet *outlet)
{
    std::map<std::string, Outlet::ptr>::iterator iter;
    for (iter = outlets_.begin(); iter != outlets_.end(); ++iter)
    {
        if ((*iter).second.get() == outlet)
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
    return outlets_.size();
}

Inlet *Node::getInlet(const char *name) const
{
    if (hasInlet(name))
        return inlets_[name].get();
    else
        return 0; // FIXME
}

Outlet *Node::getOutlet(const char *name) const
{
    if (hasOutlet(name))
        return outlet_[name].get();
    else
        return 0; // FIXME
}

Outlet::ptr Node::getOutletSharedPtr(const char *name) const throw(BadIndexException)
{
    if (hasOutlet(name))
        return outlet_[name];
    else
    {
        std::ostringstream os;
        os << "Node(" << getTypeName() << ":" << getInstanceName() << ")::" << __FUNCTION__ << ": " << ": Bad outlet name: " << name;
        throw BadIndexException(os.str().c_str());
    }
}

Attribute::ptr Node::getAttribute(const char *name) const throw(BadIndexException)
{
    std::map<std::string, Attribute::ptr>::const_iterator iter = attributes_.find(std::string(name));
    if (iter == attributes_.end())
    {
        std::ostringstream os;
        os << "Node(" << getTypeName() << ":" << getInstanceName() << ")::" << __FUNCTION__ << ": " << ": No such attribute: " << name;
        throw (BadIndexException(os.str().c_str()));
    }
    else
        return (*iter).second;
}

const Message &Node::getAttributeValue(const char *name) const throw(BadIndexException)
{
    return getAttribute(name)->getValue();
}

bool Node::hasAttribute(const char *name) const
{
    return (attributes_.find(std::string(name)) != attributes_.end());
}

void Node::addAttribute(const char *name, const Message &value, const char *doc, bool type_strict) throw(BadIndexException)
{
    if (hasAttribute(name))
    {
        std::ostringstream os;
        os << "Node::" << __FUNCTION__ << ": Already has attribute: " << name;
        throw (BadIndexException(os.str().c_str()));
    }
    Attribute::ptr attr(new Attribute(name, value, doc, type_strict));
    attributes_[std::string(name)] = attr;
}

void Node::setAttribute(const char *name, const Message &value) throw(BadIndexException, BadArgumentTypeException)
{
    bool ok_to_change = false;
    Attribute::ptr current = getAttribute(name); // might throw BadIndexException
    //std::cout << "Node::" << __FUNCTION__ << ": " << name << " = " << value << std::endl;
    if (current->isTypeStrict())
    {
        if (current->getValue().getTypes().compare(value.getTypes()) == 0)
        {
            ok_to_change = true;
        }
        else
        {
            std::ostringstream os;
            os << "Node::" << __FUNCTION__ << ": Attribute " << name << ": Bad type " << value.getTypes() << " while expecting " << current->getValue().getTypes();
            throw (BadArgumentTypeException(os.str().c_str()));
        }
    }
    else
        ok_to_change = true;
    // do it:
    if (ok_to_change)
    {
        current->setValue(value);
        if (isInitiated())
            onAttributeChanged(name, value);
    }
}

std::vector<std::string> Node::getAttributesNames() const
{
    std::vector<std::string> ret;
    std::map<std::string, Attribute::ptr>::const_iterator iter;
    for (iter = attributes_.begin(); iter != attributes_.end(); ++iter)
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
    Outlet::ptr outlet_ptr = getOutletSharedPtr(outlet);
    outlet_ptr->trigger(message);
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

