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

/**
 * @file
 * The StringJoinNode class.
 */
#ifndef __TEMPI_STRING_JOIN_NODE_H__
#define __TEMPI_STRING_JOIN_NODE_H__

#include "tempi/node.h"

namespace tempi {
namespace plugins_base {

/**
 * Joins atoms to create a string.
 */
class StringJoinNode : public Node
{
    public:
        StringJoinNode();
    private:
        virtual void processMessage(const char *inlet, const Message &message);
        static const char * const ATOMS_INLET;
        static const char * const STRING_OUTLET;
        static const char * const SEPARATOR_ATTR;
};

} // end of namespace
} // end of namespace

#endif // ifndef

