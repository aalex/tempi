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

#include <vector>
#include "tempi/source.h"
#include "tempi/message.h"
#include "tempi/sink.h"
#include "tempi/sharedptr.h"

namespace tempi
{

/**
 * A Node is something that element that can be connected to and from other elements.
 */
class Node
{
    public:
        Node();
        std::vector<std::tr1::shared_ptr<Source> > getOutlets();
        std::vector<std::tr1::shared_ptr<Sink> > getInlets();
        bool addOutlet(std::tr1::shared_ptr<Source> source);
        bool addInlet(std::tr1::shared_ptr<Sink> sink);
        bool addOutlet();
        bool addInlet();
        // TODO: properties:
        // std::map<std::string, Message> getProperties();
        // Message *getProperty(std::string);
        // bool addProperty(std::string name, Message &value);
        // bool setProperty(std::string name, Message &value);
        // type_info *getPropertyType(std::string property);
        //
        // TODO: signals:
        // typedef boost::signals2::signal<void(Message)> Signal;
        // std::map<std::string, Signal> getSignals();
        // type_info *getSignalType(std::string signal);
    private:
        std::vector<std::tr1::shared_ptr<Source> > outlets_;
        std::vector<std::tr1::shared_ptr<Sink> > inlets_;
        void onInletTriggered(Source *source, Message &data);
        virtual void processMessage(Source *source, Message &message) = 0;
        bool hasInlet(Sink *sink);
        bool hasOutlet(Source *source);
};

} // end of namespace

#endif // ifndef

