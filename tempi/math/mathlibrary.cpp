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

#include "tempi/math/mathlibrary.h"
#include "tempi/utils.h"
// #include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace tempi {
namespace math {

AddNode::AddNode() :
    Node()
{
    setShortDocumentation("Outputs the addition of two floats together.");
    addInlet("0", "Incoming float.");
    addOutlet("0", "Resulting float.");

    Message operand = Message("f", 0.0f);
    addAttribute(Attribute::ptr(new Attribute("operand", operand)));
}

void AddNode::processMessage(const char *inlet, const Message &message)
{
    if (message.typesMatch("f"))
    {
        float left_operand = message.getFloat(0);
        float right_operand = getAttribute("operand")->getValue().getFloat(0);

        Message result("f", left_operand + right_operand);
        //std::cout << "AddNode::" << __FUNCTION__ << ": " << left_operand
        //    << " + " << right_operand << " = " << result << std::endl;
        output("0", result);
    }
    else
        std::cerr << "AddNode::" << __FUNCTION__ << "(): Bad type for message " << message << std::endl;
}

DivNode::DivNode() :
    Node()
{
    setShortDocumentation("Outputs the division of one float by another.");
    addInlet("0", "Incoming float.");
    addOutlet("0", "Resulting float.");

    Message operand = Message("f", 0.0f);
    addAttribute(Attribute::ptr(new Attribute("operand", operand)));
}

void DivNode::processMessage(const char *inlet, const Message &message)
{
    if (message.typesMatch("f"))
    {
        float left_operand = message.getFloat(0);
        float right_operand = getAttribute("operand")->getValue().getFloat(0);

        if (right_operand == 0.0f)
            output("0", Message("f", 0.0f));
        else
        {
            Message result("f", left_operand / right_operand);
            //std::cout << "DivNode::" << __FUNCTION__ << ": " << left_operand
            //    << " + " << right_operand << " = " << result << std::endl;
            output("0", result);
        }
    }
    else
        std::cerr << "DivNode::" << __FUNCTION__ << "(): Bad type for message " << message << std::endl;
}

EqualsNotNode::EqualsNotNode() :
    Node()
{
    setShortDocumentation("Outputs whether two floats are equal or not.");
    addInlet("0", "Incoming float.");
    addOutlet("0", "Boolean result.");

    Message operand = Message("f", 0.0f);
    addAttribute(Attribute::ptr(new Attribute("operand", operand)));
}

void EqualsNotNode::processMessage(const char *inlet, const Message &message)
{
    if (message.typesMatch("f"))
    {
        float left_operand = message.getFloat(0);
        float right_operand = getAttribute("operand")->getValue().getFloat(0);
        Message result("b", left_operand != right_operand);
        output("0", result);
    }
    else
        std::cerr << "SubtractNode::" << __FUNCTION__ << "(): Bad type for message " << message << std::endl;
}

IsGreaterNode::IsGreaterNode() :
    Node()
{
    setShortDocumentation("Outputs whether the incoming float is greather than another value or not.");
    addInlet("0", "Incoming float.");
    addOutlet("0", "Boolean result.");

    Message operand = Message("f", 0.0f);
    addAttribute(Attribute::ptr(new Attribute("operand", operand)));
}

void IsGreaterNode::processMessage(const char *inlet, const Message &message)
{
    bool equal = false;
    if (message.typesMatch("f"))
    {
        float left_operand = message.getFloat(0);
        float right_operand = getAttribute("operand")->getValue().getFloat(0);
        if (left_operand > right_operand)
            equal = true;
        Message result("b", equal);
        //std::cout << "IsGreaterNode::" << __FUNCTION__ << ": " << left_operand
        //    << " + " << right_operand << " = " << result << std::endl;
        output("0", result);
    }
    else
        std::cerr << "IsGreaterNode::" << __FUNCTION__ << "(): Bad type for message " << message << std::endl;
}

IsEqualNode::IsEqualNode() :
    Node()
{
    setShortDocumentation("Outputs whether the incoming float is equal to another value or not.");
    addInlet("0", "Incoming float.");
    addOutlet("0", "Boolean result.");

    Message operand = Message("f", 0.0f);
    addAttribute(Attribute::ptr(new Attribute("operand", operand)));
}

void IsEqualNode::processMessage(const char *inlet, const Message &message)
{
    bool equal = false;
    if (message.typesMatch("f"))
    {
        float left_operand = message.getFloat(0);
        float right_operand = getAttribute("operand")->getValue().getFloat(0);
        if (left_operand == right_operand)
            equal = true;

        Message result("b", equal);
        //std::cout << "IsEqualNode::" << __FUNCTION__ << ": " << left_operand
        //    << " + " << right_operand << " = " << result << std::endl;
        output("0", result);
    }
    else
        std::cerr << "IsEqualNode::" << __FUNCTION__ << "(): Bad type for message " << message << std::endl;
}

IsLessNode::IsLessNode() :
    Node()
{
    setShortDocumentation("Outputs whether the incoming float is less than another value or not.");
    addInlet("0", "Incoming float.");
    addOutlet("0", "Boolean result.");

    Message operand = Message("f", 0.0f);
    addAttribute(Attribute::ptr(new Attribute("operand", operand)));
}

void IsLessNode::processMessage(const char *inlet, const Message &message)
{
    bool equal = false;
    if (message.typesMatch("f"))
    {
        float left_operand = message.getFloat(0);
        float right_operand = getAttribute("operand")->getValue().getFloat(0);
        if (left_operand < right_operand)
            equal = true;
        Message result("b", equal);
        //std::cout << "IsLessNode::" << __FUNCTION__ << ": " << left_operand
        //    << " + " << right_operand << " = " << result << std::endl;
        output("0", result);
    }
    else
        std::cerr << "IsLessNode::" << __FUNCTION__ << "(): Bad type for message " << message << std::endl;
}

MultNode::MultNode() :
    Node()
{
    setShortDocumentation("Outputs the multiplication of an incoming float and another float.");
    addInlet("0", "Incoming float.");
    addOutlet("0", "Resulting float.");

    Message operand = Message("f", 0.0f);
    addAttribute(Attribute::ptr(new Attribute("operand", operand, "The right operand in the operation.")));
}

void MultNode::processMessage(const char *inlet, const Message &message)
{
    if (message.typesMatch("f"))
    {
        float left_operand = message.getFloat(0);
        float right_operand = getAttribute("operand")->getValue().getFloat(0);

        Message result("f", left_operand * right_operand);
        //std::cout << "MultNode::" << __FUNCTION__ << ": " << left_operand
        //    << " + " << right_operand << " = " << result << std::endl;
        output("0", result);
    }
    else
        std::cerr << "MultNode::" << __FUNCTION__ << "(): Bad type for message " << message << std::endl;
}

SubtractNode::SubtractNode() :
    Node()
{
    setShortDocumentation("Outputs the subtraction of an incoming float and another float.");
    addInlet("0", "Incoming float.");
    addOutlet("0", "Resulting float.");

    Message operand = Message("f", 0.0f);
    addAttribute(Attribute::ptr(new Attribute("operand", operand, "The right operand in the operation.")));
}

void SubtractNode::processMessage(const char *inlet, const Message &message)
{
    if (message.typesMatch("f"))
    {
        float left_operand = message.getFloat(0);
        float right_operand = getAttribute("operand")->getValue().getFloat(0);

        Message result("f", left_operand - right_operand);
        //std::cout << "AddNode::" << __FUNCTION__ << ": " << left_operand
        //    << " + " << right_operand << " = " << result << std::endl;
        output("0", result);
    }
    else
        std::cerr << "SubtractNode::" << __FUNCTION__ << "(): Bad type for message " << message << std::endl;
}

class DegToRadNode : public Node
{
    public:
        DegToRadNode() :
            Node()
        {
            addInlet("0");
            addOutlet("0");
            this->setShortDocumentation("Convert degrees to radians.");
        }
    protected:
        void processMessage(const char *inlet, const Message &message)
        {
            if (message.typesMatch("f"))
            {
                output("0", Message("f", message.getFloat(0) * (M_PI / 180.0f)));
            }
            else
                std::cerr << "DegToRadNode::" << __FUNCTION__ << "(): Bad type for message " << message << std::endl;
        }
};

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
    factory.registerTypeT<DegToRadNode>(concatenate(prefix, "deg2rad").c_str());
}

} // end of namespace
} // end of namespace

