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

#include "tempi/config.h" // include before check for HAVE_SPATOSC

#ifdef HAVE_SPATOSC
#include "plugins-base/3d/3dcomparenode.h"
#include <iostream>

namespace tempi { 
namespace plugins_base {

const char * const Abstract3dCompareNode::HOT_INLET = "0";
const char * const Abstract3dCompareNode::COLD_INLET = "1";
const char * const Abstract3dCompareNode::OPERAND_ATTR = "operand";

Abstract3dCompareNode::Abstract3dCompareNode() :
    Node()
{
    addInlet(HOT_INLET, "Triggers computation. Incoming left 3-float message operand. (hot)");
    addInlet(COLD_INLET, "Stores operand. Incoming right 3-float message operand. (cold)");

    addAttribute(Attribute::ptr(new Attribute(OPERAND_ATTR, Message("fff", 0.0f, 0.0f, 0.0f),
        "Right operand to apply calculation with incoming 3-float messages.")));
}

void Abstract3dCompareNode::processMessage(const char *inlet, const Message &message)
{
    if (utils::stringsMatch(inlet, COLD_INLET))
    {
        if (message.typesMatch("fff"))
            this->setAttributeValue(OPERAND_ATTR, message);
        else
        {
            std::cerr << "Abstract3dCompareNode::" << __FUNCTION__ <<
                "(): Bad type for message " << message << std::endl;
            return;
        }
    }
    else if (utils::stringsMatch(inlet, HOT_INLET))
    {
        if (message.typesMatch("fff"))
        {
            spatosc::Vector3 left_operand(message.getFloat(0), message.getFloat(1), message.getFloat(2));
            Message right = getAttributeValue(OPERAND_ATTR);
            spatosc::Vector3 right_operand(right.getFloat(0), right.getFloat(1), right.getFloat(2));
            this->compare(left_operand, right_operand);
        }
        else
        {
            std::cerr << "Abstract3dCompareNode::" << __FUNCTION__ <<
                "(): Bad type for message " << message << std::endl;
        }
    }
}

const char * const Angle3dNode::ANGLE_ELEVATION_OUTLET = "angle_elevation";

Angle3dNode::Angle3dNode() :
    Abstract3dCompareNode()
{
    setShortDocumentation("Outputs the angle and elevation between two 3-float XYZ vectors.");
    addOutlet(ANGLE_ELEVATION_OUTLET, "Outputs f:angle, f:elevation (in radians)");
}

void Angle3dNode::compare(const spatosc::Vector3 &left, const spatosc::Vector3 &right)
{
    double angle = spatosc::angleBetweenVectors(left, right, 2); // Y-axis is null
    double elevation = spatosc::angleBetweenVectors(left, right, 1); // X-axis is null
    Message ret("ff", (float) angle, (float) elevation);
    this->output(ANGLE_ELEVATION_OUTLET, ret);
}

} // end of namespace
} // end of namespace

#endif // HAVE_SPATOSC

