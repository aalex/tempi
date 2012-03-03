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

#include "plugins-base/math/onefloatmathnode.h"
#include <iostream>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace tempi { 
namespace plugins_base {

OneFloatMathNode::OneFloatMathNode() :
    Node()
{
    addInlet("0", "Incoming float.");
    addOutlet("0", "Resulting float.");
}

void OneFloatMathNode::processMessage(const char *inlet, const Message &message)
{
    if (message.typesMatch("f"))
    {
        float operand = message.getFloat(0);
        Message result("f", this->calculate(operand));
        this->output("0", result);
    }
    else
        std::cerr << "OneFloatMathNode::" << __FUNCTION__ <<
            "(): Bad type for message " << message << std::endl;
}

DegToRadNode::DegToRadNode() :
    OneFloatMathNode()
{
    this->setShortDocumentation("Convert degrees to radians.");
}

float DegToRadNode::calculate(float operand)
{
    return operand * (M_PI / 180.0f);
}

} // end of namespace
} // end of namespace

