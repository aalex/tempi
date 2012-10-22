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
#include "plugins-base/osc/oscprependnode.h"
#include "tempi/utils.h"
#include "tempi/log.h"

namespace tempi {
namespace plugins_base {

OscPrependNode::OscPrependNode() :
    Node()
{
    setShortDocumentation("Prepends some text to the first text atom of a message.");
    setShortDocumentation("Useful for OSC paths. For example, one might want to prepend \"/toon\" to the path \"/frame/add\". The result in this case would be \"/toon/frame/add\". All the other atoms after the 0th one are left unchanged.");
    addAttribute(Attribute::ptr(new Attribute("text", Message("s", ""), "Text to prepend to the first string atom in incoming messages.")));
    addInlet("incoming", "Incoming messages.");
    addOutlet("outgoing", "Resulting messages with prepent text.");
}

void OscPrependNode::processMessage(const char *inlet, const Message &message)
{
    if (! utils::stringsMatch(inlet, "incoming"))
        return;
    if (! message.indexMatchesType(0, 's'))
    {
        Logger::Output os;
        os << "[osc.prepend]: Message's first atom should be a string: " << message;
        Logger::log(WARNING, os);
        return;
    }
    Message ret = message.cloneRange(1, message.getSize() + 1);
    std::string text = getAttributeValue("text").getString(0) + message.getString(0);
    ret.prependString(text.c_str());
    output("outgoing", ret);
}

} // end of namespace
} // end of namespace

