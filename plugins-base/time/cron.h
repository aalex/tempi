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

/**
 * The CronScheduler class.
 */

#ifndef __TEMPI_TIME_CRON_SCHEDULER_H__
#define __TEMPI_TIME_CRON_SCHEDULER_H__

#include "tempi/timer.h"
#include "tempi/timeposition.h"
#include "tempi/node.h"

namespace tempi {
namespace plugins_base {

struct DateTime {
    public:
        int minute;
        int hour;
        int day;
        int month;
        int day_of_week;
};

class CronValidator
{
    public:

        CronValidator();
        bool isNow() const;
        bool setMinute(const std::string &value);
        bool setHour(const std::string &value);
        bool setDay(const std::string &value);
        bool setMonth(const std::string &value);
        bool setDayOfWeek(const std::string &value);
    private:
        std::string minute_;
        std::string hour_;
        std::string day_;
        std::string month_;
        std::string day_of_week_;
};

} // end of namespace
} // end of namespace
#endif

