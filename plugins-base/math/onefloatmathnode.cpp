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

#include "plugins-base/math/onefloatmathnode.h"
#include "tempi/utils.h"
#include "tempi/log.h"
#include <iostream>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#include <cmath>

namespace tempi { 
namespace plugins_base {

OneFloatMathNode::OneFloatMathNode() :
    Node()
{
    addInlet("0", "Incoming float.", "", "f");
    addOutlet("0", "Resulting float.");
}

void OneFloatMathNode::processMessage(const char *inlet, const Message &message)
{
    if (message.getTypes() != "f")
    {
        std::ostringstream os;
        os << "OneFloatMathNode." << __FUNCTION__ << "(" << inlet << ", " << message << ")";
        os << ": type should be f";
        Logger::log(ERROR, os);
    }
    float operand = operand = message.getFloat(0);
    Message result("f", this->calculate(operand));
    this->output("0", result);
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

MapNode::MapNode() :
    OneFloatMathNode()
{
    this->setShortDocumentation("Convert a float from a range to another.");
    this->addAttribute(Attribute::ptr(new Attribute("input_range", Message("ff", 0.0f, 1.0f), "Input range to map from.")));
    this->addAttribute(Attribute::ptr(new Attribute("output_range", Message("ff", 0.0f, 1.0f), "Output range to map to.")));
}

float MapNode::calculate(float operand)
{
    Message input = this->getAttributeValue("input_range");
    Message output = this->getAttributeValue("output_range");
    return utils::map_float(operand,
        input.getFloat(0), input.getFloat(1),
        output.getFloat(0), output.getFloat(1));
}

CosNode::CosNode() :
    OneFloatMathNode()
{
    this->setShortDocumentation("Outputs the cosine of what you provide it.");
}

float CosNode::calculate(float operand)
{
    return std::cos(operand);
}

SinNode::SinNode() :
    OneFloatMathNode()
{
    this->setShortDocumentation("Outputs the sine of what you provide it.");
}

float SinNode::calculate(float operand)
{
    return std::sin(operand);
}

} // end of namespace
} // end of namespace

