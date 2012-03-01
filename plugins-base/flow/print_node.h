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
 * The PrintNode class.
 */
#ifndef __TEMPI_BASE_PRINTNODE_H__
#define __TEMPI_BASE_PRINTNODE_H__

#include "tempi/node.h"
#include "tempi/message.h"
#include <iostream>

namespace tempi {
namespace base {

/**
 * The PrintNode prints the contents of a Message.
 */
class PrintNode : public Node
{
    public:
        PrintNode();
    protected:
        virtual void processMessage(const char *inlet, const Message &message);
        virtual void onAttributeChanged(const char *name, const Message &value);
};

} // end of namespace
} // end of namespace

#endif // ifndef

