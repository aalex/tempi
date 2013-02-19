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
 * The CronNode class.
 */

#ifndef __TEMPI_TIME_CRON_NODE_H__
#define __TEMPI_TIME_CRON_NODE_H__

#include "tempi/node.h"
#include "plugins-base/time/cron.h"

namespace tempi {
namespace plugins_base {

/**
 * Outputs a message when the desired date and time has come.
 */
class CronNode : public Node
{
    public:
        static const char * const IS_ENABLED_ATTR;
        static const char * const MINUTE_ATTR;
        static const char * const HOUR_ATTR;
        static const char * const DAY_ATTR;
        static const char * const MONTH_ATTR;
        static const char * const DAYOFWEEK_ATTR;
        static const char * const TRIGGER_OUTLET;
        static const char * const CHECK_INLET;
        CronNode();
    protected:
        virtual void processMessage(const char *inlet, const Message &message);
        virtual bool onNodeAttributeChanged(const char *name, const Message &value);
    private:
        CronValidator cron_;
};

} // end of namespace
} // end of namespace

#endif // ifndef

