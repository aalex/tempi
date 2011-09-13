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
#include "tempi/sink.h"
#include "tempi/sharedptr.h"

namespace tempi
{

/**
 * A Graph is a group of Node that are connected and through which messages can flow.
 */
class Graph
{
    public:
        Graph();
        std::map<std::string, std::tr1::shared_ptr<Node> > getNodes();
        bool addNode(std::string &name, std::tr1::shared_ptr<Node> node);
        bool addSource(std::string &name);
        bool addSink(std::string &name);
        bool addFilter(std::string &name);
        bool message(std::string &node, boost::any &message);
        bool connect(std::string &from, std::string &to);
        Sink *getSink(std::string &name) const;
        Node *getNode(std::string &name) const;
    private:
        std::map<std::string, std::tr1::shared_ptr<Node> > nodes_;
        //void onInletTriggered(Source *source, boost::any data);
};

} // end of namespace

#endif // ifndef

