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
 * The DateNode class.
 */

#ifndef __TEMPI_TIME_DATE_NODE_H__
#define __TEMPI_TIME_DATE_NODE_H__

#include "tempi/timer.h"
#include "tempi/timeposition.h"
#include "tempi/node.h"

namespace tempi {
namespace plugins_base {

/**
 * Get the current date.
 */
class DateNode : public Node
{
    public:
        // date
        static const char * const YEAR_OUTLET;
        static const char * const MONTH_OUTLET;
        static const char * const DAY_OF_MONTH_OUTLET;
        // week day
        static const char * const DAY_OF_WEEK_OUTLET;
        // misc info
        static const char * const END_OF_MONTH_OUTLET;
        static const char * const WEEK_NUMBER_OUTLET;
        static const char * const IS_LEAP_YEAR_OUTLET;
        // options
        static const char * const USE_UTC_NOT_LOCAL_ATTR;
        // controls
        static const char * const TRIGGER_INLET;

        DateNode();
    protected:
        virtual void processMessage(const char *inlet, const Message &message);
    //private:
    //   virtual void doTick();
};

} // end of namespace
} // end of namespace

#endif // ifndef

