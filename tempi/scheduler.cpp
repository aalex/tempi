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

namespace tempi {

Scheduler::Scheduler()
{
    factory_.reset(new NodeFactory);
    // TODO: do not load internals by default?
    internals::loadInternals(*(factory_.get()));
}
bool Scheduler::createGraph(const char *name)
{
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

bool Scheduler::sendToAllGraphs(const Message &message)
{
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

Graph::ptr Scheduler::getGraph(const char *name)
{
    if (! hasGraph(name))
    {
        std::cout << "No Graph named "
            << name << std::endl;
        return Graph::ptr(); // NULL pointer!!
    }
    return (* (graphs_.find(std::string(name)))).second;

}

bool Scheduler::hasGraph(const char *name)
{
    return graphs_.find(std::string(name)) != graphs_.end();
}

NodeFactory::ptr Scheduler::getFactory() const
{
    return factory_;
}

bool Scheduler::tickGraphs()
{
    if (! isRunning())
        return false;
    std::map<std::string, Graph::ptr>::const_iterator iter;
    for (iter = graphs_.begin(); iter != graphs_.end(); ++iter)
    {
        (*iter).second.get()->tick();
    }
    return true;
}

} // end of namespace

