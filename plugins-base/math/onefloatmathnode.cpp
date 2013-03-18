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
    AbstractMathNode()
{
}

DegToRadNode::DegToRadNode() :
    OneFloatMathNode()
{
    this->setShortDocumentation("Convert degrees to radians.");
}

Float DegToRadNode::calculate(Float operand)
{
    return operand * (M_PI / 180.0f);
}

MapNode::MapNode() :
    OneFloatMathNode()
{
    this->setShortDocumentation("Convert a float from a range to another.");
    this->addAttribute(Attribute::ptr(new Attribute("input_range", Message("ff", 0.0, 1.0), "Input range to map from.")));
    this->addAttribute(Attribute::ptr(new Attribute("output_range", Message("ff", 0.0, 1.0), "Output range to map to.")));
}

Float MapNode::calculate(Float operand)
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

Float CosNode::calculate(Float operand)
{
    return std::cos(operand);
}

SinNode::SinNode() :
    OneFloatMathNode()
{
    this->setShortDocumentation("Outputs the sine of what you provide it.");
}

Float SinNode::calculate(Float operand)
{
    return std::sin(operand);
}

static double cint(double x)
{
    if (std::fmod(x, 0) >= 0.5) // FIXME: should be modf?
        return x >= 0 ? std::ceil(x) : std::floor(x);
    else
        return x < 0 ? std::ceil(x) : std::floor(x);
}

static double round(double r, unsigned int places)
{
    if (places == 0)
    {
        return cint(r);
    }
    else
    {
        double off = std::pow(10, (double) places);
        return cint(r * off) / off;
    }
}

RoundNode::RoundNode() :
    OneFloatMathNode()
{
    this->setShortDocumentation("Rounds floats to a given precision.");
    this->addAttribute(Attribute::ptr(new Attribute("precision", Message("i", 0), "How much float precision you want to keep.")));
}

Float RoundNode::calculate(Float operand)
{
    return round(operand, std::abs(this->getAttributeValue("precision").getInt(0)));
}

} // end of namespace
} // end of namespace

