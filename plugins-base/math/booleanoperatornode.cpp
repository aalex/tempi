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

#include "plugins-base/math/booleanoperatornode.h"
#include "tempi/utils.h"

namespace tempi { 
namespace plugins_base {

BooleanMathNode::BooleanMathNode() :
    Node()
{
    addInlet("0", "Incoming left float operand. (hot)");
    addInlet("1", "Incoming right float operand. (cold)");
    addOutlet("0", "Resulting float.");

    Message operand = Message("f", 0.0f);
    addAttribute(Attribute::ptr(new Attribute("operand", operand, "Right operand to compare incoming float values with.")));
}

void BooleanMathNode::processMessage(const char *inlet, const Message &message)
{
    if (utils::stringsMatch(inlet, "1"))
    {
        if (message.typesMatch("f"))
            this->setAttributeValue("operand", message);
        else
            std::cerr << "BooleanMathNode::" << __FUNCTION__ << "(): Bad type for message " << message << std::endl;
    }
    else if (message.typesMatch("f"))
    {
        float left_operand = message.getFloat(0);
        float right_operand = getAttribute("operand")->
            getValue().getFloat(0);
        Message result("b", this->compare(left_operand, right_operand));
        this->output("0", result);
    }
    else
        std::cerr << "BooleanMathNode::" << __FUNCTION__ <<
            "(): Bad type for message " << message << std::endl;
}

EqualsNotNode::EqualsNotNode() :
    BooleanMathNode()
{
    setShortDocumentation("Outputs true if two float are not equal.");
}

bool EqualsNotNode::compare(float left_operand, float right_operand)
{
    return left_operand != right_operand;
}

IsGreaterNode::IsGreaterNode() :
    BooleanMathNode()
{
    setShortDocumentation("Outputs true if the incoming float is greather than the right operand.");
}

bool IsGreaterNode::compare(float left_operand, float right_operand)
{
    return left_operand > right_operand;
}

IsEqualNode::IsEqualNode() :
    BooleanMathNode()
{
    setShortDocumentation("Outputs true if the incoming float is equal to the right operand.");
}

bool IsEqualNode::compare(float left_operand, float right_operand)
{
    return left_operand == right_operand;
}

IsLessNode::IsLessNode() :
    BooleanMathNode()
{
    setShortDocumentation("Outputs true if the incoming float is smaller than the right operand.");
}

bool IsLessNode::compare(float left_operand, float right_operand)
{
    return left_operand < right_operand;
}

} // end of namespace
} // end of namespace

