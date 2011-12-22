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
#include "tempi/outlet.h"
#include "tempi/sharedptr.h"

namespace tempi
{

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
        bool init();
        bool isInitiated() const;
        /**
         * Returns all its outlets.
         */
        std::vector<Outlet::ptr> getOutlets();
        /**
         * Returns all its inlets.
         */
        std::vector<Inlet::ptr> getInlets();
        unsigned int getNumberOfInlets() const;
        unsigned int getNumberOfOutlets() const;
        bool message(unsigned int inlet, const Message &message);
        Inlet *getInlet(unsigned int number) const;
        // TODO: deprecate getOutlet?
        Outlet *getOutlet(unsigned int number) const;
        Outlet::ptr getOutletSharedPtr(unsigned int number) const throw(BadIndexException);
        /**
         * Triggers whatever time-dependent events. Calleds by the Graph.
         */
        void tick();
        // TODO: properties:
        // std::map<std::string, Message> getAttributes();
        Attribute::ptr getAttribute(const char *name) const throw(BadIndexException);
        const Message &getAttributeValue(const char *name) const throw(BadIndexException);
        const Message &getArguments() const;
        void setArguments(const Message &message);
        bool hasAttribute(const char *name) const;
        /**
         * Sets a attribute value.
         * You can also do this by sending a message in the form s:"set" s:name ...
         * WARNING: if the value has not changed, it won't call onAttributeChanged.
         */
        void setAttribute(const char *name, const Message &value) throw(BadIndexException, BadArgumentTypeException);
        std::string getAttributeType(const char *name);
        std::vector<std::string> getAttributesNames() const;
        //
        // TODO: signals:
        // typedef boost::signals2::signal<void(Message)> Signal;
        // std::map<std::string, Signal> getSignals();
        // type_info *getSignalType(std::string signal);
        void setTypeName(const char *typeName);
        const std::string &getTypeName() const;
        void setInstanceName(const char *instanceName);
        const std::string &getInstanceName() const;
        bool handlesReceiveSymbol(const char *selector) const;
        bool handleReceive(const char *selector, const Message &message)
        {
            if (! handlesReceiveSymbol(selector))
                return false;
            onHandleReceive(selector, message);
            return true;
        }
    protected:
        void enableHandlingReceiveSymbol(const char *selector);
        virtual void onHandleReceive(const char *selector, const Message &message)
        {}
        /**
         * Adds a outlet.
         */
        bool addOutlet();
        /**
         * Adds a inlet.
         */
        bool addInlet();
        /**
         * Adds a outlet.
         */
        bool addOutlet(Outlet::ptr source);
        /**
         * Adds a inlet.
         */
        bool addInlet(Inlet::ptr sink);
        void addAttribute(const char *name, const Message &value, const char *doc="", bool type_strict=true) throw(BadIndexException);
        void output(unsigned int outlet, const Message &message) const throw(BadIndexException);
        virtual void onAttributeChanged(const char *name, const Message &value)
        {}
        virtual void onSetArguments(const Message &message)
        {}
        unsigned int getInletIndex(Inlet *sink) const throw(BadIndexException);
        // TODO: make private:
        void onInletTriggered(Inlet *sink, const Message &message);
        // TODO: make private:
        virtual void processMessage(unsigned int inlet, const Message &message) = 0;
        // TODO: make private:
        virtual void doTick();
        bool hasInlet(Inlet *sink);
        bool hasOutlet(Outlet *source);
        /**
         * Called when init() is called.
         * Subclasses of node should implement this if needed.
         * (for initiating sockets, files, user interfaces, etc.)
         */
        virtual void onInit();
    private:
        bool initiated_;
        std::vector<Outlet::ptr> outlets_;
        std::map<std::string, Attribute::ptr> attributes_;
        std::vector<Inlet::ptr> inlets_;
        Message arguments_;
        std::string typeName_;
        std::string instanceName_;
        std::string handledReceiveSymbol_;
        // TODO: return success
        // TODO: add unsigned int inlet_number
};

} // end of namespace

#endif // ifndef

