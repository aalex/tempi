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

#include "tempi/timeposition.h"
#include "tempi/log.h"
#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/date_time/gregorian/gregorian.hpp"
#include "plugins-base/time/cron_node.h"

namespace tempi {
namespace plugins_base {

static bool isNumber(const std::string &text)
{
    try
    {
        boost::lexical_cast<int>(text);
        return true;
    }
    catch (const boost::bad_lexical_cast &e)
    {
        return false;
    }
}

static int toNumber(const std::string &text)
{
    try
    {
        return boost::lexical_cast<int>(text);
    }
    catch (const boost::bad_lexical_cast &e)
    {
        return -1;
    }
}

static bool isWildcard(const std::string &text)
{
    if (text == "*")
        return true;
    else
        return false;
}

static DateTime getNow()
{
    boost::gregorian::date date;
    date = boost::gregorian::day_clock::local_day();
    int month = date.month();
    int month_day = date.day();
    int day_of_week = date.day_of_week();

    boost::posix_time::ptime now;
    now = boost::posix_time::microsec_clock::local_time();
    std::tm timetm = boost::posix_time::to_tm(now);
    int hours = timetm.tm_hour;
    int minutes = timetm.tm_min;

    DateTime ret;
    ret.minute = minutes;
    ret.hour = hours;
    ret.day = month_day;
    ret.month = month;
    ret.day_of_week = day_of_week;
    return ret;
}


static bool isNumberWithinRangeOrWildcard(const std::string &text, int from, int to)
{
    if (isWildcard(text))
        return true;
    else if (isNumber(text))
    {
        int number = toNumber(text);
        if (number >= from && number <= to)
            return true;
        else
            return false;
    }
    else
        return false;
}

CronValidator::CronValidator() :
    minute_("*"),
    hour_("*"),
    day_("*"),
    month_("*"),
    day_of_week_("*")
{
}

bool CronValidator::isNow() const
{
    bool ret = true;
    DateTime now = getNow();
    if (Logger::isEnabledFor(DEBUG))
    {
        std::ostringstream os;
        os << "CronValidator::" << __FUNCTION__ << ": now is " << now.month << "th month, " << now.day << "th day, "<< now.hour << ":" << now.minute << " " << " " << now.day_of_week << "th day of week.";
        os << " Check if it matches " << this->month_ << "th month, " << this->day_ << "th day, "<< this->hour_ << ":" << this->minute_ << " " << " " << this->day_of_week_ << "th day of week.";
        Logger::log(DEBUG, os);
    }
    if (! isWildcard(this->minute_))
    {
        if (toNumber(this->minute_) != now.minute)
            ret = false;
    }
    if (! isWildcard(this->hour_))
    {
        if (toNumber(this->hour_) != now.hour)
            ret = false;
    }
    if (! isWildcard(this->day_))
    {
        if (toNumber(this->day_) != now.day)
            ret = false;
    }
    if (! isWildcard(this->month_))
    {
        if (toNumber(this->month_) != now.month)
            ret = false;
    }
    if (! isWildcard(this->day_of_week_))
    {
        if (toNumber(this->day_of_week_) != now.day_of_week)
            ret = false;
    }
    return ret;
}

bool CronValidator::setMinute(const std::string &value)
{
    if (! isNumberWithinRangeOrWildcard(value, 0, 59))
        return false;
    else
        this->minute_ = value;
}

bool CronValidator::setHour(const std::string &value)
{
    if (! isNumberWithinRangeOrWildcard(value, 0, 23))
        return false;
    else
        this->hour_ = value;
}

bool CronValidator::setDay(const std::string &value)
{
    if (! isNumberWithinRangeOrWildcard(value, 1, 31))
        return false;
    else
        this->day_ = value;
}

bool CronValidator::setMonth(const std::string &value)
{
    if (! isNumberWithinRangeOrWildcard(value, 1, 12))
        return false;
    else
        this->month_ = value;
}

bool CronValidator::setDayOfWeek(const std::string &value)
{
    if (! isNumberWithinRangeOrWildcard(value, 0, 7))
        return false;
    else
        this->day_of_week_ = value;
}

} // end of namespace
} // end of namespace

