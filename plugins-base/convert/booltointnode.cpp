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

#include <iostream>
#include "plugins-base/convert/booltointnode.h"

namespace tempi {
namespace plugins_base {

const char * const BoolToIntNode::BOOL_INLET = "in";
const char * const BoolToIntNode::NUMBER_OUTLET = "out";
const char * const BoolToIntNode::IFFALSE_NUMBER_ATTR = "if_false";
const char * const BoolToIntNode::IFTRUE_NUMBER_ATTR = "if_true";

BoolToIntNode::BoolToIntNode() :
    Node()
{
    this->setShortDocumentation("Convert a boolean to an integer.");
    this->addInlet(BOOL_INLET, "Receives messages with a single boolean value.", "", "b");
    this->addOutlet(NUMBER_OUTLET, "Outputs messages with a single integer value");
    this->addAttribute(Attribute::ptr(new Attribute(IFTRUE_NUMBER_ATTR, Message("i", 1), "Number to output if input value is true.")));
    this->addAttribute(Attribute::ptr(new Attribute(IFFALSE_NUMBER_ATTR, Message("i", 0), "Number to output if input value is false.")));
}

void BoolToIntNode::processMessage(const char *inlet, const Message &message)
{
    if (message.getBoolean(0))
        this->output(NUMBER_OUTLET, this->getAttributeValue(IFTRUE_NUMBER_ATTR));
    else
        this->output(NUMBER_OUTLET, this->getAttributeValue(IFFALSE_NUMBER_ATTR));
}

} // end of namespace
} // end of namespace

