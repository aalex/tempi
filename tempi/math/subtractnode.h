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
 * @file
 * The SubtractNode class.
 */
#ifndef __TEMPI_MATH_SUBNODE_H__
#define __TEMPI_MATH_SUBNODE_H__

#include "tempi/node.h"
#include "tempi/message.h"
#include <iostream>

namespace tempi {
namespace math {

/**
 * The SubtractNode subtracts one float from another
 */
class SubtractNode : public Node
{
    public:
        SubtractNode();
    protected:
        virtual void processMessage(unsigned int inlet, const Message &message);
};

} // end of namespace
} // end of namespace

#endif // ifndef

