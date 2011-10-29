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
 * The NodeType and AbstractNodeType classes.
 */

#ifndef __TEMPI_NODETYPE_H__
#define __TEMPI_NODETYPE_H__

#include "tempi/node.h"
#include "tempi/sharedptr.h"

namespace tempi
{

/**
 * An entry in the NodeFactory.
 */
class AbstractNodeType
{
    public:
        typedef std::tr1::shared_ptr<AbstractNodeType> ptr;
        virtual Node *create() = 0;
};

/**
 * An entry in the NodeFactory, specific to a single Node type.
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

} // end of namespace

#endif // ifndef

