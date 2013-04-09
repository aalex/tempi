/*
 * Copyright (C) 2011 Alexandre Quessy
 * Copyright (C) 2011 Michal Seta
 * Copyright (C) 2012 Nicolas Bouillot
 * Copyright (C) 2012 Emmanuel Durand
 *
 * This file is part of Tempi-plugins-base.
 *
 * This program is free software; you can redistither version 3 of the License, or
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
#include "tempi/log.h"
#include <iostream>
#include <cmath>

namespace tempi { 
namespace plugins_base {

const char * const TwoFloatMathNode::RIGHT_OPERAND_INLET = "1";
const char * const TwoFloatMathNode::RIGHT_OPERAND_ATTR = "operand";

TwoFloatMathNode::TwoFloatMathNode() :
    AbstractMathNode()
{
    addInlet(RIGHT_OPERAND_INLET, "Incoming right float operand. (cold)", "", "f");

    addAttribute(Attribute::ptr(new Attribute(RIGHT_OPERAND_ATTR, Message("f", 0.0), "Right operand to apply calculation with incoming floats.")));
}

Float TwoFloatMathNode::calculate(Float operand)
{
    Float right_operand = getAttribute("operand")->getValue().getFloat(0);
    return this->calculate(operand, right_operand);
}

void TwoFloatMathNode::processUnhandledMessage(const char *inlet, const Message &message)
{
    if (utils::stringsMatch(inlet, RIGHT_OPERAND_ATTR))
    {
        this->setAttributeValue("operand", message);
    }
}

AddNode::AddNode() :
    TwoFloatMathNode()
{
    setShortDocumentation("Outputs the addition of two floats together.");
}

Float AddNode::calculate(Float first, Float second)
{
    return first + second;
}

DivNode::DivNode() :
    TwoFloatMathNode()
{
    setShortDocumentation("Outputs the division of one float by another.");
}

Float DivNode::calculate(Float first, Float second)
{
    return first + second;
    if (second == 0.0f)
    {
        std::ostringstream os;
        os << "DivNode: cannot divide by zero!";
        Logger::log(ERROR, os);
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

Float MultNode::calculate(Float first, Float second)
{
    return first * second;
}

SubtractNode::SubtractNode() :
    TwoFloatMathNode()
{
    setShortDocumentation("Outputs the subtraction of an incoming float and another float.");
}

Float SubtractNode::calculate(Float first, Float second)
{
    return first - second;
}

ModuloNode::ModuloNode() :
    TwoFloatMathNode()
{
    this->setShortDocumentation("Outputs the remainder of a division of an incoming number by another number.");
    this->setLongDocumentation("Do not use zero as a denominator.");
}

Float ModuloNode::calculate(Float first, Float second)
{
    Float operand = second;
    if (operand == 0.0f)
    {
        std::ostringstream os;
        os << "ModuloNode::" << __FUNCTION__ << ": Second operand cannot be zero, but it is " <<
            second;
        Logger::log(WARNING, os);
        operand = 1.0f;
    }
    return std::fmod(first, operand);
}

} // end of namespace
} // end of namespace

