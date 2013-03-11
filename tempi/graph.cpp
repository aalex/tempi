/*
 * Copyright (C) 2011 Alexandre Quessy
 *
 * This file is part of Tempi.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software ither version 3 of the License, or
 * (at your option) any later version.
 * 
 * Tempi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with Tempi.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

#include "tempi/graph.h"
#include "tempi/utils.h"
#include "tempi/log.h"
#include <iostream>
#include <ostream>
#include <sstream>

namespace tempi
{

static std::string connectionVecToString(const std::vector<Graph::Connection> &vec)
{
    std::ostringstream os;
    os << "[";
    std::vector<Graph::Connection>::const_iterator iter;
    for (iter = vec.begin(); iter != vec.end(); ++iter)
    {
        os << "\"";
        os << (*iter).get<0>();
        os << ":";
        os << (*iter).get<1>();
        os << "->";
        os << (*iter).get<2>();
        os << ":";
        os << (*iter).get<3>();
        os << "\"";
        os << ", ";
    }
    os << "]";
    return os.str();
}

static std::vector<Graph::Connection>::iterator find_connection(std::vector<Graph::Connection> &connections, const char *from, const char *outlet, const char *to, const char *inlet);

Graph::Graph(NodeFactory::ptr factory) :
    factory_(factory)
{
    this->message_depth_counter_ = 0;
}

Graph::Graph()
{
    this->message_depth_counter_ = 0;
}

bool Graph::addNode(const char *type, const char *name)
{
    if (factory_.get() == 0)
    {
        std::ostringstream os;
        os << "Graph." << __FUNCTION__ << ": this NodeFactory is an Invalid pointer.";
        Logger::log(ERROR, os);
        return false;
    }
    if (factory_->hasType(type))
    {
        //os << "Graph." << __FUNCTION__ << ": NodeFactory does have type " << type;
        if (getNode(name).get() != 0)
        {
            std::ostringstream os;
            os << "Graph." << __FUNCTION__ << ": There is already a node with ID " << name;
            Logger::log(ERROR, os);
            return false;
        }
        Node::ptr node = factory_->create(type);
        if (node.get() == 0)
        {
            std::ostringstream os;
            os << "Graph." << __FUNCTION__ << ": Invalid pointer to Node.";
            Logger::log(ERROR, os);
            return false;
        }
        node->setName(name);
        node->setGraph(this);
        nodes_[name] = node;
        if (Logger::isEnabledFor(DEBUG))
        {
            std::ostringstream os;
            os << "Graph." << __FUNCTION__ << "(\"" << type << "\", \"" << name << "\")";
            Logger::log(DEBUG, os);
        }

        try
        {
            //std::cout << "Graph.addNode: node->getSignal()\n";
            node->getSignal(Node::INLET_DELETED_SIGNAL)->getSignal().connect(
                boost::bind(&Graph::onInletDeleted, this, _1));
            node->getSignal(Node::OUTLET_DELETED_SIGNAL)->getSignal().connect(
                boost::bind(&Graph::onOutletDeleted, this, _1));
        }
        catch (const BadIndexException &e)
        {
            // XXX should not occur!!
            std::ostringstream os;
            os << "Graph::" << __FUNCTION__ << ": ";
            os << e.what();
            Logger::log(ERROR, os);
        }
        return true;
    }
    else
    {
        std::ostringstream os;
        os << "Graph." << __FUNCTION__ << ": This NodeFactory doesn't have type " << type;
        //os << "Graph::" << __FUNCTION__ << ": Look:" << std::endl;
        //os << "Graph::" << __FUNCTION__ << ": " << *factory_.get();
        Logger::log(ERROR, os);
        return false; // FIXME
    }
}

// called BEFORE the pad is actually deleted
void Graph::onInletDeleted(const Message &message)
{
    std::string node = message.getString(0);
    std::string inlet = message.getString(1);
    ConnectionVec connections = getAllConnectedTo(node.c_str(), inlet.c_str());
    disconnectMany(connections);
}

// called BEFORE the pad is actually deleted
void Graph::onOutletDeleted(const Message &message)
{
    if (Logger::isEnabledFor(INFO))
    {
        std::ostringstream os;
        os << "Graph." << __FUNCTION__ << "(" << message << ")";
        Logger::log(INFO, os);
    }
    std::string node = message.getString(0);
    std::string outlet = message.getString(1);
    if (Logger::isEnabledFor(INFO))
    {
        std::ostringstream os;
        os << "Calling getAllConnectedFrom(" << node << ", " << outlet << ")";
        Logger::log(INFO, os);
    }
    ConnectionVec connections = getAllConnectedFrom(node.c_str(), outlet.c_str());
    if (Logger::isEnabledFor(INFO))
    {
        std::ostringstream os;
        os << "Calling disconnectMany(";
        os << connectionVecToString(connections);
        os << ")";
        Logger::log(INFO, os);
    }
    disconnectMany(connections);
}

bool Graph::message(const char *node, const char *inlet, const Message &message)
{
    Node::ptr nodePtr = getNode(node);
    if (nodePtr.get() == 0)
    {
        std::ostringstream os;
        os << "Graph." << __FUNCTION__ << ": No such node: " << node;
        Logger::log(ERROR, os);
        return false;
    }
    else if (inlet == 0)
    {
        std::ostringstream os;
        os << "Graph." << __FUNCTION__ << ": Null inlet string name ! ";
        Logger::log(ERROR, os);
        return false;
    }
    if (Logger::isEnabledFor(DEBUG))
    {
        std::ostringstream os;
        os << "Graph.";
        os << __FUNCTION__ << "(" << node << ", " << inlet << ", " << message << ")";
        Logger::log(DEBUG, os);
    }
    return nodePtr->message(inlet, message);
}

bool Graph::connect(const char *from, const char *outlet, const char *to, const char *inlet)
{
    Node::ptr fromNode = getNode(from);
    if (fromNode.get() == 0)
    {
        std::ostringstream os;
        os << "Graph." << __FUNCTION__ << ": Cannot find node \"" << from << "\".";
        Logger::log(ERROR, os);
        return false;
    }
    Node::ptr toNode = getNode(to);
    if (toNode.get() == 0)
    {
        std::ostringstream os;
        os << "Graph." << __FUNCTION__ << ": Cannot find node \"" << to << "\".";
        Logger::log(ERROR, os);
        return false;
    }

    if (inlet == 0 || outlet == 0)
    {
        std::ostringstream os;
        os << "Graph." << __FUNCTION__ << ": Null inlet/outlet!!";
        Logger::log(ERROR, os);
        return false;
    }
    if (! fromNode->hasOutlet(outlet))
    {
        std::ostringstream os;
        os << "Graph." << __FUNCTION__ << ": Outlet " << outlet << " not found in " << from << ".";
        Logger::log(ERROR, os);
        return false;
    }
    if (! toNode->hasInlet(inlet))
    {
        std::ostringstream os;
        os << "Graph." << __FUNCTION__ << ": Inlet " << inlet << " not found in " << to << ".";
        Logger::log(ERROR, os);
        return false;
    }
    if (this->isConnected(from, outlet, to, inlet))
    {
        std::ostringstream os;
        os << "Graph." << __FUNCTION__ << ": Already connected " <<
            from << ":" << outlet << "->" <<
            to << ":" << inlet;
        Logger::log(ERROR, os);
        return false;
    }
    try
    {
        toNode->getInlet(inlet)->connect(fromNode->getOutletSharedPtr(outlet));
    }
    catch (const BadIndexException &e)
    {
        std::ostringstream os;
        os << "Graph." << __FUNCTION__ << ": " << e.what();
        Logger::log(ERROR, os);
        return false;
    }
    connections_.push_back(Connection(std::string(from), std::string(outlet), std::string(to), std::string(inlet)));
    if (Logger::isEnabledFor(DEBUG))
    {
        std::ostringstream os;
        os << "Graph." << __FUNCTION__ << "(" <<
            from << ":" << outlet << ", " << to << ":" << inlet << ")";
        Logger::log(DEBUG, os);
    }
    return true;
}

bool Graph::isConnected(const char *from, const char *outlet, const char *to, const char *inlet)
{
    if (Logger::isEnabledFor(DEBUG))
    {
        std::ostringstream os;
        os << "Graph." << __FUNCTION__ << "(" <<
            from << ":" << outlet << ", " << to << ":" << inlet << ")";
        Logger::log(DEBUG, os);
    }
    if (! hasNode(from))
    {
        std::ostringstream os;
        os << "Graph." << __FUNCTION__ << ": Cannot find node \"" << from << "\".";
        Logger::log(ERROR, os);
        return false;
    }
    if (! hasNode(to))
    {
        std::ostringstream os;
        os << "Graph." << __FUNCTION__ << ": Cannot find node \"" << to << "\".";
        Logger::log(ERROR, os);
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

        bool success = inletPtr->disconnect(source);

        //for (iter = connections_.begin(); iter != connections_.end(); ++iter)
        //{
        //}
        //
        //
        std::vector<Connection>::iterator iter = find_connection(this->connections_, from, outlet, to, inlet);
        if (iter == this->connections_.end())
        {
            // item not anymore in vector!
        }
        else
        {
            this->connections_.erase(iter);
        }
        return success;
    }
    else
        return false;
}

std::vector<Graph::Connection>::iterator find_connection(std::vector<Graph::Connection> &connections, const char *from, const char *outlet, const char *to, const char *inlet)
{
    std::vector<Graph::Connection>::iterator iter;
    for (iter = connections.begin(); iter != connections.end(); ++iter)
    {
        if ((*iter).get<0>() == from && 
            (*iter).get<1>() == outlet && 
            (*iter).get<2>() == to && 
            (*iter).get<3>() == inlet)
        {
            return iter;
        }
    }
    return connections.end();
}


Node::ptr Graph::getNode(const char *name) const
{
    if (name == 0)
    {
        std::ostringstream os;
        os << "Graph.getNode() :: null string!\n";
        Logger::log(ERROR, os.str().c_str());
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
    if (Logger::isEnabledFor(DEBUG))
    {
        std::ostringstream os;
        os << "Graph." << __FUNCTION__ << "(" << name << ", " << outlet << ")";
        Logger::log(DEBUG, os);
    }
    std::vector<Connection> ret;
    std::vector<Connection>::const_iterator iter;
    for (iter = connections_.begin(); iter != connections_.end(); ++iter)
    {
        if ((*iter).get<0>() == name && 
            (*iter).get<1>() == outlet)
        {
            ret.push_back(*iter);
            if (Logger::isEnabledFor(DEBUG))
            {
                std::ostringstream os;
                os << "Graph." << __FUNCTION__ << "(" << name << ", " << outlet << ")";
                os << " ret.push_back(";
                os << (*iter).get<0>() << ",";
                os << (*iter).get<1>() << ",";
                os << (*iter).get<2>() << ",";
                os << (*iter).get<3>();
                os << ")";
                Logger::log(DEBUG, os);
            }
        }
    }
    return ret;
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

            if (Logger::isEnabledFor(DEBUG))
            {
                std::ostringstream os;
                os << "Graph." << __FUNCTION__ << "(" << name << ")";
                os << " : Found ";
                os << (*iter).get<0>() << ",";
                os << (*iter).get<1>() << ",";
                os << (*iter).get<2>() << ",";
                os << (*iter).get<3>();
                Logger::log(DEBUG, os);
            }
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
        std::ostringstream os;
        os << "Graph." << __FUNCTION__ << ": Cannot find node " << name << ".";
        Logger::log(ERROR, os);
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

bool Graph::setNodeAttribute(const char *nodeName, const char *attributeName, const Message &value)
{
    if (! hasNode(nodeName))
        return false;
    Node::ptr nodePtr = getNode(nodeName);
    if (nodePtr.get() == 0)
    {
        std::ostringstream os;
        os << "Graph." << __FUNCTION__ << ": No such node: " << nodeName;
        Logger::log(ERROR, os);
        return false;
    }
    try
    {
        nodePtr->setAttributeValue(attributeName, value);
        return true;
    }
    catch (const BadIndexException &e)
    {
        std::ostringstream os;
        os << "BadIndexException in ";
        os << "Graph." << __FUNCTION__ << "(" <<
            nodeName << ", " << attributeName << ", " << value
            << "): " << e.what();
        Logger::log(ERROR, os);
        return false;
    }
    catch (const BadAtomTypeException &e)
    {
        std::ostringstream os;
        os << "BadAtomTypeException in ";
        os << "Graph." << __FUNCTION__ << "(" <<
            nodeName << ", " << attributeName << ", " << value
            << "): " << e.what();
        Logger::log(ERROR, os);
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
    if (Logger::isEnabledFor(DEBUG))
    {
        std::ostringstream os;
        os << "Graph." << __FUNCTION__ << "(";
        os << "disconnectMany(";
        os << connectionVecToString(connections);
        os << ")";
        Logger::log(DEBUG, os);
    }

    ConnectionVec::const_iterator iter;
    for (iter = connections.begin(); iter != connections.end(); ++iter)
    {
        Connection conn = (*iter);

        if (Logger::isEnabledFor(DEBUG))
        {
            std::ostringstream os;
            os << "Graph." << __FUNCTION__ << "(";
            os << "disconnect(";
            os << conn.get<0>() << ",";
            os << conn.get<1>() << ",";
            os << conn.get<2>() << ",";
            os << conn.get<3>();
            os << ")";
            Logger::log(DEBUG, os);
        }
        this->disconnect(conn.get<0>().c_str(), conn.get<1>().c_str(), conn.get<2>().c_str(), conn.get<3>().c_str());
    }
}

void Graph::disconnectAllConnectedFrom(const char *name, const char *outlet)
{
    ConnectionVec connections = this->getAllConnectedFrom(name, outlet);
    this->disconnectMany(connections);
}

void Graph::disconnectAllConnectedFrom(const char *name)
{
    ConnectionVec connections = this->getAllConnectedFrom(name);
    this->disconnectMany(connections);
}

void Graph::disconnectAllConnectedTo(const char *name)
{
    ConnectionVec connections = this->getAllConnectedTo(name);
    this->disconnectMany(connections);
}

void Graph::disconnectAllConnectedTo(const char *name, const char *inlet)
{
    ConnectionVec connections = this->getAllConnectedTo(name, inlet);
    this->disconnectMany(connections);
}

void Graph::setScheduler(Scheduler *scheduler)
{
    scheduler_ = scheduler;
}

Scheduler *Graph::getScheduler() const
{
    return scheduler_;
}

void Graph::pushMessageDepth()
    throw (BadIndexException)
{
    static const int MAX_MESSAGE_DEPTH = 1000;
    if (this->message_depth_counter_ >= MAX_MESSAGE_DEPTH)
    {
        std::ostringstream os;
        os << "Reached maximum message depth! Be careful when connecting a node's outlet into one of its inlets.";
        throw BadIndexException(os.str().c_str());
    }
    else
        this->message_depth_counter_ += 1;
}

void Graph::popMessageDepth()
    throw (BadIndexException)
{
    if (this->message_depth_counter_ <= 0)
    {
        std::ostringstream os;
        os << "Reached 0 message depth! Some programmer made a mistake.";
        throw BadIndexException(os.str().c_str());
    }
    else
        this->message_depth_counter_ -= 1;
}

} // end of namespace

