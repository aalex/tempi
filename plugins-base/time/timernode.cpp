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

#include "plugins-base/time/timernode.h"
#include "tempi/utils.h"
#include "tempi/log.h"

namespace tempi {
namespace plugins_base {

const char * const TimerNode::RESET_INLET = "reset";
const char * const TimerNode::QUERY_INLET = "query";
const char * const TimerNode::ELAPSED_OUTLET = "elapsed";

TimerNode::TimerNode() :
    Node()
{
    this->setShortDocumentation("Calculates how much time has passed since the last time it was reset.");

    this->addInlet(RESET_INLET, "Resets the timer. (any message)");
    this->addInlet(QUERY_INLET, "Queries the internal timer. (any message)");
    this->addOutlet(ELAPSED_OUTLET, "Outputs how long since last reset when queried. (milliseconds)");

    this->timer_.reset();
}

void TimerNode::processMessage(const char *inlet, const Message &message)
{
    if (utils::stringsMatch(inlet, QUERY_INLET))
    {
        TimePosition now = this->timer_.elapsed();
        unsigned long long elapsed = timeposition::to_ms(now);
        this->output(ELAPSED_OUTLET, Message("l", (long long) elapsed));
    }
    else if (utils::stringsMatch(inlet, RESET_INLET))
    {
        this->timer_.reset();
    }
}

} // end of namespace
} // end of namespace

