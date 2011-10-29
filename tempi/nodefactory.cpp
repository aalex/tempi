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

bool NodeFactory::registerType(const char *name, AbstractNodeType::ptr entry)
{
    //std::cout << "NodeFactory::" << __FUNCTION__ << "(" << name << ", " << entry << ")" << std::endl;
    if (hasType(name))
    {
        std::cerr << "NodeFactory::" << __FUNCTION__ << "Already got an entry named " << name << std::endl;
        return false;
    }
    if (entry.get() == 0)
    {
        std::cerr << "NodeFactory::" << __FUNCTION__ << ": " << name << " is a " << "null pointer!" << std::endl;
        return false;
    }
    entries_[std::string(name)] = entry;
    return true;
}

Node::ptr NodeFactory::create(const char *name) throw(BadNodeTypeException)
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
    return Node::ptr(nodeType->create());
}

bool NodeFactory::hasType(const char *name)
{
    bool ret = false;
    std::string nameStr(name);
    //if (entries_.find(nameStr) != entries_.end())
    //    ret = true;
    //std::cout << "NodeFactory::" << __FUNCTION__ << "(" << name << ") ..." << std::endl;
    std::map<std::string, AbstractNodeType::ptr>::iterator iter;
    for (iter = entries_.begin(); iter != entries_.end(); ++iter)
    {
        //std::cout << " * " << (*iter).first << ": " << (*iter).second.get() << std::endl;
        if ((*iter).first == name)
        {
            //std::cout << "   MATCHES!" << std::endl;
            ret = true;
            break;
        }
    }
    //std::cout << "   return " << (ret ? "true" : "false") << std::endl;
    return ret;
}

std::ostream &operator<<(std::ostream &os, const NodeFactory &nodeFactory)
{
    os << "NodeFactory:" << std::endl;
    std::map<std::string, AbstractNodeType::ptr> entries = nodeFactory.getEntries();
    std::map<std::string, AbstractNodeType::ptr>::iterator iter;
    for (iter = entries.begin(); iter != entries.end(); ++iter)
    {
        os << " * " << (*iter).first << ": " << (*iter).second.get() << std::endl;
    }
    return os;
}

} // end of namespace

