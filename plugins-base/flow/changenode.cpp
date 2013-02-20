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

#include "plugins-base/flow/changenode.h"
#include "tempi/utils.h"
#include "tempi/log.h"

namespace tempi {
namespace plugins_base {

const char * const ChangeNode::IN_INLET = "in";
const char * const ChangeNode::OUT_OUTLET = "out";

ChangeNode::ChangeNode() :
    Node()
{
    setShortDocumentation("Outputs a message only when its incoming message is different from the last one it got.");
    addInlet(IN_INLET, "Input for incoming messages.");
    addOutlet(OUT_OUTLET, "Outputs the incoming message is different from the last one.");
}

void ChangeNode::processMessage(const char *inlet, const Message &message)
{
    if (utils::stringsMatch(IN_INLET, inlet))
    {
        if (message != this->last_)
        {
            this->last_ = message;
            this->output(OUT_OUTLET, message);
        }
        else
        {
            if (Logger::isEnabledFor(DEBUG))
            {
                std::ostringstream os;
                os << "ChangeNode::" << __FUNCTION__ << ": Same message as before. No output. " <<
                    message;
                Logger::log(DEBUG, os);
            }
        }
    }
}

} // end of namespace
} // end of namespace

