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

#ifndef __TEMPI_GRAPH_H__
#define __TEMPI_GRAPH_H__

#include <boost/any.hpp>
#include <boost/tuple/tuple.hpp>
#include <iostream>
#include <vector>
#include "tempi/outlet.h"
#include "tempi/message.h"
#include "tempi/inlet.h"
#include "tempi/node.h"
#include "tempi/sharedptr.h"
#include "tempi/nodefactory.h"

namespace tempi
{

/**
 * A Graph is a group of Node that are connected and through which messages can flow.
 */
class Graph
{
    public:
        typedef std::tr1::shared_ptr<Graph> ptr;
        /**
         * Tuple containing the name of the source node, outlet name, name of sink node, inlet name.
         */
        typedef boost::tuple<std::string, std::string, std::string, std::string> Connection;
        /**
         * Creates a Graph that can create node with the given NodeFactory.
         */
        Graph(NodeFactory::ptr factory);
        /**
         * Creates a Graph with an empty NodeFactory.
         */
        Graph();
        // TODO: rename to createNode
        /**
         * Creates a Node of a given type name in this Graph.
         */
        bool addNode(const char *type, const char *name);
        /**
         * Sends a message to a given Node inlet in this Graph.
         * @param node Name of the Node.
         * @param inlet Name of the inlet of that Node.
         * @param message Message to send.
         */
        bool message(const char *node, const char *inlet, const Message &message);
        /**
         * This method allows dynamic patching of a Graph.
         * Message not start with "__tempi__" are sent to receive symbols in the Graph. See the tempi::base::ReceiveNode class which allows to receive messages starting by a receive symbol. (selector)
         * 
         * Message starting by the "__tempi__" string are dynamic patching commands. Here is a list of message the Graph can handle.
         * - ,ssisi: connect [from] [outlet] [to] [inlet]
         * - ,sss: addNode [type] [name]
         * - ,ss: deleteNode [name]
         * - ,ss...: setNodeAttribute [nodeName] [prop] ...
         */
        //bool handleMessage(const Message &message);
        /**
         * Connects the outlet of a node in this Graph, to the inlet of another node in this Graph.
         */
        bool connect(const char *from, const char *outlet, const char *to, const char *inlet);
        /**
         * Disconnects the outlet of a node in this Graph, from the inlet of another node in this Graph.
         */
        bool disconnect(const char *from, const char *outlet, const char *to, const char *inlet);
        /**
         * Checks if a given outlet is connected to a given inlet.
         */
        bool isConnected(const char *from, const char *outlet, const char *to, const char *inlet);
        /**
         * Retrieves a Node in this Graph.
         */
        Node::ptr getNode(const char *name) const;
        /**
         * Returns the list of Node names in this Graph.
         */
        std::vector<std::string> getNodeNames() const;
        /**
         * Checks if a given named Node is in this Graph.
         */
        bool hasNode(const char *name) const;
        /**
         * Triggers whatever timed action need to be done in this Graph.
         * Also calls Node::init() for each Node if not called.
         */
        void tick();
        /**
         * Deletes a node after disconnecting all its outlets and inlets.
         */
        bool deleteNode(const char *name);
        /**
         * Returns all connections in this graph.
         */
        const std::vector<Connection> getAllConnections() const; // TODO: const
        /**
         * Sets the value of a given named attribute of a named Node in this Graph.
         */
        bool setNodeAttribute(const char *nodeName, const char *attributeName, const Message &value);
    private:
        typedef std::map<std::string, Node::ptr> NodesMapType;
        typedef std::vector<Connection> ConnectionVec;
        NodeFactory::ptr factory_;
        NodesMapType nodes_;
        void disconnectAllConnectedTo(const char *name);
        void disconnectAllConnectedFrom(const char *name);
        void disconnectAllConnectedTo(const char *name, const char *inlet);
        void disconnectAllConnectedFrom(const char *name, const char *outlet);
        void disconnectMany(ConnectionVec &connections);
        std::vector<Connection> getAllConnectedTo(const char *name, const char *inlet); // TODO: const
        std::vector<Connection> getAllConnectedFrom(const char *name, const char *outlet); // TODO: const
        std::vector<Connection> getAllConnectedTo(const char *name); // TODO: const
        std::vector<Connection> getAllConnectedFrom(const char *name); // TODO: const
        std::vector<Connection> connections_;
        //void onInletTriggered(Outlet *source, boost::any data);
        // TODO: store connections to decrease complexity
        //bool handleTempiMessage(const Message &message);
        void onInletDeleted(const Message &message);
        void onOutletDeleted(const Message &message);
};

// not a good idea:
//std::vector<boost::tuple<std::string, unsigned int, std::string, unsigned int> > getConnections();
//std::map<std::string, std::tr1::shared_ptr<Node> > getNodes() const;

std::ostream &operator<<(std::ostream &os, const Graph &graph);

} // end of namespace

#endif // ifndef

