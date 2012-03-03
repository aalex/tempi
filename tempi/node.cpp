/*
 * Copyright (C) 2011 Alexandre Quessy
 * Copyright (C) 2011 Michal Seta
 * Copyright (C) 2012 Nicolas Bouillot
 *
 * This file is part of Tempi.
 *
 * This program is free software: you can redistribute it and/or
 * modify it under the terms of, either version 3 of the License, or
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
#include "tempi/log.h"
#include <boost/bind.hpp>
#include <iostream>
#include <sstream>

namespace tempi
{

Node::Node()
{
    load_banged_ = false;
    // XXX: Add Signals BEFORE adding inlets/outlets!
    // TODO: intercept ATTRIBUTE_LOG when set to make sure it's in the right range.
    //addAttribute(ATTRIBUTE_LOG, Message("i", 1), "How much [1-5] to print debug info in the console. 1=ERROR, 2=CRITICAL, 3=WARNING, 4=INFO, 5=DEBUG");
    //TODO: __attr__ and __log__ ?
    //std::cout << __FUNCTION__ << "()" << std::enD

    addSignal(NodeSignal::ptr(new NodeSignal(OUTLET_DELETED_SIGNAL,
        "Triggered when an outlet is deleted. Arguments are: the name of this node, the name of the outlet.", "TODO", "ss")));
    addSignal(NodeSignal::ptr(new NodeSignal(INLET_DELETED_SIGNAL,
        "Triggered when an inlet is deleted. Arguments are: the name of this node, the name of the inlet.", "TODO", "ss")));
    addSignal(NodeSignal::ptr(new NodeSignal(OUTLET_CREATED_SIGNAL,
        "Triggered when an outlet is created. Arguments are: the name of this node, the name of the outlet.", "TODO", "ss")));
    addSignal(NodeSignal::ptr(new NodeSignal(INLET_CREATED_SIGNAL,
        "Triggered when an inlet is created. Arguments are: the name of this node, the name of the inlet.", "TODO", "ss")));

    addInlet(ATTRIBUTES_INLET, "Set attribute value with (s:\"set\", s:name, ...)"); // all nodes have at least one inlet for attributes
    //addAttribute("log-level", Message("i", 1), "How much [1-5] to print debug info in the console. 1=ERROR, 2=CRITICAL, 3=WARNING, 4=INFO, 5=DEBUG");
}

bool Node::isInitiated() const
{
    return initiated_;
}

bool Node::init()
{
    {
        std::ostringstream os;
        os << "Node.init(): " << getName();
        Logger::log(DEBUG, os.str().c_str());
    }
    if (isInitiated())
        return false;
    else
    {
        initiated_ = true; // very important!
        onInit();
        std::vector<std::string> attributes = listAttributes();
        std::vector<std::string>::const_iterator iter;
        for (iter = attributes.begin(); iter != attributes.end(); ++iter)
        {
            // Updates properties, etc.
            Attribute* attribute = getAttribute((*iter).c_str());
            onAttributeChanged((*iter).c_str(), attribute->getValue());
            // add an inlet for each attribute
            addInlet((*iter).c_str(), attribute->getShortDocumentation().c_str());
        }
        return true;
    }
}

void Node::loadBang()
{
    {
        std::ostringstream os;
        os << "Node.loadBang: " << getName();
        Logger::log(DEBUG, os.str().c_str());
    }
    if (! load_banged_)
    {
        onLoadBang();
        load_banged_ = true;
    }
    else
    {
        std::ostringstream os;
        os << "Node.loadBang: had already been loadBanged: " << getName();
        Logger::log(DEBUG, os.str().c_str());
    }
}

void Node::onInit()
{
    // pass
}

void Node::onLoadBang()
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
    if (inlet->getName() == ATTRIBUTES_INLET && message.getSize() >= 3)
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
                    std::cerr << "Node(" << getTypeName() << ":" << getName() << ")::" << __FUNCTION__ << ": " << e.what();
                }
                catch (const BadArgumentTypeException &e)
                {
                    std::cerr << "Node(" << getTypeName() << ":" << getName() << ")::" << __FUNCTION__ << ": " << e.what();
                }
            }
        }
    }

    if (is_a_attribute)
        return;
    
    // Attribute inlet:
    std::vector<std::string> attributes = listAttributes();
    std::vector<std::string>::const_iterator iter;
    for (iter = attributes.begin(); iter != attributes.end(); ++iter)
    {
        if (inlet->getName() == (*iter))
        {
            try
            {
                setAttribute(inlet->getName().c_str(), message);
            }
            catch (const BadArgumentTypeException &e)
            {
                std::cerr << "Node(" << getTypeName() << ":" << getName() << ")::" << __FUNCTION__ << ": " << e.what();
            }
            is_a_attribute = true;
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

std::vector<std::string> Node::listInlets() const
{
    std::vector<std::string> ret;
    std::map<std::string, Inlet::ptr>::const_iterator iter;
    for (iter = inlets_.begin(); iter != inlets_.end(); ++iter)
        ret.push_back((*iter).first);
    return ret;
}

std::vector<std::string> Node::listOutlets() const
{
    std::vector<std::string> ret;
    std::map<std::string, Outlet::ptr>::const_iterator iter;
    for (iter = outlets_.begin(); iter != outlets_.end(); ++iter)
        ret.push_back((*iter).first);
    return ret;
}

bool Node::addOutlet(Outlet::ptr outlet)
{
    if (! hasOutlet(outlet.get()))
    {
        {
            std::ostringstream os;
            os << "Node.addOutlet: (" << getName() << "): " << outlet->getName();
            Logger::log(DEBUG, os.str().c_str());
        }
        outlets_[outlet->getName()] = outlet;
        try
        {
            getSignal(OUTLET_CREATED_SIGNAL)->trigger(
                Message("ss", getName().c_str(), outlet->getName().c_str()));
        }
        catch (const BadIndexException &e)
        {
            std::cerr << "In Node::" << __FUNCTION__ << ": already got such and outlet: " << std::endl;
            std::cerr << e.what() << std::endl;
        }
        return true;
    }
    return false;
}

bool Node::addInlet(Inlet::ptr inlet)
{
    if (! hasInlet(inlet.get()))
    {
        inlets_[inlet->getName()] = inlet;
        {
            std::ostringstream os;
            os << "Node.addInlet: (" << getName() << "): " << inlet->getName();
            Logger::log(DEBUG, os.str().c_str());
        }
        inlet.get()->getOnTriggeredSignal().connect(boost::bind(&Node::onInletTriggered, this, _1, _2));
        try
        {
            getSignal(INLET_CREATED_SIGNAL)->trigger(
                Message("ss", getName().c_str(), inlet->getName().c_str()));
        }
        catch (const BadIndexException &e)
        {
            std::cerr << "In " << __FUNCTION__ << std::endl;
            std::cerr << e.what() << std::endl;
        }
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
    return addOutlet(Outlet::ptr(new Outlet(name, documentation)));
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

Inlet *Node::getInlet(const char *name) const
{
    if (hasInlet(name))
    {
        std::map<std::string, Inlet::ptr>::const_iterator iter = inlets_.find(std::string(name));
        return ((*iter).second).get();
    }
    else
        return 0; // FIXME
}

Outlet *Node::getOutlet(const char *name) const
{
    if (hasOutlet(name))
    {
        std::map<std::string, Outlet::ptr>::const_iterator iter = outlets_.find(std::string(name));
        return ((*iter).second).get();
    }
    else
        return 0; // FIXME
}

Outlet::ptr Node::getOutletSharedPtr(const char *name) const throw(BadIndexException)
{
    if (name == 0)
    {
        std::cerr << "Node::getOutletSharedPtr(): NULL string is not valid.\n";
    }
    if (hasOutlet(name))
    {
        std::map<std::string, Outlet::ptr>::const_iterator iter = outlets_.find(std::string(name));
        return (*iter).second;
    }
    else
    {
        std::ostringstream os;
        os << "Node(" << getTypeName() << ":" << getName() << ")::" << __FUNCTION__ << ": " << ": Bad outlet name: " << name;
        throw BadIndexException(os.str().c_str());
    }
}

bool Node::message(const char *inlet, const Message &message)
{
    if (inlet == 0)
    {
        std::cerr << "Error: Called " << __FUNCTION__ << "() with null-string inlet on node of type " << getTypeName() << ": " << message << std::endl;
        return false;
    }
    if (isInitiated())
    {
        Inlet *inletPtr = getInlet(inlet);
        if (inletPtr == 0)
        {
            std::cerr << "Error: Node::message(): Node of type " << getTypeName() << " has no inlet named " << inlet << "!!" << std::endl;
            // : Called " << __FUNCTION__ << "() on Node of type " << getTypeName() << " via inlet " << inlet << " but could not find such an inlet. Message is: " << message << std::endl;
            return false;
        }
        inletPtr->trigger(message);
        return true;
    }
    else
    {
        std::cerr << "Warning: Called " << __FUNCTION__ << "() on null-string Node of type " << getTypeName() << " in inlet " << inlet << ": " << message << std::endl;
        return false;
    }
}

void Node::output(const char *outlet, const Message &message) const throw(BadIndexException)
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

void Node::enableHandlingReceiveSymbol(const char *selector)
{
    handledReceiveSymbol_ = selector;
}

bool Node::handlesReceiveSymbol(const char *selector) const
{
    return handledReceiveSymbol_ == selector;
}

bool Node::isLoadBanged() const
{
    return load_banged_;
}

bool Node::removeOutlet(const char *name)
{
    std::string nameStr(name);
    if (hasOutlet(name))
    {
        outlets_.erase(outlets_.find(nameStr));
        Message message("ss", this->getName().c_str(), name); // node, inlet
        getSignal(OUTLET_DELETED_SIGNAL)->trigger(message);
        return true;
    }
    return false;
}

bool Node::removeInlet(const char *name)
{
    std::string nameStr(name);
    if (hasInlet(name))
    {
        inlets_.erase(inlets_.find(nameStr));
        Message message("ss", this->getName().c_str(), name); // node, inlet
        getSignal(INLET_DELETED_SIGNAL)->trigger(message);
        return true;
    }
    return false;
}

void Node::setGraph(Graph *graph)
{
    graph_ = graph;
}

Graph* Node::getGraph() const
{
    return graph_;
}

} // end of namespace

