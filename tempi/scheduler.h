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
 * @file The Scheduler class.
 */

#ifndef __TEMPI_SCHEDULER_H__
#define __TEMPI_SCHEDULER_H__

#include "tempi/nodefactory.h"
#include "tempi/graph.h"
#include "tempi/message.h"
#include <map>

namespace tempi {

/**
 * A Scheduler holds graphs and one can send messages to them, synchronously or asynchronously.
 */
class Scheduler
{
    public:
        Scheduler();
        /**
         * Returns if the scheduler is running or not.
         */
        virtual bool isRunning() const = 0;
        /**
         * Sends a message to all its graphs.
         * See tempi::Graph::handleMessage for handled messages.
         */
        void sendMessage(const Message &message);
        /**
         * Create a Graph identified by a name.
         */
        bool createGraph(const char *name);
        // TODO: bool remoteGraph(const char *name);
        NodeFactory::ptr getFactory() const;
    protected:
        bool sendToAllGraphs(const Message &message);
        Graph::ptr getGraph(const char *name);
        bool hasGraph(const char *name);
        bool tickGraphs();
    private:
        // TODO: rename this
        virtual void doSendMessage(const Message &message) = 0;
        std::map<std::string, Graph::ptr> graphs_;
        NodeFactory::ptr factory_;
};

std::ostream &operator<<(std::ostream &os, const Scheduler &scheduler);

} // end of namespace

#endif // ifndef

