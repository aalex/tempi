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

/**
 * The NodeFactory, NodeType and AbstractNodeType classes.
 */

#ifndef __TEMPI_NODEFACTORY_H__
#define __TEMPI_NODEFACTORY_H__

#include <map>
#include <string>
#include <sstream>
#include <tr1/memory>
#include "tempi/exceptions.h"
#include "tempi/node.h"
#include "tempi/sharedptr.h"

namespace tempi
{

/**
 * An entry in the registry.
 */
class AbstractNodeType
{
    public:
        typedef std::tr1::shared_ptr<AbstractNodeType> ptr;
        virtual Node *create() = 0;
};

/**
 * An entry in the registry, specific to a single Node type.
 */
template <class T>
class NodeType
{
    public:
        virtual Node *create()
        {
            return new T();
        }
};

/**
 * A NodeFactory stores stores factories for Node types.
 * see http://stackoverflow.com/questions/2850213/dynamically-register-constructor-methods-in-an-abstractfactory-at-compile-time-us
 */
class NodeFactory
{
    public:
        typedef std::tr1::shared_ptr<NodeFactory> ptr;
        /**
         * Registers a node type.
         * Returns false if it failed.
         */
        bool registerType(const char *name, AbstractNodeType::ptr entry);
        /**
         * Checks for a given node type.
         */
        bool hasType(const char *name);
        Node::ptr create(const char *name) throw(BadNodeTypeException);
        // TODO: std::map<std::string name, Property> getProperties();
        // TODO: std::map<std::string name, type_info> getPropertyTypes();
        // TODO: type_info getPropertyType(std::string &name);

        std::map<std::string, AbstractNodeType::ptr> getEntries() const
        {
            return entries_;
        }
    private:
        std::map<std::string, AbstractNodeType::ptr> entries_;
};

std::ostream &operator<<(std::ostream &os, const NodeFactory &nodeFactory);

} // end of namespace

#endif // ifndef

