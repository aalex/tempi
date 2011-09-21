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

#ifndef __TEMPI_NODEREGISTRY_H__
#define __TEMPI_NODEREGISTRY_H__

#include <boost/signals2.hpp>
#include <tr1/memory>
#include <map>

namespace tempi
{

/**
 * A NodeRegistry store the type info about the Node classes available.
 */
class NodeRegistry
{
    public:
        NodeRegistry();
        // Node types:
        bool addNodeType(std::string &name, type_info *type);
        std::map<std::string, type_info> getNodeTypes();
        type_info getNodeType(std::string &name);

        // Property types:
        std::map<std::string name, Property> getProperties();
        std::map<std::string name, type_info> getPropertyTypes();
        type_info getPropertyType(std::string &name);
    private:
        std::map<std::string name, *type_info> node_types_;
};

} // end of namespace

#endif // ifndef

