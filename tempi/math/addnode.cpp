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

#include <iostream>
#include "tempi/math/addnode.h"
#include "tempi/utils.h"

namespace tempi {
namespace math {

AddNode::AddNode() :
    Node()
{
    addOutlet();

    Message operand = Message("f", 0.0f);
    addAttribute("operand", operand);
}

void AddNode::processMessage(unsigned int inlet, const Message &message)
{
    if (message.typesMatch("f"))
    {
        float left_operand = message.getFloat(0);
        float right_operand = getAttribute("operand").getFloat(0);

        Message result("f", left_operand + right_operand);
        //std::cout << "AddNode::" << __FUNCTION__ << ": " << left_operand
        //    << " + " << right_operand << " = " << result << std::endl;
        output(0, result);
    }
    else
        std::cerr << "AddNode::" << __FUNCTION__ << "(): Bad type for message " << message << std::endl;
}

} // end of namespace
} // end of namespace

