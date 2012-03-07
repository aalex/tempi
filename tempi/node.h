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
#include "tempi/entity.h"

namespace tempi
{


class Graph; // forward declaration

/**
 * A Node is something that element that can be connected to and from other elements.
 * All nodes have at least one inlet for setting properties. (using ,ss... "set" "name" ...)
 */
class Node : public Entity
{
    public:
        static const char * const ATTRIBUTES_GET_METHOD_SELECTOR;
        static const char * const ATTRIBUTES_GET_OUTPUT_PREFIX;
        static const char * const ATTRIBUTES_INLET;
        static const char * const ATTRIBUTES_LIST_METHOD_SELECTOR;
        static const char * const ATTRIBUTES_LIST_OUTPUT_PREFIX;
        static const char * const ATTRIBUTES_OUTLET;
        static const char * const ATTRIBUTES_SET_METHOD_SELECTOR;
        static const char * const ATTRIBUTES_SET_OUTPUT_PREFIX;
        static const char * const ATTRIBUTE_LOG;
        static const char * const INLET_CREATED_SIGNAL;
        static const char * const INLET_DELETED_SIGNAL;
        static const char * const OUTLET_CREATED_SIGNAL;
        static const char * const OUTLET_DELETED_SIGNAL;
        typedef std::tr1::shared_ptr<Node> ptr;
        Node();
        virtual ~Node() {}
        /**
         * Initializes the Node.
         * Should be called when one ticks its parent Graph.
         */
        bool init();
        void loadBang();
        void setGraph(Graph *graph);
        Graph *getGraph() const;
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
         * Returns the name of all inlets for this Node.
         */
        std::vector<std::string> listInlets() const;
        /**
         * Returns the name of all outlets for this Node.
         */
        std::vector<std::string> listOutlets() const;
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
        /**
         * Sets the type name for this Node.
         * WARNING: This should be only called by the NodeFactory.
         */
        void setTypeName(const char *typeName);
        /**
         * Gets the type name for this Node.
         */
        const std::string &getTypeName() const;
        ///**
        // * Sets the instance name for this Node.
        // * WARNING: Should only be called by its parent Graph.
        // */
        //void setInstanceName(const char *instanceName) // TODO:deprecate
        //{
        //    setName(instanceName);
        //}
        ///**
        // * Gets the instance name for this Node.
        // */
        //std::string getInstanceName() const // TODO:deprecate
        //{
        //    return getName();
        //}
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
        bool isLoadBanged() const;
        /**
         * Children of Node should not override this method.
         */
        virtual bool onAttributeChanged(const char *name, const Message &value);
        /**
         * Return ok if it is fine to change it to that new value.
         */
        virtual bool onNodeAttributeChanged(const char *name, const Message &value)
        {
            return true;
        }
    protected:
        void enableHandlingReceiveSymbol(const char *selector);
        virtual void onHandleReceive(const char *selector, const Message &message)
        {}
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
        bool removeInlet(const char *name);
        /**
         * Adds an inlet.
         */
        bool addInlet(Inlet::ptr inlet);
        /**
         * Adds an attribute.
         * TODO: remove this
         */
        // void addAttribute(const char *name, const Message &value, const char *doc="", bool type_strict=true) throw(BadIndexException)
        // {
        //     addAttribute(Attribute::ptr(new Attribute(name, value, doc, type_strict)));
        // }
        /**
         * Outputs a Message through the given outlet.
         */
        void output(const char *outlet, const Message &message) const throw(BadIndexException);
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
         * Called for each node after onInit()
         */
        virtual void onLoadBang();
    private:
        bool initiated_;
        bool load_banged_;
        std::map<std::string, Outlet::ptr> outlets_;
        std::map<std::string, Inlet::ptr> inlets_;
        std::string typeName_;
        std::string instanceName_;
        std::string handledReceiveSymbol_;
        // TODO: return success
        // TODO: add unsigned int inlet_number
        Graph *graph_;
};

} // end of namespace

#endif // ifndef

