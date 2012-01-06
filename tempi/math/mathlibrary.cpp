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

#include "tempi/math/mathlibrary.h"
#include "tempi/utils.h"

namespace tempi {
namespace math {

void MathLibrary::load(NodeFactory &factory, const char *prefix) const
{
    using utils::concatenate;
    factory.registerTypeT<AddNode>(concatenate(prefix, "+").c_str());
    factory.registerTypeT<DivNode>(concatenate(prefix, "/").c_str());
    factory.registerTypeT<EqualsNotNode>(concatenate(prefix, "!=").c_str());
    factory.registerTypeT<IsEqualNode>(concatenate(prefix, "==").c_str());
    factory.registerTypeT<IsGreaterNode>(concatenate(prefix, ">").c_str());
    factory.registerTypeT<IsLessNode>(concatenate(prefix, "<").c_str());
    factory.registerTypeT<SubtractNode>(concatenate(prefix, "-").c_str());
    factory.registerTypeT<MultNode>(concatenate(prefix, "*").c_str());
}

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

DivNode::DivNode() :
    Node()
{
    addOutlet();

    Message operand = Message("f", 0.0f);
    addAttribute("operand", operand);
}

void DivNode::processMessage(unsigned int inlet, const Message &message)
{
    if (message.typesMatch("f"))
    {
        float left_operand = message.getFloat(0);
        float right_operand = getAttribute("operand").getFloat(0);

        Message result("f", left_operand / right_operand);
        //std::cout << "DivNode::" << __FUNCTION__ << ": " << left_operand
        //    << " + " << right_operand << " = " << result << std::endl;
        output(0, result);
    }
    else
        std::cerr << "DivNode::" << __FUNCTION__ << "(): Bad type for message " << message << std::endl;
}

EqualsNotNode::EqualsNotNode() :
    Node()
{
    addOutlet();

    Message operand = Message("f", 0.0f);
    addAttribute("operand", operand);
}

void EqualsNotNode::processMessage(unsigned int inlet, const Message &message)
{
    if (message.typesMatch("f"))
    {
        float left_operand = message.getFloat(0);
        float right_operand = getAttribute("operand").getFloat(0);
        Message result("b", left_operand != right_operand);
        output(0, result);
    }
    else
        std::cerr << "SubtractNode::" << __FUNCTION__ << "(): Bad type for message " << message << std::endl;
}

IsGreaterNode::IsGreaterNode() :
    Node()
{
    addOutlet();

    Message operand = Message("f", 0.0f);
    addAttribute("operand", operand);
}

void IsGreaterNode::processMessage(unsigned int inlet, const Message &message)
{
    bool equal = false;
    if (message.typesMatch("f"))
    {
        float left_operand = message.getFloat(0);
        float right_operand = getAttribute("operand").getFloat(0);
        if (left_operand > right_operand)
            equal = true;
        Message result("b", equal);
        //std::cout << "IsGreaterNode::" << __FUNCTION__ << ": " << left_operand
        //    << " + " << right_operand << " = " << result << std::endl;
        output(0, result);
    }
    else
        std::cerr << "IsGreaterNode::" << __FUNCTION__ << "(): Bad type for message " << message << std::endl;
}

IsEqualNode::IsEqualNode() :
    Node()
{
    addOutlet();

    Message operand = Message("f", 0.0f);
    addAttribute("operand", operand);
}

void IsEqualNode::processMessage(unsigned int inlet, const Message &message)
{
    bool equal = false;
    if (message.typesMatch("f"))
    {
        float left_operand = message.getFloat(0);
        float right_operand = getAttribute("operand").getFloat(0);
        if (left_operand == right_operand)
            equal = true;

        Message result("b", equal);
        //std::cout << "IsEqualNode::" << __FUNCTION__ << ": " << left_operand
        //    << " + " << right_operand << " = " << result << std::endl;
        output(0, result);
    }
    else
        std::cerr << "IsEqualNode::" << __FUNCTION__ << "(): Bad type for message " << message << std::endl;
}

IsLessNode::IsLessNode() :
    Node()
{
    addOutlet();

    Message operand = Message("f", 0.0f);
    addAttribute("operand", operand);
}

void IsLessNode::processMessage(unsigned int inlet, const Message &message)
{
    bool equal = false;
    if (message.typesMatch("f"))
    {
        float left_operand = message.getFloat(0);
        float right_operand = getAttribute("operand").getFloat(0);
        if (left_operand < right_operand)
            equal = true;
        Message result("b", equal);
        //std::cout << "IsLessNode::" << __FUNCTION__ << ": " << left_operand
        //    << " + " << right_operand << " = " << result << std::endl;
        output(0, result);
    }
    else
        std::cerr << "IsLessNode::" << __FUNCTION__ << "(): Bad type for message " << message << std::endl;
}

MultNode::MultNode() :
    Node()
{
    addOutlet();

    Message operand = Message("f", 0.0f);
    addAttribute("operand", operand);
}

void MultNode::processMessage(unsigned int inlet, const Message &message)
{
    if (message.typesMatch("f"))
    {
        float left_operand = message.getFloat(0);
        float right_operand = getAttribute("operand").getFloat(0);

        Message result("f", left_operand * right_operand);
        //std::cout << "MultNode::" << __FUNCTION__ << ": " << left_operand
        //    << " + " << right_operand << " = " << result << std::endl;
        output(0, result);
    }
    else
        std::cerr << "MultNode::" << __FUNCTION__ << "(): Bad type for message " << message << std::endl;
}

SubtractNode::SubtractNode() :
    Node()
{
    addOutlet();

    Message operand = Message("f", 0.0f);
    addAttribute("operand", operand);
}

void SubtractNode::processMessage(unsigned int inlet, const Message &message)
{
    if (message.typesMatch("f"))
    {
        float left_operand = message.getFloat(0);
        float right_operand = getAttribute("operand").getFloat(0);

        Message result("f", left_operand - right_operand);
        //std::cout << "AddNode::" << __FUNCTION__ << ": " << left_operand
        //    << " + " << right_operand << " = " << result << std::endl;
        output(0, result);
    }
    else
        std::cerr << "SubtractNode::" << __FUNCTION__ << "(): Bad type for message " << message << std::endl;
}

} // end of namespace
} // end of namespace

