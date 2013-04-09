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
 * The ClockNode class.
 */

#ifndef __TEMPI_TIME_CLOCK_NODE_H__
#define __TEMPI_TIME_CLOCK_NODE_H__

#include "tempi/node.h"

namespace tempi {
namespace plugins_base {

/**
 * Get the current time.
 */
class ClockNode : public Node
{
    public:
        // date
        static const char * const HOURS_OUTLET;
        static const char * const MINUTES_OUTLET;
        static const char * const SECONDS_OUTLET;
        //static const char * const MILLISECONDS_OUTLET;
        // options
        static const char * const USE_UTC_NOT_LOCAL_ATTR;
        // controls
        static const char * const TRIGGER_INLET;

        ClockNode();
    protected:
        virtual void processMessage(const char *inlet, const Message &message);
};

} // end of namespace
} // end of namespace

#endif // ifndef

