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

#include "plugins-base/time/clock_node.h"
#include "boost/date_time/posix_time/posix_time.hpp"
#include "tempi/log.h"
#include "tempi/utils.h"

namespace tempi {
namespace plugins_base {

const char * const ClockNode::HOURS_OUTLET = "hours";
const char * const ClockNode::MINUTES_OUTLET = "minutes";
const char * const ClockNode::SECONDS_OUTLET = "seconds";
//const char * const ClockNode::MILLISECONDS_OUTLET = "milliseconds";
const char * const ClockNode::USE_UTC_NOT_LOCAL_ATTR = "use_UTC";
const char * const ClockNode::TRIGGER_INLET = "trigger";

ClockNode::ClockNode() :
    Node()
{
    this->setShortDocumentation("Provides the current time information.");
    this->addAttribute(Attribute::ptr(new Attribute(USE_UTC_NOT_LOCAL_ATTR, Message("b", false), "Use the Universal Time Clock. Uses the local time if set to false.")));

    this->addOutlet(HOURS_OUTLET);
    this->addOutlet(MINUTES_OUTLET);
    this->addOutlet(SECONDS_OUTLET);
    //this->addOutlet(MILLISECONDS_OUTLET);
    this->addInlet(TRIGGER_INLET);
}

void ClockNode::processMessage(const char *inlet, const Message &message)
{
    if (! utils::stringsMatch(inlet, TRIGGER_INLET))
    {
        return;
    }
    else
    {
        bool use_utc = this->getAttributeValue(USE_UTC_NOT_LOCAL_ATTR).getBoolean(0);
        boost::posix_time::ptime now;
        if (use_utc)
            now = boost::posix_time::microsec_clock::universal_time();
        else
            now = boost::posix_time::microsec_clock::local_time();
        std::tm timetm = boost::posix_time::to_tm(now);
        int hours = timetm.tm_hour;
        int minutes = timetm.tm_min;
        int seconds = timetm.tm_sec;
        //int milliseconds = boost::posix_time::to_tm(now).milliseconds();

        this->output(HOURS_OUTLET, Message("i", hours));
        this->output(MINUTES_OUTLET, Message("i", minutes));
        this->output(SECONDS_OUTLET, Message("i", seconds));
        //this->output(MILLISECONDS_OUTLET, Message("i", milliseconds));
    }
}

} // end of namespace
} // end of namespace

