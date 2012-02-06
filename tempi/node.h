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

/**
 * @file
 * The Node class.
 */
#ifndef __TEMPI_NODE_H__
#define __TEMPI_NODE_H__

#include <boost/bind.hpp>
#include <vector>
#include "tempi/attribute.h"
#include "tempi/exceptions.h"
#include "tempi/inlet.h"
#include "tempi/message.h"
#include "tempi/nodesignal.h"
#include "tempi/outlet.h"
#include "tempi/sharedptr.h"

namespace tempi
{

const char * const ATTRIBUTES_INLET = "__attr__";
const char * const ATTRIBUTE_LOG = "__log__";
const char * const INLET_CREATED_SIGNAL = "__create_inlet__";
const char * const INLET_DELETED_SIGNAL = "__delete_inlet__";
const char * const OUTLET_CREATED_SIGNAL = "__create_outlet__";
const char * const OUTLET_DELETED_SIGNAL = "__delete_outlet__";

/**
 * A Node is something that element that can be connected to and from other elements.
 * All nodes have at least one inlet for setting properties. (using ,ss... "set" "name" ...)
 */
class Node
{
    public:
        typedef std::tr1::shared_ptr<Node> ptr;
        Node();
        virtual ~Node() {}
        /**
         * Initializes the Node.
         * Should be called when one ticks its parent Graph.
         */
        bool init();
        /**
         * Returns whether or not this node's init() has been called.
         */
        bool isInitiated() const;
        /**
         * Returns all its outlets.
         */
        std::map<std::string, Outlet::ptr> getOutlets();
        /**
         * Returns all its inlets.
         */
        std::map<std::string, Inlet::ptr> getInlets();
        /**
         * Returns the number of inlets this node has.
         */
        unsigned int getNumberOfInlets() const;
        /**
         * Returns the number of outlets this node has.
         */
        unsigned int getNumberOfOutlets() const;
        /**
         * Sends a message to a given inlet of this node.
         * @param inlet Name of the inlet to send a message to.
         * @param message Message to send.
         */
        bool message(const char *inlet, const Message &message);
        /**
         * Retrieves an inlet of this Node.
         */
        Inlet *getInlet(const char *name) const;
        // TODO: deprecate getOutlet?
        Outlet *getOutlet(const char *name) const;
        Outlet::ptr getOutletSharedPtr(const char *name) const throw(BadIndexException);
        /**
         * Triggers whatever time-dependent events. Calleds by the Graph.
         */
        void tick();
        // TODO: properties:
        // std::map<std::string, Message> getAttributes();
        Attribute::ptr getAttribute(const char *name) const throw(BadIndexException);
        /**
         * Returns the value of a given named attribute of this Node.
         */
        const Message &getAttributeValue(const char *name) const throw(BadIndexException);
        /**
         * Checks if this node has a given named attribute.
         * @return True if this node has such an attribute.
         */
        bool hasAttribute(const char *name) const;
        /**
         * Sets a attribute value.
         * You can also do this by sending a message in the form s:"set" s:name ...
         * WARNING: if the value has not changed, it won't call onAttributeChanged.
         */
        void setAttribute(const char *name, const Message &value) throw(BadIndexException, BadArgumentTypeException);
        /**
         * Returns the type for a given named attribute.
         */
        std::string getAttributeType(const char *name);
        /**
         * Return the list of all the attributes names for this Node.
         * Note that some attributes can appear/disappear at run time.
         */
        std::vector<std::string> getAttributesNames() const;
        //
        // signals:
        std::map<std::string, NodeSignal::ptr> getSignals();
        NodeSignal::ptr getSignal(const char *name) throw(BadIndexException);
        bool hasSignal(const char *name);
        /**
         * Sets the type name for this Node.
         * WARNING: This should be only called by the NodeFactory.
         */
        void setTypeName(const char *typeName);
        /**
         * Gets the type name for this Node.
         */
        const std::string &getTypeName() const;
        /**
         * Sets the instance name for this Node.
         * WARNING: Should only be called by its parent Graph.
         */
        void setInstanceName(const char *instanceName);
        /**
         * Gets the instance name for this Node.
         */
        const std::string &getInstanceName() const;
        bool handlesReceiveSymbol(const char *selector) const;
        bool handleReceive(const char *selector, const Message &message)
        {
            if (! handlesReceiveSymbol(selector))
                return false;
            onHandleReceive(selector, message);
            return true;
        }
        /**
         * Checks if this Node has an inlet with the given name.
         * @param name Name of the inlet to look for.
         * @return True if it has it.
         */
        bool hasInlet(const char *name) const;
        /**
         * Checks if this Node has an outlet with the given name.
         * @param name Name of the outlet to look for.
         * @return True if it has it.
         */
        bool hasOutlet(const char *name) const;
        /**
         * Gets the documentation string for this Node type.
         */
        std::string getDocumentation() const;
    protected:
        void enableHandlingReceiveSymbol(const char *selector);
        virtual void onHandleReceive(const char *selector, const Message &message)
        {}
        bool addSignal(NodeSignal::ptr signal);
        /**
         * Adds an outlet.
         */
        bool addOutlet(const char *name, const char *documentation="");
        /**
         * Adds an inlet.
         */
        bool addInlet(const char *name, const char *documentation="");
        /**
         * Adds an outlet.
         */
        bool addOutlet(Outlet::ptr outlet);

        /**
         * Removes an outlet.
         */
        bool removeOutlet(const char *name);
        /**
         * Adds an inlet.
         */
        bool addInlet(Inlet::ptr inlet);
        /**
         * Adds an attribute.
         */
        void addAttribute(const char *name, const Message &value, const char *doc="", bool type_strict=true) throw(BadIndexException);
        /**
         * Outputs a Message through the given outlet.
         */
        void output(const char *outlet, const Message &message) const throw(BadIndexException);
        virtual void onAttributeChanged(const char *name, const Message &value)
        {}
        // TODO: make private:
        void onInletTriggered(Inlet *inlet, const Message &message);
        // TODO: make private:
        virtual void processMessage(const char *inlet, const Message &message) = 0;
        // TODO: make private:
        virtual void doTick();
        bool hasInlet(Inlet *inlet);
        bool hasOutlet(Outlet *outlet);
        /**
         * Called when init() is called.
         * Subclasses of node should implement this if needed.
         * (for initiating sockets, files, user interfaces, etc.)
         */
        virtual void onInit();
        /**
         * Sets the documentation string for this node type.
         */
        void setDocumentation(const char *documentation);
    private:
        bool initiated_;
        std::map<std::string, Outlet::ptr> outlets_;
        std::map<std::string, Attribute::ptr> attributes_;
        std::map<std::string, Inlet::ptr> inlets_;
        std::map<std::string, NodeSignal::ptr> signals_;
        std::string typeName_;
        std::string instanceName_;
        std::string handledReceiveSymbol_;
        std::string documentation_;
        // TODO: return success
        // TODO: add unsigned int inlet_number
};

} // end of namespace

#endif // ifndef

