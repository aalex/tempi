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

#include "plugins-base/time/date_node.h"
#include "boost/date_time/gregorian/gregorian.hpp"
#include "tempi/log.h"
#include "tempi/utils.h"

namespace tempi {
namespace plugins_base {

const char * const DateNode::YEAR_OUTLET = "year";
const char * const DateNode::MONTH_OUTLET = "month";
const char * const DateNode::DAY_OF_MONTH_OUTLET = "day_of_month";
const char * const DateNode::DAY_OF_WEEK_OUTLET = "day_of_week";
const char * const DateNode::END_OF_MONTH_OUTLET = "end_of_month";
const char * const DateNode::WEEK_NUMBER_OUTLET = "week_number";
//const char * const DateNode::IS_LEAP_YEAR_OUTLET = "is_leap_year";
const char * const DateNode::USE_UTC_NOT_LOCAL_ATTR = "use_UTC";
const char * const DateNode::TRIGGER_INLET = "trigger";

DateNode::DateNode() :
    Node()
{
    this->setShortDocumentation("Provides the current date and time information.");
    this->addAttribute(Attribute::ptr(new Attribute(USE_UTC_NOT_LOCAL_ATTR, Message("b", false), "Use the Universal Time Clock. Uses the local time if set to false.")));

    //this->addOutlet(HOUR_OUTLET);
    //this->addOutlet(MINUTE_OUTLET);
    //this->addOutlet(SECOND_OUTLET);
    this->addOutlet(YEAR_OUTLET);
    this->addOutlet(MONTH_OUTLET);
    this->addOutlet(DAY_OF_MONTH_OUTLET);
    this->addOutlet(DAY_OF_WEEK_OUTLET);
    this->addOutlet(END_OF_MONTH_OUTLET);
    this->addOutlet(WEEK_NUMBER_OUTLET);
    //this->addOutlet(IS_LEAP_YEAR_OUTLET);
    this->addInlet(TRIGGER_INLET);
}

void DateNode::processMessage(const char *inlet, const Message &message)
{
    if (! utils::stringsMatch(inlet, TRIGGER_INLET))
    {
        return;
    }
    else
    {
        bool use_utc = this->getAttributeValue(USE_UTC_NOT_LOCAL_ATTR).getBoolean(0);
        boost::gregorian::date date;
        if (use_utc)
            date = boost::gregorian::day_clock::universal_day();
        else
            date = boost::gregorian::day_clock::local_day();

        int year = date.year();
        int month = date.month();
        int month_day = date.day();
        int day_of_week = date.day_of_week();
        int end_of_month = date.end_of_month().day();
        int week_number = date.week_number();
        //bool is_leap_year = boost::gregorian_calendar::is_leap_year(year);

        this->output(YEAR_OUTLET, Message("i", year));
        this->output(MONTH_OUTLET, Message("i", month));
        this->output(DAY_OF_MONTH_OUTLET, Message("i", month_day));
        this->output(DAY_OF_WEEK_OUTLET, Message("i", day_of_week));
        this->output(END_OF_MONTH_OUTLET, Message("i", end_of_month));
        this->output(WEEK_NUMBER_OUTLET, Message("i", week_number));
        //this->output(IS_LEAP_YEAR_OUTLET, Message("b", is_leap_year));
    }
}

} // end of namespace
} // end of namespace

