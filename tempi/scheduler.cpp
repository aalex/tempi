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

#include "tempi/internals.h"
#include "tempi/nodefactory.h"
#include "tempi/graph.h"
#include "tempi/message.h"
#include "tempi/scheduler.h"
#include "tempi/log.h"
#include <iostream>

// more thread-safety with mutexes

namespace tempi {

Scheduler::Scheduler()
{
    factory_.reset(new NodeFactory);
    // TODO: do not load internals by default?
    internals::loadInternals(*(factory_.get()));
}

void Scheduler::createGraph(const char *name)
    throw(BadIndexException)
{
    Graph::ptr graph(new Graph(factory_));
    graph->setName(name);
    graph->setScheduler(this);
    graphs_.add(graph);
}

void Scheduler::createRegion(const char *name)
    throw(BadIndexException)
{
    sampler::Region::ptr region(new sampler::Region);
    region->setName(name);
    regions_.add(region);
}

//void Scheduler::sendMessage(const Message &message)
//{
//    doSendMessage(message);
//}

// bool Scheduler::sendToAllGraphs(const Message &message)
// {
//     //if (! makeSureLockIsAcquired())
//     //    return false;
//     bool ret = false;
//     std::cout << __FUNCTION__ << std::endl;
//     std::map<std::string, Graph::ptr>::const_iterator iter;
//     for (iter = graphs_.begin(); iter != graphs_.end(); ++iter)
//     {
//         if ((*iter).second.get()->handleMessage(message))
//             ret = true;
//     }
//     return ret;
// }

Graph::ptr Scheduler::getGraph(const char *name) const
    throw(BadIndexException)
{
    //if (! makeSureLockIsAcquired())
    //    return Graph::ptr();
    return graphs_.get(name);
}

std::vector<std::string> Scheduler::listGraphs() const
{
//    ScopedLock lock = acquireLock();
    return graphs_.listNames();
}

std::vector<std::string> Scheduler::listRegions() const
{
//    ScopedLock lock = acquireLock();
    return regions_.listNames();
}

bool Scheduler::hasGraph(const char *name) const
{
//    ScopedLock lock = acquireLock();
    return graphs_.has(name);
}

bool Scheduler::hasRegion(const char *name) const
{
//    ScopedLock lock = acquireLock();
    return regions_.has(name);
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
    std::vector<std::string> names = listGraphs();
    std::vector<std::string>::const_iterator iter;
    for (iter = names.begin(); iter != names.end(); ++iter)
    {
        getGraph((*iter).c_str())->tick();
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

sampler::Region::ptr Scheduler::getRegion(const char *name) const
    throw(BadIndexException)
{
    return regions_.get(name);
}

void Scheduler::removeGraph(const char *name)
    throw(BadIndexException)
{
    graphs_.remove(name);
}

void Scheduler::removeRegion(const char *name)
    throw(BadIndexException)
{
    regions_.remove(name);
}

std::ostream &operator<<(std::ostream &os, const Scheduler &scheduler)
{
    //if (! scheduler.makeSureLockIsAcquired())
    //    return os;
    os << "Scheduler:" << std::endl;
    os << (* scheduler.getFactory().get()) << std::endl;
    std::vector<std::string> graphsNames = scheduler.listGraphs();
    std::vector<std::string>::const_iterator iter;
    for (iter = graphsNames.begin(); iter != graphsNames.end(); ++iter)
        os << (* scheduler.getGraph((*iter).c_str()).get()) << std::endl;
    return os;
}

} // end of namespace

