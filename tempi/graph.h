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

#ifndef __TEMPI_GRAPH_H__
#define __TEMPI_GRAPH_H__

#include <boost/any.hpp>
#include <boost/tuple/tuple.hpp>
#include <iostream>
#include <vector>
#include "tempi/source.h"
#include "tempi/message.h"
#include "tempi/sink.h"
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
         * Tuple containing the name of the source node, outlet number, name of sink node, inlet number.
         */
        typedef boost::tuple<std::string, unsigned int, std::string, unsigned int> Connection;
        Graph(NodeFactory::ptr factory);
        Graph();
        bool addNode(const char *type, const char *name);
        bool message(const char *node, unsigned int inlet, const Message &message);
        /**
         * This method allows dynamic patching of a Graph.
         * Message not start with "__tempi__" are sent to receive symbols in the Graph. See the tempi::base::ReceiveNode class which allows to receive messages starting by a receive symbol. (selector)
         * 
         * Message starting by the "__tempi__" string are dynamic patching commands. Here is a list of message the Graph can handle.
         * - ,ssisi: connect [from] [outlet] [to] [inlet]
         * - ,sss: addNode [type] [name]
         * - ,ss: deleteNode [name]
         * - ,ss...: setNodeProperty [nodeName] [prop] ...
         */
        bool handleMessage(const Message &message);
        bool connect(const char *from, unsigned int outlet, const char *to, unsigned int inlet);
        bool disconnect(const char *from, unsigned int outlet, const char *to, unsigned int inlet);
        bool isConnected(const char *from, unsigned int outlet, const char *to, unsigned int inlet);
        Node::ptr getNode(const char *name) const;
        std::vector<std::string> getNodeNames() const;
        bool hasNode(const char *name) const;
        void tick();
        /**
         * Deletes a node after disconnecting all its outlets and inlets.
         */
        bool deleteNode(const char *name);
        /**
         * Returns all connections in this graph.
         */
        std::vector<Connection> getAllConnections(); // TODO: const
        // TODO: store all connections in a vector
        bool setNodeProperty(const char *nodeName, const char *propertyName, const Message &value);
    private:
        typedef std::map<std::string, Node::ptr> NodesMapType;
        typedef std::vector<Connection> ConnectionVec;
        NodeFactory::ptr factory_;
        NodesMapType nodes_;
        void disconnectAllConnectedTo(const char *name);
        void disconnectAllConnectedFrom(const char *name);
        std::vector<Connection> getAllConnectedTo(const char *name, unsigned int inlet); // TODO: const
        std::vector<Connection> getAllConnectedFrom(const char *name, unsigned int inlet); // TODO: const
        std::vector<Connection> getAllConnectedTo(const char *name); // TODO: const
        std::vector<Connection> getAllConnectedFrom(const char *name); // TODO: const
        //void onInletTriggered(Source *source, boost::any data);
        // TODO: store connections to decrease complexity
        bool handleTempiMessage(const Message &message);
};

// not a good idea:
//std::vector<boost::tuple<std::string, unsigned int, std::string, unsigned int> > getConnections();
//std::map<std::string, std::tr1::shared_ptr<Node> > getNodes() const;

std::ostream &operator<<(std::ostream &os, const Graph &graph);

} // end of namespace

#endif // ifndef

