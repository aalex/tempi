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

#include <boost/any.hpp>
#include <boost/bind.hpp>
#include "tempi/filter.h"
#include "tempi/node.h"
#include "tempi/source.h"
#include "tempi/sink.h"
#include "tempi/filter.h"
#include "tempi/sharedptr.h"
#include <iostream>

namespace tempi
{

Graph::Graph()
{
}

Graph::Graph()
{
}
std::map<std::string, std::tr1::shared_ptr<Node> > Graph::getNodes()
{
    return nodes_;
}
bool Graph::addNode(std::string &name, std::tr1::shared_ptr<Node> node)
{
    if (getNode(name) != 0)
    {
        std::cout << "Graph::" << __FUNCTION__ << ": There is already a node with ID " << name << std::endl;
        return false;
    }
    nodes_[name] = node;
    return true;
}
bool Graph::addSource(std::string &name)
{
}
bool Graph::addSink(std::string &name)
{
}
bool Graph::addFilter(std::string &name)
{
}
bool Graph::message(std::string &node, boost::any &message)
{
}
bool Graph::connect(std::string &from, std::string &to)
{
}
Sink *Graph::getSink(std::string &name) const
{
}
Node *Graph::getNode(std::string &name) const
{
}


} // end of namespace

