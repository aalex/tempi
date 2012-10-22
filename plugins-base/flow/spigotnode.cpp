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
#include "plugins-base/flow/spigotnode.h"
#include "tempi/utils.h"

namespace tempi {
namespace plugins_base {

SpigotNode::SpigotNode() :
    Node()
{
    addAttribute(Attribute::ptr(new Attribute("pass", Message("b", true), "Let messages flow or not.")));
    addInlet("0", "Incoming messages.");
    addOutlet("0", "Outlet from which messages flow if its open.");
    setShortDocumentation("Lets messages flow through it or not.");
}

void SpigotNode::processMessage(const char *inlet, const Message &message)
{
    if (utils::stringsMatch("0", inlet))
    {
        if (getAttributeValue("pass").getBoolean(0))
            output("0", message);
    }
}

} // end of namespace
} // end of namespace

