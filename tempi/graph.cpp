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

#include "tempi/graph.h"
#include <iostream>

namespace tempi
{

Graph::Graph()
{
}

std::map<std::string, std::tr1::shared_ptr<Node> > Graph::getNodes()
{
    return nodes_;
}

bool Graph::addNode(std::string &name, std::tr1::shared_ptr<Node> &node)
{
    if (getNode(name) != 0)
    {
        std::cout << "Graph::" << __FUNCTION__ << ": There is already a node with ID " << name << std::endl;
        return false;
    }
    nodes_[name] = node;
    return true;
}

bool Graph::message(std::string &node, unsigned int inlet, const Message &message)
{
    Node *nodePtr = getNode(node);
    if (nodePtr == 0)
    {
        std::cout << "Graph::" << __FUNCTION__ << ": No such node: " << node << std::endl;
        return false;
    }
    if (inlet >= nodePtr->getNumberOfInlets())
    {
        std::cout << "Graph::" << __FUNCTION__ << ": Inlet " << inlet << "too big for node " << node << "." << std::endl;
        return false;
    }
    Sink *inletPtr = nodePtr->getInlet(inlet);
    inletPtr->trigger(message);
    return true;
}

bool Graph::connect(std::string &from, unsigned int outlet, std::string &to, unsigned int inlet)
{
    Node *fromNode = getNode(from);
    if (fromNode == 0)
    {

        std::cout << "Graph::" << __FUNCTION__ << ": Cannot find node " << from << "." << std::endl;
        return false;
    }
    Node *toNode = getNode(to);
    if (toNode == 0)
    {

        std::cout << "Graph::" << __FUNCTION__ << ": Cannot find node " << to << "." << std::endl;
        return false;
    }
    if (outlet >= fromNode->getNumberOfOutlets())
    {
        std::cout << "Graph::" << __FUNCTION__ << ": Outlet " << outlet << "too big for node " << from << "." << std::endl;
        return false;
    }
    if (inlet >= toNode->getNumberOfInlets())
    {
        std::cout << "Graph::" << __FUNCTION__ << ": Inlet " << inlet << "too big for node " << to << "." << std::endl;
        return false;
    }
    try
    {
        toNode->getInlet(inlet)->connect(fromNode->getOutletSharedPtr(outlet));
    }
    catch (const BadIndexException &e)
    {
        std::cout << e.what() << std::endl;
        return false;
    }
    return true;
}

Node *Graph::getNode(std::string &name) const
{
    std::map<std::string, std::tr1::shared_ptr<Node> >::const_iterator iter = nodes_.find(name);
    if (iter == nodes_.end())
    {
        return 0;
    }
    else
        return (*iter).second.get();
}

void Graph::tick()
{
    std::map<std::string, std::tr1::shared_ptr<Node> >::iterator iter;
    for (iter = nodes_.begin(); iter != nodes_.end(); ++iter)
    {
        (*iter).second.get()->tick();
    }
}

} // end of namespace

