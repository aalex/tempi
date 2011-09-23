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

#include "tempi/nodefactory.h"
#include "tempi/node.h"
#include <iostream>
#include <sstream>

namespace tempi
{

bool NodeFactory::registerType(const char *name, AbstractNodeType *entry)
{
    //std::cout << "NodeFactory::" << __FUNCTION__ << "(" << name << ", " << entry << ")" << std::endl;
    if (hasType(name))
    {
        std::cerr << "NodeFactory::" << __FUNCTION__ << "Already got an entry named " << name << std::endl;
        return false;
    }
    if (entry == 0)
    {
        std::cerr << "NodeFactory::" << __FUNCTION__ << "Null pointer!" << std::endl;
        return false;
    }
    entries_[std::string(name)] = std::tr1::shared_ptr<AbstractNodeType>(entry);
    return true;
}

std::tr1::shared_ptr<Node> NodeFactory::create(const char *name) throw(BadNodeTypeException)
{
    // TODO: throw error if type not found
    if (! hasType(name))
    {
        std::ostringstream os;
        os << "NodeFactory::" << __FUNCTION__ << ": No such node type: " << name;
        throw BadNodeTypeException(os.str().c_str());
    }
    //std::cout << "NodeFactory::" << __FUNCTION__ << "(" << name << ")" << std::endl;
    std::string nameString(name);
    AbstractNodeType *nodeType = entries_[nameString].get();
    return std::tr1::shared_ptr<Node>(nodeType->create());
}

bool NodeFactory::hasType(const char *name)
{
    if (entries_.find(std::string(name)) == entries_.end())
        return false;
    return true;
}

std::ostream &operator<<(std::ostream &os, const NodeFactory &nodeFactory)
{
    os << "NodeFactory:" << std::endl;
    std::map<std::string, std::tr1::shared_ptr<AbstractNodeType> > entries = nodeFactory.getEntries();
    std::map<std::string, std::tr1::shared_ptr<AbstractNodeType> >::iterator iter;
    for (iter = entries.begin(); iter != entries.end(); ++iter)
    {
        os << " * " << (*iter).first << ": " << (*iter).second.get() << std::endl;
    }
    return os;
}

} // end of namespace
