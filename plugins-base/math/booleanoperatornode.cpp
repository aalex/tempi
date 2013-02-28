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
#include "tempi/log.h"

namespace tempi { 
namespace plugins_base {

const char * const BooleanMathNode::HOT_NUMBER_INLET = "0";
const char * const BooleanMathNode::COLD_NUMBER_INLET = "1";
const char * const BooleanMathNode::BOOL_OUTLET = "0";
const char * const BooleanMathNode::OPERAND_ATTR = "operand";

BooleanMathNode::BooleanMathNode() :
    Node()
{
    addInlet(HOT_NUMBER_INLET, "Incoming left float operand. (hot)", "Expects a single float value.", "f");
    addInlet(COLD_NUMBER_INLET, "Incoming right float operand. (cold)", "Expects a single float value", "f");
    addOutlet(BOOL_OUTLET, "Resulting boolean.");

    addAttribute(Attribute::ptr(new Attribute(OPERAND_ATTR, Message("f", 0.0f), "Right operand to compare incoming float values with.")));
}

void BooleanMathNode::processMessage(const char *inlet, const Message &message)
{
    if (utils::stringsMatch(inlet, COLD_NUMBER_INLET))
    {
        this->setAttributeValue("operand", message);
    }
    else if (utils::stringsMatch(inlet, HOT_NUMBER_INLET))
    {
        float left_operand = message.getFloat(0);
        float right_operand = getAttribute("operand")->
            getValue().getFloat(0);
        Message result("b", this->compare(left_operand, right_operand));
        this->output(BOOL_OUTLET, result);
    }
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

IsGreaterOrEqualNode::IsGreaterOrEqualNode() :
    BooleanMathNode()
{
    setShortDocumentation("Outputs true if the incoming float is greather or equal than the right operand.");
}

bool IsGreaterOrEqualNode::compare(float left_operand, float right_operand)
{
    return left_operand >= right_operand;
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

IsLessOrEqualNode::IsLessOrEqualNode() :
    BooleanMathNode()
{
    setShortDocumentation("Outputs true if the incoming float is smaller or equal than the right operand.");
}

bool IsLessOrEqualNode::compare(float left_operand, float right_operand)
{
    return left_operand <= right_operand;
}

} // end of namespace
} // end of namespace

