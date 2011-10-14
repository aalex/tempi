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
        Graph(NodeFactory::ptr factory);
        Graph();
        bool addNode(Node::ptr node, const char *name);
        bool addNode(const char *type, const char *name);
        // TODO: bool addNode(const char *type);
        bool message(const char *node, unsigned int inlet, const Message &message);
        bool connect(const char *from, unsigned int outlet, const char *to, unsigned int inlet);
        bool disconnect(const char *from, unsigned int outlet, const char *to, unsigned int inlet);
        bool isConnected(const char *from, unsigned int outlet, const char *to, unsigned int inlet);
        Node *getNode(const char *name) const;
        void tick();
    private:
        NodeFactory::ptr factory_;
        std::map<std::string, Node::ptr> nodes_;
        //void onInletTriggered(Source *source, boost::any data);
};

// not a good idea:
//std::vector<boost::tuple<std::string, unsigned int, std::string, unsigned int> > getConnections();
//std::map<std::string, std::tr1::shared_ptr<Node> > getNodes() const;

} // end of namespace

#endif // ifndef

