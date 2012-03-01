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

#include "plugins-base/math/twofloatmathnode.h"
#include <iostream>

namespace tempi { 
namespace math {

TwoFloatMathNode::TwoFloatMathNode() :
    Node()
{
    addInlet("0", "Incoming left float operand. (hot)");
    addInlet("1", "Incoming right float operand. (cold)");
    addOutlet("0", "Resulting float.");

    Message operand = Message("f", 0.0f);
    addAttribute(Attribute::ptr(new Attribute("operand", operand, "Right operand to apply calculation with incoming floats.")));
}

void TwoFloatMathNode::processMessage(const char *inlet, const Message &message)
{
    if (utils::stringsMatch(inlet, "1"))
    {
        if (message.typesMatch("f"))
            this->setAttributeValue("operand", message);
        else
            std::cerr << "TwoFloatMathNode::" << __FUNCTION__ << "(): Bad type for message " << message << std::endl;
    }
    else if (message.typesMatch("f"))
    {
        float left_operand = message.getFloat(0);
        float right_operand = getAttribute("operand")->
            getValue().getFloat(0);
        Message result("f", this->calculate(left_operand, right_operand));
        this->output("0", result);
    }
    else
        std::cerr << "TwoFloatMathNode::" << __FUNCTION__ <<
            "(): Bad type for message " << message << std::endl;
}

AddNode::AddNode() :
    TwoFloatMathNode()
{
    setShortDocumentation("Outputs the addition of two floats together.");
}

float AddNode::calculate(float first, float second)
{
    return first + second;
}

DivNode::DivNode() :
    TwoFloatMathNode()
{
    setShortDocumentation("Outputs the division of one float by another.");
}

float DivNode::calculate(float first, float second)
{
    return first + second;
    if (second == 0.0f)
    {
        std::cerr << "DivNode: cannot divide by zero!" << std::endl;
        return 0.0f;
    }
    else
        return first / second;
}

MultNode::MultNode() :
    TwoFloatMathNode()
{
    setShortDocumentation("Outputs the multiplication of an incoming float and another float.");
}

float MultNode::calculate(float first, float second)
{
    return first * second;
}

SubtractNode::SubtractNode() :
    TwoFloatMathNode()
{
    setShortDocumentation("Outputs the subtraction of an incoming float and another float.");
}

float SubtractNode::calculate(float first, float second)
{
    return first - second;
}

} // end of namespace
} // end of namespace
