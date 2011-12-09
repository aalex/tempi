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

#include "tempi/internals.h"
#include "tempi/nodefactory.h"
#include "tempi/graph.h"
#include "tempi/message.h"
#include "tempi/scheduler.h"
#include <iostream>

// more thread-safety with mutexes

namespace tempi {

Scheduler::Scheduler()
{
    factory_.reset(new NodeFactory);
    // TODO: do not load internals by default?
    internals::loadInternals(*(factory_.get()));
}

bool Scheduler::createGraph(const char *name)
{
    //if (! makeSureLockIsAcquired())
    //    return false;
    if (hasGraph(name))
    {
        std::cout << "Already has a Graph named "
            << name << std::endl;
        return false;
    }
    else
    {
        Graph::ptr graph(new Graph(factory_));
        graphs_[name] = graph;
        return true;
    }
}

//void Scheduler::sendMessage(const Message &message)
//{
//    doSendMessage(message);
//}

bool Scheduler::sendToAllGraphs(const Message &message)
{
    //if (! makeSureLockIsAcquired())
    //    return false;
    bool ret = false;
    std::cout << __FUNCTION__ << std::endl;
    std::map<std::string, Graph::ptr>::const_iterator iter;
    for (iter = graphs_.begin(); iter != graphs_.end(); ++iter)
    {
        if ((*iter).second.get()->handleMessage(message))
            ret = true;
    }
    return ret;
}

Graph::ptr Scheduler::getGraph(const char *name) const
{
    //if (! makeSureLockIsAcquired())
    //    return Graph::ptr();
    if (! hasGraph(name))
    {
        std::cout << "No Graph named "
            << name << std::endl;
        return Graph::ptr(); // NULL pointer!!
    }
    return (* (graphs_.find(std::string(name)))).second;
}

std::vector<std::string> Scheduler::getGraphNames() const
{
//    ScopedLock lock = acquireLock();
    std::vector<std::string> ret;
    std::map<std::string, Graph::ptr>::const_iterator iter;
    for (iter = graphs_.begin(); iter != graphs_.end(); ++iter)
    {
        ret.push_back((*iter).first);
    }
    return ret;
}

bool Scheduler::hasGraph(const char *name) const
{
//    ScopedLock lock = acquireLock();
    return graphs_.find(std::string(name)) != graphs_.end();
}

NodeFactory::ptr Scheduler::getFactory() const
{
//    ScopedLock lock = acquireLock();
    return factory_;
}

bool Scheduler::tickGraphs()
{
    //if (! makeSureLockIsAcquired())
    //    return false;
    if (! isRunning())
        return false;
    std::map<std::string, Graph::ptr>::const_iterator iter;
    for (iter = graphs_.begin(); iter != graphs_.end(); ++iter)
    {
        (*iter).second.get()->tick();
    }
    return true;
}


// bool Scheduler::makeSureLockIsAcquired() const
// {
//     if (canGetGraphPtr())
//         return true;
//     else
//     {
//         std::cerr << "Cannot alter the internal state of the scheduler since its mutex is not locked." << std::endl;
//         return false;
//     }
// }

std::ostream &operator<<(std::ostream &os, const Scheduler &scheduler)
{
    //if (! scheduler.makeSureLockIsAcquired())
    //    return os;
    os << "Scheduler:" << std::endl;
    os << (* scheduler.getFactory().get()) << std::endl;
    std::vector<std::string> graphsNames = scheduler.getGraphNames();
    std::vector<std::string>::const_iterator iter;
    for (iter = graphsNames.begin(); iter != graphsNames.end(); ++iter)
        os << (* scheduler.getGraph((*iter).c_str()).get()) << std::endl;
    return os;
}

} // end of namespace

