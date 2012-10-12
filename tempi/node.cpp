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

const char * const Node::ATTRIBUTES_GET_METHOD_SELECTOR = "get";
const char * const Node::ATTRIBUTES_GET_OUTPUT_PREFIX = "get";
const char * const Node::ATTRIBUTES_INLET = "__attr__";
const char * const Node::ATTRIBUTES_LIST_METHOD_SELECTOR = "list";
const char * const Node::ATTRIBUTES_LIST_OUTPUT_PREFIX = "list";
const char * const Node::ATTRIBUTES_OUTLET = "__attr__";
const char * const Node::ATTRIBUTES_SET_METHOD_SELECTOR = "set";
const char * const Node::ATTRIBUTES_SET_OUTPUT_PREFIX = "set";
const char * const Node::ATTRIBUTE_LOG = "__log__";
const char * const Node::INLET_CREATED_SIGNAL = "__create_inlet__";
const char * const Node::INLET_DELETED_SIGNAL = "__delete_inlet__";
const char * const Node::OUTLET_CREATED_SIGNAL = "__create_outlet__";
const char * const Node::OUTLET_DELETED_SIGNAL = "__delete_outlet__";
const char * const Node::INLET_CALL = "__call__";
const char * const Node::OUTLET_RETURN = "__return__";
const char * const Node::ATTRIBUTE_POSITION = "__position__";

Node::Node()
{
    load_banged_ = false;
    // XXX: Add Signals BEFORE adding inlets/outlets!
    addSignal(NodeSignal::ptr(new NodeSignal(OUTLET_DELETED_SIGNAL,
        "Triggered when an outlet is deleted. Arguments are: the name of this node, the name of the outlet.", "TODO", "ss")));
    addSignal(NodeSignal::ptr(new NodeSignal(INLET_DELETED_SIGNAL,
        "Triggered when an inlet is deleted. Arguments are: the name of this node, the name of the inlet.", "TODO", "ss")));
    addSignal(NodeSignal::ptr(new NodeSignal(OUTLET_CREATED_SIGNAL,
        "Triggered when an outlet is created. Arguments are: the name of this node, the name of the outlet.", "TODO", "ss")));
    addSignal(NodeSignal::ptr(new NodeSignal(INLET_CREATED_SIGNAL,
        "Triggered when an inlet is created. Arguments are: the name of this node, the name of the inlet.", "TODO", "ss")));
    addInlet(ATTRIBUTES_INLET, "Set attribute value with (s:\"set\", s:<name>, ...) and list them via the __attr__ outlet with the (s:\"list\") message. Get attribute value with (s:\"get\", s:<name>)"); // all nodes have at least one inlet for attributes
    addOutlet(ATTRIBUTES_OUTLET, "Outputs value of each attribute, prefixed by their respective name, and also the list of all attributes name, prefixed with s:\"__list__\"."); // all nodes have at least one outlet for attributes
    addOutlet(OUTLET_RETURN, "Outputs return value for every method called, prefixed by the name of the method called.");
    addInlet(INLET_CALL, "Inlet to call methods of this node. Their return value should be output by the __return__ outlet, prefixed by the name of the method called.");
    addAttribute(Attribute::ptr(new Attribute(ATTRIBUTE_POSITION, Message("fff", 0.0f, 0.0f, 0.0f), "Position of the node, in an hypothetical GUI.")));
}

bool Node::isInitiated() const
{
    return initiated_;
}

bool Node::init()
{
    if (Logger::isEnabledFor(DEBUG))
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
        return true;
    }
}

void Node::loadBang()
{
    if (Logger::isEnabledFor(DEBUG))
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
        if (Logger::isEnabledFor(DEBUG))
        {
            std::ostringstream os;
            os << "Node.loadBang: had already been loadBanged: " << getName();
            Logger::log(DEBUG, os.str().c_str());
        }
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

bool Node::onAttributeChanged(const char *name, const Message &value)
{
    bool ok = this->onNodeAttributeChanged(name, value);
    if (ok)
    {
        Message mess = value;
        mess.prependString(name);
        mess.prependString(ATTRIBUTES_SET_OUTPUT_PREFIX);
        output(ATTRIBUTES_OUTLET, mess);
    }
    return ok;
}

void Node::onInletTriggered(Inlet *inlet, const Message &message)
{
    if (inlet->getName() == ATTRIBUTES_INLET)
    {
        if (message.indexMatchesType(0, STRING))
        {
            if (message.getString(0) == ATTRIBUTES_GET_METHOD_SELECTOR)
            {
                if ((message.getSize() == 2) &&
                    (message.indexMatchesType(1, STRING)))
                {
                    std::string attr_name = message.getString(1);
                    Message result = this->getAttribute(attr_name.c_str())->getValue();
                    result.prependString(attr_name.c_str());
                    result.prependString(ATTRIBUTES_GET_OUTPUT_PREFIX);
                    this->output(ATTRIBUTES_OUTLET, result);
                    return;
                }
                else
                {
                    std::ostringstream os;
                    os << "Node." << __FUNCTION__ <<
                        ": Wrong arguments for message " <<
                        message << " in inlet " << ATTRIBUTES_INLET;
                    Logger::log(ERROR, os);
                }
            } // end get
            // set:
            else if (message.getString(0) == ATTRIBUTES_SET_METHOD_SELECTOR)
            {
                if ((message.getSize() >= 3) &&
                    (message.indexMatchesType(1, STRING)))
                {
                    std::string attr_name = message.getString(1);
                    try
                    {
                        Message attribute = message.cloneRange(2,
                            message.getSize() - 1);
                        std::string attr_name = message.getString(1);
                        if (this->getAttribute(attr_name.c_str())->getMutable())
                        {
                            setAttributeValue(attr_name.c_str(), attribute);
                            return;
                        }
                        else
                        {
                            std::ostringstream os;
                            os << "Node." << __FUNCTION__ << ": " <<
                                attr_name <<
                                " is not mutable! Cannot change it via messages.";
                            Logger::log(ERROR, os);
                            return;
                        }
                    }
                    catch (const BadIndexException &e)
                    {
                        std::cerr << "Node " << getTypeName() << " \"" <<
                            getName() << "\":" << __FUNCTION__ << ": " <<
                            message << " " << e.what();
                        return;
                    }
                    catch (const BadAtomTypeException &e)
                    {
                        std::cerr << "Node " << getTypeName() << " \"" <<
                            getName() << "\":" << __FUNCTION__ << ": " <<
                            message << " " << e.what();
                        return;
                    }
                }
                else
                {
                    std::ostringstream os;
                    os << "Node." << __FUNCTION__ <<
                        ": Wrong arguments for message " << message <<
                        " in inlet " << ATTRIBUTES_INLET;
                    Logger::log(ERROR, os);
                    return;
                }
            } // end set
            // ---------- list:
            else if (message.getString(0) == ATTRIBUTES_LIST_METHOD_SELECTOR)
            {
                std::vector<std::string> attributes = this->listAttributes();
                Message attributes_message("s", ATTRIBUTES_LIST_OUTPUT_PREFIX);
                std::vector<std::string>::const_iterator iter;
                for (iter = attributes.begin(); iter != attributes.end(); ++iter)
                {
                    attributes_message.appendString((*iter).c_str());
                }
                this->output(ATTRIBUTES_OUTLET, attributes_message);
            } // end list
            else
            {
                std::ostringstream os;
                os << "Node." << __FUNCTION__ << ": Cannot handle message " << message << " in inlet " << ATTRIBUTES_INLET;
                Logger::log(ERROR, os);
            }
        } // first atom is a string
        else
        {
            std::ostringstream os;
            os << "Node." << __FUNCTION__ << ": Cannot handle message " << message << " in inlet " << ATTRIBUTES_INLET;
            Logger::log(ERROR, os);
        }
        return;
    } // ATTRIBUTES_INLET
    // CALL INLET:
    else if (inlet->getName() == INLET_CALL)
    {
        if (message.indexMatchesType(0, STRING))
        {
            if (this->hasMethod(message.getString(0).c_str()))
            {
                this->getMethod(message.getString(0).c_str())->
                    trigger(message.cloneRange(1, message.getSize()));
                return;
            }
        }
    }
    processMessage(inlet->getName().c_str(), message);
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
    if (Logger::isEnabledFor(DEBUG))
    {
        std::ostringstream os;
        os << "Node::" << __FUNCTION__;
        Logger::log(DEBUG, os);
    }
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

void Node::output(const char *outlet, const Message &message) const
    throw(BadIndexException)
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

