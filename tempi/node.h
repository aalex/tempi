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
#include "tempi/exceptions.h"
#include "tempi/message.h"
#include "tempi/property.h"
#include "tempi/sharedptr.h"
#include "tempi/sink.h"
#include "tempi/source.h"

namespace tempi
{

/**
 * A Node is something that element that can be connected to and from other elements.
 */
class Node
{
    public:
        Node();
        /**
         * Returns all its outlets.
         */
        std::vector<std::tr1::shared_ptr<Source> > getOutlets();
        /**
         * Returns all its inlets.
         */
        std::vector<std::tr1::shared_ptr<Sink> > getInlets();

        unsigned int getNumberOfInlets() const;
        unsigned int getNumberOfOutlets() const;
        /**
         * Adds a outlet.
         */
        bool addOutlet(std::tr1::shared_ptr<Source> source);
        /**
         * Adds a inlet.
         */
        bool addInlet(std::tr1::shared_ptr<Sink> sink);
        /**
         * Adds a outlet.
         */
        bool addOutlet();

        Sink *getInlet(unsigned int number) const;
        // TODO: deprecate getOutlet?
        Source *getOutlet(unsigned int number) const;
        std::tr1::shared_ptr<Source> getOutletSharedPtr(unsigned int number) const throw(BadIndexException);
        /**
         * Adds a inlet.
         */
        bool addInlet();
        /**
         * Triggers whatever time-dependent events. Calleds by the Graph.
         */
        void tick();
        // TODO: properties:
        // std::map<std::string, Message> getProperties();
        Property &getProperty(const char *name) throw(BadIndexException);;
        void addProperty(std::tr1::shared_ptr<Property> property) throw(BadIndexException);
        bool hasProperty(const char *name);
        // bool setProperty(std::string name, Message &value);
        // type_info *getPropertyType(std::string property);
        //
        // TODO: signals:
        // typedef boost::signals2::signal<void(Message)> Signal;
        // std::map<std::string, Signal> getSignals();
        // type_info *getSignalType(std::string signal);
    private:
        virtual void onPropertyChanged(Property &property) {}
        std::vector<std::tr1::shared_ptr<Source> > outlets_;
        std::vector<std::tr1::shared_ptr<Property> > properties_;
        std::vector<std::tr1::shared_ptr<Sink> > inlets_;
        // TODO: return success
        void onInletTriggered(const Message &message);
        virtual void processMessage(const Message &message) = 0;
        virtual void doTick();
        bool hasInlet(Sink *sink);
        bool hasOutlet(Source *source);
};

} // end of namespace

#endif // ifndef

