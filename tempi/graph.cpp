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

Graph::Graph(NodeFactory::ptr factory) :
    factory_(factory)
{
}

Graph::Graph()
{
}

bool Graph::addNode(const char *type, const char *name)
{
    if (factory_.get() == 0)
    {
        std::cerr << "Graph::" << __FUNCTION__ << ": this NodeFactory is an Invalid pointer." << std::endl;
        return false;
    }
    if (factory_->hasType(type))
    {
        //std::cerr << "Graph::" << __FUNCTION__ << ": NodeFactory does have type " << type << std::endl;
        if (getNode(name) != 0)
        {
            std::cerr << "Graph::" << __FUNCTION__ << ": There is already a node with ID " << name << std::endl;
            return false;
        }
        Node::ptr node = factory_->create(type);
        if (node.get() == 0)
        {
            std::cerr << "Graph::" << __FUNCTION__ << ": Invalid pointer to Node." << std::endl;
            return false;
        }
        node->setTypeName(type);
        node->setInstanceName(name);
        nodes_[name] = node;
        return true;
    }
    else
    {
        std::cerr << "Graph::" << __FUNCTION__ << ": This NodeFactory doesn't have type " << type << std::endl;
        //std::cerr << "Graph::" << __FUNCTION__ << ": Look:" << std::endl;
        //std::cerr << "Graph::" << __FUNCTION__ << ": " << *factory_.get();
        return false; // FIXME
    }
}

bool Graph::message(const char *node, unsigned int inlet, const Message &message)
{
    Node *nodePtr = getNode(node);
    if (nodePtr == 0)
    {
        std::cerr << "Graph::" << __FUNCTION__ << ": No such node: " << node << std::endl;
        return false;
    }
    return nodePtr->message(inlet, message);
}

bool Graph::connect(const char *from, unsigned int outlet, const char *to, unsigned int inlet)
{
    Node *fromNode = getNode(from);
    if (fromNode == 0)
    {

        std::cerr << "Graph::" << __FUNCTION__ << ": Cannot find node " << from << "." << std::endl;
        return false;
    }
    Node *toNode = getNode(to);
    if (toNode == 0)
    {
        std::cerr << "Graph::" << __FUNCTION__ << ": Cannot find node " << to << "." << std::endl;
        return false;
    }
    if (outlet >= fromNode->getNumberOfOutlets())
    {
        std::cerr << "Graph::" << __FUNCTION__ << ": Outlet " << outlet << "too big for node " << from << "." << std::endl;
        return false;
    }
    if (inlet >= toNode->getNumberOfInlets())
    {
        std::cerr << "Graph::" << __FUNCTION__ << ": Inlet " << inlet << "too big for node " << to << "." << std::endl;
        return false;
    }
    try
    {
        toNode->getInlet(inlet)->connect(fromNode->getOutletSharedPtr(outlet));
    }
    catch (const BadIndexException &e)
    {
        std::cerr << e.what() << std::endl;
        return false;
    }
    return true;
}

bool Graph::isConnected(const char *from, unsigned int outlet, const char *to, unsigned int inlet)
{
    Node *fromNode = getNode(from);
    if (fromNode == 0)
    {

        std::cerr << "Graph::" << __FUNCTION__ << ": Cannot find node " << from << "." << std::endl;
        return false;
    }
    Node *toNode = getNode(to);
    if (toNode == 0)
    {
        std::cerr << "Graph::" << __FUNCTION__ << ": Cannot find node " << to << "." << std::endl;
        return false;
    }
    if (outlet >= fromNode->getNumberOfOutlets())
    {
        std::cerr << "Graph::" << __FUNCTION__ << ": Outlet " << outlet << "too big for node " << from << "." << std::endl;
        return false;
    }
    if (inlet >= toNode->getNumberOfInlets())
    {
        std::cerr << "Graph::" << __FUNCTION__ << ": Inlet " << inlet << "too big for node " << to << "." << std::endl;
        return false;
    }
    // no need to catch BadIndexException sinze already tested it
    Source::ptr source = fromNode->getOutletSharedPtr(outlet);
    Sink *sink = toNode->getInlet(inlet);
    return sink->isConnected(source);
}

bool Graph::disconnect(const char *from, unsigned int outlet, const char *to, unsigned int inlet)
{
    Node *fromNode = getNode(from);
    if (fromNode == 0)
    {

        std::cerr << "Graph::" << __FUNCTION__ << ": Cannot find node " << from << "." << std::endl;
        return false;
    }
    Node *toNode = getNode(to);
    if (toNode == 0)
    {
        std::cerr << "Graph::" << __FUNCTION__ << ": Cannot find node " << to << "." << std::endl;
        return false;
    }
    if (outlet >= fromNode->getNumberOfOutlets())
    {
        std::cerr << "Graph::" << __FUNCTION__ << ": Outlet " << outlet << "too big for node " << from << "." << std::endl;
        return false;
    }
    if (inlet >= toNode->getNumberOfInlets())
    {
        std::cerr << "Graph::" << __FUNCTION__ << ": Inlet " << inlet << "too big for node " << to << "." << std::endl;
        return false;
    }
    // no need to catch BadIndexException sinze already tested it
    Source::ptr source = fromNode->getOutletSharedPtr(outlet);
    Sink *sink = toNode->getInlet(inlet);
    return sink->disconnect(source);
}

Node *Graph::getNode(const char *name) const
{
    std::string nameString(name);
    NodesMapType::const_iterator iter = nodes_.find(nameString);
    if (iter == nodes_.end())
    {
        return 0;
    }
    else
        return (*iter).second.get();
}

void Graph::tick()
{
    // FIXME: nodes are ticked in a random order, pretty much
    NodesMapType::const_iterator iter;
    for (iter = nodes_.begin(); iter != nodes_.end(); ++iter)
    {
        (*iter).second.get()->tick();
    }
}

std::vector<Graph::Connection> Graph::getAllConnectedTo(const char *name, unsigned int inlet)
{
    std::vector<Connection> ret;
    NodesMapType::const_iterator iter;
    for (iter = nodes_.begin(); iter != nodes_.end(); ++iter)
    {
        Node *node = (*iter).second.get();
        std::string fromName((*iter).first);
        for (unsigned int outlet = 0; outlet < node->getNumberOfOutlets(); ++outlet)
        {
            if (isConnected(fromName.c_str(), outlet, name, inlet))
            {
                ret.push_back(Connection(fromName, outlet, name, inlet));
            }
        }
    }
    return ret;
}

std::vector<Graph::Connection> Graph::getAllConnectedFrom(const char *name, unsigned int inlet)
{
    std::vector<Connection> ret;
    NodesMapType::const_iterator iter;
    for (iter = nodes_.begin(); iter != nodes_.end(); ++iter)
    {
        Node *node = (*iter).second.get();
        std::string toName((*iter).first);
        for (unsigned int outlet = 0; outlet < node->getNumberOfOutlets(); ++outlet)
        {
            if (isConnected(name, outlet, toName.c_str(), inlet))
            {
                ret.push_back(Connection(toName, outlet));
            }
        }
    }
    return ret;
}

std::vector<Graph::Connection> Graph::getAllConnectedTo(const char *name)
{
    Node *node = getNode(name);
    ConnectionVec ret;
    for (unsigned int inlet = 0; inlet < node->getNumberOfInlets(); ++inlet)
    {
        ConnectionVec connections = getAllConnectedTo(name, inlet);
        ConnectionVec::const_iterator iter;
        for (iter = connections.begin(); iter != connections.end(); ++iter)
        {
            ret.push_back(*iter);
        }
    }
    return ret;
}

std::vector<Graph::Connection> Graph::getAllConnectedFrom(const char *name)
{
    Node *node = getNode(name);
    ConnectionVec ret;
    for (unsigned int outlet = 0; outlet < node->getNumberOfOutlets(); ++outlet)
    {
        ConnectionVec connections = getAllConnectedFrom(name, outlet);
        ConnectionVec::const_iterator iter;
        for (iter = connections.begin(); iter != connections.end(); ++iter)
        {
            ret.push_back(*iter);
        }
    }
    return ret;
}

void Graph::disconnectAllConnectedTo(const char *name)
{
    // FIXME: O(n!)
    Node *node = getNode(name);
    ConnectionVec connections = getAllConnectedTo(name);
    ConnectionVec::const_iterator iter;
    for (iter = connections.begin(); iter != connections.end(); ++iter)
    {
        Connection conn = (*iter);
        disconnect(conn.get<0>().c_str(), conn.get<1>(), conn.get<2>().c_str(), conn.get<3>());
    }
}

void Graph::disconnectAllConnectedFrom(const char *name)
{
    // FIXME: O(n!)
    Node *node = getNode(name);
    ConnectionVec connections = getAllConnectedFrom(name);
    ConnectionVec::const_iterator iter;
    for (iter = connections.begin(); iter != connections.end(); ++iter)
    {
        Connection conn = (*iter);
        disconnect(conn.get<0>().c_str(), conn.get<1>(), conn.get<2>().c_str(), conn.get<3>());
    }
}

std::vector<Graph::Connection> Graph::getAllConnections()
{
    // FIXME: O(n!)
    std::vector<Connection> ret;
    NodesMapType::const_iterator iter;
    for (iter = nodes_.begin(); iter != nodes_.end(); ++iter)
    {
        std::string nodeName = (*iter).first;
        ConnectionVec connections = getAllConnectedFrom(nodeName.c_str());
        ConnectionVec::const_iterator iter2;
        for (iter2 = connections.begin(); iter2 != connections.end(); ++iter2)
        {
            ret.push_back(*iter2);
        }
    }
    return ret;
}

bool Graph::deleteNode(const char *name)
{
    Node *node = getNode(name);
    if (node == 0)
    {
        std::cerr << "Graph::" << __FUNCTION__ << ": Cannot find node " << name << "." << std::endl;
        return false;
    }
    else
    {
        disconnectAllConnectedTo(name);
        disconnectAllConnectedFrom(name);
        std::string nameString(name);
        NodesMapType::iterator iter = nodes_.find(nameString);
        nodes_.erase(iter);
        return true;
    }
}

bool Graph:: hasNode(const char *name) const
{
    return getNode(name) != 0;
}

std::ostream &operator<<(std::ostream &os, const Graph &graph)
{
    os << "Graph: " << std::endl;
    os << " TODO " << std::endl;
    return os;
}

} // end of namespace

