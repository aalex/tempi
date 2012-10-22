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

#include "plugins-base/flow/anynode.h"
#include "tempi/utils.h"

namespace tempi {
namespace plugins_base {

AnyNode::AnyNode() :
    Node()
{
    setShortDocumentation("Stores message and outputs them when it receives an empty message. (bang)");
    addAttribute(Attribute::ptr(new Attribute("value", Message(), "Holds any message to store.", false)));
    addOutlet("0", "Outputs the stored value when inlet 0 is banged.");
    addInlet("0", "Bang to output value. Any other type of message will set and output value.");
}

void AnyNode::processMessage(const char *inlet, const Message &message)
{
    if (message.getTypes() == "") // bang only outputs the value
    {
        // pass
    }
    else // any message with some type tags sets the value and outputs it
    {
        setAttributeValue("value", message);
    }
    if (utils::stringsMatch(inlet, "0"))
        output("0", getAttributeValue("value"));
}

} // end of namespace
} // end of namespace

