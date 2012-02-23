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

#include "tempi/graph.h"
#include "tempi/utils.h"
#include "tempi/log.h"
#include <iostream>
#include <ostream>
#include <sstream>

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
        if (getNode(name).get() != 0)
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
        node->setName(name);
        nodes_[name] = node;
        std::ostringstream os;
        os << "Graph::" << __FUNCTION__ << "(\"" << type << "\", \"" << name << "\")";
        Logger::log(DEBUG, os.str().c_str());

        try
        {
            //std::cout << "Graph::addNode: node->getSignal()\n";
            node->getSignal(INLET_DELETED_SIGNAL)->getSignal().connect(
                boost::bind(&Graph::onInletDeleted, this, _1));
            node->getSignal(OUTLET_DELETED_SIGNAL)->getSignal().connect(
                boost::bind(&Graph::onOutletDeleted, this, _1));
        }
        catch (const BadIndexException &e)
        {
            // XXX should not occur!!
            std::cerr << __FILE__ << ": " << __FUNCTION__ << std::endl;
            std::cerr << e.what() << std::endl;
        }
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

void Graph::onInletDeleted(const Message &message)
{
    std::string node = message.getString(0);
    std::string inlet = message.getString(1);
    ConnectionVec connections = getAllConnectedTo(node.c_str(), inlet.c_str());
    disconnectMany(connections);
}

void Graph::onOutletDeleted(const Message &message)
{
    std::string node = message.getString(0);
    std::string outlet = message.getString(1);
    ConnectionVec connections = getAllConnectedFrom(node.c_str(), outlet.c_str());
    disconnectMany(connections);
}

bool Graph::message(const char *node, const char *inlet, const Message &message)
{
    Node::ptr nodePtr = getNode(node);
    if (nodePtr.get() == 0)
    {
        std::cerr << "Graph::" << __FUNCTION__ << ": No such node: " << node << std::endl;
        return false;
    }
    else if (inlet == 0)
    {
        std::cerr << "Graph::" << __FUNCTION__ << ": Null inlet string name ! " << std::endl;
        return false;
    }
    return nodePtr->message(inlet, message);
}

bool Graph::connect(const char *from, const char *outlet, const char *to, const char *inlet)
{
    Node::ptr fromNode = getNode(from);
    if (fromNode.get() == 0)
    {

        std::cerr << "Graph::" << __FUNCTION__ << ": Cannot find node \"" << from << "\"." << std::endl;
        return false;
    }
    Node::ptr toNode = getNode(to);
    if (toNode.get() == 0)
    {
        std::cerr << "Graph::" << __FUNCTION__ << ": Cannot find node \"" << to << "\"." << std::endl;
        return false;
    }

    if (inlet == 0 || outlet == 0)
    {
        std::cerr << "Graph::" << __FUNCTION__ << ": Null inlet/outlet!!" << std::endl;
        return false;
    }
    if (! fromNode->hasOutlet(outlet))
    {
        std::cerr << "Graph::" << __FUNCTION__ << ": Outlet " << outlet << " not found in " << from << "." << std::endl;
        return false;
    }
    if (! toNode->hasInlet(inlet))
    {
        std::cerr << "Graph::" << __FUNCTION__ << ": Inlet " << inlet << " not found in " << to << "." << std::endl;
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
    connections_.push_back(Connection(std::string(from), std::string(outlet), std::string(to), std::string(inlet)));
    std::ostringstream os;
    os << "Graph::" << __FUNCTION__ << "(" <<
        from << ":" << outlet << ", " << to << ":" << inlet << ")";
    Logger::log(DEBUG, os.str().c_str());
    return true;
}

bool Graph::isConnected(const char *from, const char *outlet, const char *to, const char *inlet)
{
    if (! hasNode(from))
    {
        std::cerr << "Graph::" << __FUNCTION__ << ": Cannot find node \"" << from << "\"." << std::endl;
        return false;
    }
    if (! hasNode(to))
    {
        std::cerr << "Graph::" << __FUNCTION__ << ": Cannot find node \"" << to << "\"." << std::endl;
        return false;
    }
    std::vector<Connection>::const_iterator iter;
    for (iter = connections_.begin(); iter != connections_.end(); ++iter)
    {
        if ((*iter).get<0>() == from && 
            (*iter).get<1>() == outlet && 
            (*iter).get<2>() == to && 
            (*iter).get<3>() == inlet)
        {
            return true;
        }
    }
    return false;
}

bool Graph::disconnect(const char *from, const char *outlet, const char *to, const char *inlet)
{
    if (isConnected(from, outlet, to, inlet))
    {
        Node::ptr fromNode = getNode(from);
        Node::ptr toNode = getNode(to);
        // no need to catch BadIndexException sinze already tested it
        Outlet::ptr source = fromNode->getOutletSharedPtr(outlet);
        Inlet *inletPtr = toNode->getInlet(inlet);
        return inletPtr->disconnect(source);
    }
    else
        return false;
}

Node::ptr Graph::getNode(const char *name) const
{
    if (name == 0)
    {
        std::cerr << "Graph::getNode() :: null string!\n";
        return Node::ptr((Node *) 0); // NULL pointer
    }
    std::string nameString(name);
    NodesMapType::const_iterator iter = nodes_.find(nameString);
    if (iter == nodes_.end())
    {
        return Node::ptr((Node *) 0); // NULL pointer
    }
    else
        return (*iter).second; // The Node::ptr
}

void Graph::tick()
{
    // FIXME: nodes are ticked in a random order, pretty much
    NodesMapType::const_iterator iter;
    for (iter = nodes_.begin(); iter != nodes_.end(); ++iter)
    {
        Node *node = (*iter).second.get();
        if (! node->isInitiated())
            node->init();
    }
    for (iter = nodes_.begin(); iter != nodes_.end(); ++iter)
    {
        Node *node = (*iter).second.get();
        node->tick();
    }
}

void Graph::loadBang()
{
    NodesMapType::const_iterator iter;
    for (iter = nodes_.begin(); iter != nodes_.end(); ++iter)
    {
        Node *node = (*iter).second.get();
        if (! node->isLoadBanged())
            node->loadBang();
    }
}

std::vector<Graph::Connection> Graph::getAllConnectedTo(const char *name, const char *inlet)
{
    std::vector<Connection> ret;
    std::vector<Connection>::const_iterator iter;
    for (iter = connections_.begin(); iter != connections_.end(); ++iter)
    {
        if ((*iter).get<2>() == name && 
            (*iter).get<3>() == inlet)
        {
            ret.push_back(*iter);
        }
    }
    return ret;
}

std::vector<Graph::Connection> Graph::getAllConnectedFrom(const char *name, const char *outlet)
{
    std::vector<Connection> ret;
    std::vector<Connection>::const_iterator iter;
    for (iter = connections_.begin(); iter != connections_.end(); ++iter)
    {
        if ((*iter).get<0>() == name && 
            (*iter).get<1>() == outlet)
        {
            ret.push_back(*iter);
        }
    }
}

std::vector<Graph::Connection> Graph::getAllConnectedTo(const char *name)
{
    std::vector<Connection> ret;
    std::vector<Connection>::const_iterator iter;
    for (iter = connections_.begin(); iter != connections_.end(); ++iter)
    {
        if ((*iter).get<2>() == name)
        {
            ret.push_back(*iter);
        }
    }
    return ret;
}

std::vector<Graph::Connection> Graph::getAllConnectedFrom(const char *name)
{
    std::vector<Connection> ret;
    std::vector<Connection>::const_iterator iter;
    for (iter = connections_.begin(); iter != connections_.end(); ++iter)
    {
        if ((*iter).get<0>() == name)
        {
            ret.push_back(*iter);
        }
    }
    return ret;
}


const std::vector<Graph::Connection> Graph::getAllConnections() const
{
    return connections_;
}

bool Graph::deleteNode(const char *name)
{
    Node::ptr node = getNode(name);
    if (node.get() == 0)
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

bool Graph::hasNode(const char *name) const
{
    return getNode(name).get() != 0;
}

// /**
//  * Return true if handled.
//  */
// bool Graph::handleMessage(const Message &message)
// {
//     //std::cout << "Graph::" << __FUNCTION__ << "(" << message << ")" << std::endl;
//     std::string types = message.getTypes();
//     if (utils::stringBeginsWith(types.c_str(), "s")
//         && message.getString(0) == "__tempi__")
//     {
//         //std::cout << __FILE__ << "::" << __FUNCTION__ << ": starts with tempi" << std::endl;
//         return handleTempiMessage(
//             message.cloneRange(1, message.getSize() - 1));
//     }
//     else if (utils::stringBeginsWith(types.c_str(), "s"))
//     {
//         std::string receiveSlot = message.getString(0);
//         std::cout << "TODO: Graph::" << __FUNCTION__ << "(" << message << ")" << std::endl;
//         return false;
//     }
// }
// 
// /**
//  * Handles messages meant to dynamically patch the graph.
//  * - ,ssisi: connect [from] [outlet] [to] [inlet]
//  * - ,sss: addNode [type] [name]
//  * - ,ss: deleteNode [name]
//  * - ,ss...: setNodeAttribute [nodeName] [prop] ...
//  */
// bool Graph::handleTempiMessage(const Message &message)
// {
//     std::string types = message.getTypes();
//     if (utils::stringsMatch(types.c_str(), "ssisi")
//         && message.getString(0) == "connect")
//     {
//         std::string from = message.getString(1);
//         unsigned int outlet = (unsigned) message.getInt(2);
//         std::string to = message.getString(3);
//         unsigned int inlet = (unsigned) message.getInt(4);
//         std::string string0 = message.getString(0);
//         return connect(from.c_str(), outlet,
//             to.c_str(), inlet);
//     }
//     if (utils::stringsMatch(types.c_str(), "sss")
//         && message.getString(0) == "addNode")
//     {
//         std::string type = message.getString(1);
//         std::string name = message.getString(2);
//         bool ok = addNode(type.c_str(), name.c_str());
//         if (ok)
//             std::cout << "did create node " << name << std::endl;
//         return ok;
//     }
//     if (utils::stringsMatch(types.c_str(), "ss")
//         && message.getString(0) == "deleteNode")
//     {
//         std::string name = message.getString(1);
//         return deleteNode(name.c_str());
//     }
//     if (utils::stringBeginsWith(types.c_str(), "sss")
//         && message.getString(0) == "setNodeAttribute")
//     {
//         std::string nodeName = message.getString(1);
//         std::string attributeName = message.getString(2);
//         Message value = message.cloneRange(3, message.getSize() - 1);
//         return setNodeAttribute(nodeName.c_str(),
//             attributeName.c_str(), value);
//     }
//     return false; // unhandled
// }

bool Graph::setNodeAttribute(const char *nodeName, const char *attributeName, const Message &value)
{
    if (! hasNode(nodeName))
        return false;
    Node::ptr nodePtr = getNode(nodeName);
    if (nodePtr.get() == 0)
    {
        std::cerr << "Graph::" << __FUNCTION__ << ": No such node: " << nodeName << std::endl;
        return false;
    }
    try
    {
        nodePtr->setAttribute(attributeName, value);
        return true;
    }
    catch (const BadIndexException &e)
    {
        std::cerr << "BadIndexException in ";
        std::cerr << "Graph::" << __FUNCTION__ << "(" <<
            nodeName << ", " << attributeName << ", " << value
            << "): " << e.what() << std::endl;;
        return false;
    }
    catch (const BadArgumentTypeException &e)
    {
        std::cerr << "BadArgumentTypeException in ";
        std::cerr << "Graph::" << __FUNCTION__ << "(" <<
            nodeName << ", " << attributeName << ", " << value
            << "): " << e.what() << std::endl;;
        return false;
    }
}

std::vector<std::string> Graph::getNodeNames() const
{
    std::vector<std::string> ret;
    NodesMapType::const_iterator iter;
    for (iter = nodes_.begin(); iter != nodes_.end(); ++iter)
    {
        ret.push_back((*iter).first);
    }
    return ret;
}

std::ostream &operator<<(std::ostream &os, const Graph &graph)
{
    os << "Graph:" << std::endl;
    os << " Nodes:" << std::endl;
    std::vector<std::string> nodes = graph.getNodeNames();
    std::vector<std::string>::const_iterator iter;
    for (iter = nodes.begin(); iter != nodes.end(); ++iter)
        os << " * " << (*iter) << " (" << graph.getNode((*iter).c_str())->getTypeName() << ")" << std::endl;

    std::vector<Graph::Connection> connections = graph.getAllConnections();
    std::vector<Graph::Connection>::const_iterator iter2;
    if (connections.size() == 0)
        os << " No Connections." << std::endl;
    else
        os << " Connections:" << std::endl;
    for (iter2 = connections.begin(); iter2 != connections.end(); ++iter2)
    {
        Graph::Connection conn = (*iter2);
        os << " * " << conn.get<0>() << ":" << conn.get<1>() <<
            " ->" << conn.get<2>() << ":" << conn.get<3>() << std::endl;
    }
    return os;
}

void Graph::disconnectMany(ConnectionVec &connections)
{
    ConnectionVec::const_iterator iter;
    for (iter = connections.begin(); iter != connections.end(); ++iter)
    {
        Connection conn = (*iter);
        disconnect(conn.get<0>().c_str(), conn.get<1>().c_str(), conn.get<2>().c_str(), conn.get<3>().c_str());
    }
}

void Graph::disconnectAllConnectedFrom(const char *name, const char *outlet)
{
    ConnectionVec connections = getAllConnectedFrom(name, outlet);
    disconnectMany(connections);
}

void Graph::disconnectAllConnectedFrom(const char *name)
{
    ConnectionVec connections = getAllConnectedFrom(name);
    disconnectMany(connections);
}

void Graph::disconnectAllConnectedTo(const char *name)
{
    ConnectionVec connections = getAllConnectedTo(name);
    disconnectMany(connections);
}

void Graph::disconnectAllConnectedTo(const char *name, const char *inlet)
{
    ConnectionVec connections = getAllConnectedTo(name, inlet);
    disconnectMany(connections);
}

} // end of namespace

