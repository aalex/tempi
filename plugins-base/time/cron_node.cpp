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

#include "plugins-base/time/cron_node.h"
#include "tempi/log.h"
#include "tempi/utils.h"

namespace tempi {
namespace plugins_base {

const char * const CronNode::IS_ENABLED_ATTR = "is_enabled";
const char * const CronNode::MINUTE_ATTR = "minute";
const char * const CronNode::HOUR_ATTR = "hour";
const char * const CronNode::DAY_ATTR = "day";
const char * const CronNode::MONTH_ATTR = "month";
const char * const CronNode::DAYOFWEEK_ATTR = "day_of_week";
const char * const CronNode::TRIGGER_OUTLET = "trigger";
const char * const CronNode::CHECK_INLET = "check";
static const char * const WILDCARD = "*";

CronNode::CronNode() :
    Node()
{
    this->setShortDocumentation("Sends a message when the desired date and time has come.");
    this->setLongDocumentation("Ranges (-) and lists (,) are not currently supported. You need to send a message to the check inlet so that is checks if the time has come.");
    this->addAttribute(Attribute::ptr(new Attribute(IS_ENABLED_ATTR, Message("b", false), "Whether it's enabled or not.")));

    this->addAttribute(Attribute::ptr(new Attribute(MINUTE_ATTR, Message("s", "*"), "Minute. Use a number within [0, 59] or *.")));
    this->addAttribute(Attribute::ptr(new Attribute(HOUR_ATTR, Message("s", "*"), "Hour. Use a number within [0, 23] or *.")));
    this->addAttribute(Attribute::ptr(new Attribute(DAY_ATTR, Message("s", "*"), "Day. Use a number within [1, 31] or *.")));
    this->addAttribute(Attribute::ptr(new Attribute(MONTH_ATTR, Message("s", "*"), "Month. Use a number within [1, 12] or *.")));
    this->addAttribute(Attribute::ptr(new Attribute(DAYOFWEEK_ATTR, Message("s", "*"), "Day of week. Use a number within [0, 7] or *. Both 0 and 7 mean Sunday.")));

    this->addOutlet(TRIGGER_OUTLET);
    this->addInlet(CHECK_INLET);
}

void CronNode::processMessage(const char *name, const Message &value)
{
    if (! utils::stringsMatch(name, CHECK_INLET))
    {
        return;
    }
    else
    {
        bool matches_now = this->cron_.isNow();
        if (matches_now)
        {
            this->output(TRIGGER_OUTLET, Message("b", true));
        }
    }
}


bool CronNode::onNodeAttributeChanged(const char *name, const Message &value)
{
    if (utils::stringsMatch(name, IS_ENABLED_ATTR))
        return true;
    else if (utils::stringsMatch(MINUTE_ATTR, name))
        return this->cron_.setMinute(value.getString(0));
    else if (utils::stringsMatch(HOUR_ATTR, name))
        return this->cron_.setHour(value.getString(0));
    else if (utils::stringsMatch(DAY_ATTR, name))
        return this->cron_.setDay(value.getString(0));
    else if (utils::stringsMatch(MONTH_ATTR, name))
        return this->cron_.setMonth(value.getString(0));
    else if (utils::stringsMatch(DAYOFWEEK_ATTR, name))
        return this->cron_.setDayOfWeek(value.getString(0));
    else
        return false;
}

} // end of namespace
} // end of namespace

