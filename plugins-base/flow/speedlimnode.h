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
 * The SpeedLimNode class.
 */

#ifndef __TEMPI_FLOW_SPEEDLIM_NODE_H__
#define __TEMPI_FLOW_SPEEDLIM_NODE_H__

#include "tempi/timer.h"
#include "tempi/timeposition.h"
#include "tempi/node.h"

namespace tempi {
namespace plugins_base {

/**
 * Limits the data rate by eliminating messages younger than a given interval between the last time one passed through.
 */
class SpeedLimNode : public Node
{
    public:
        SpeedLimNode();
        static const char * const INTERVAL_ATTR;
        static const char * const MSG_INLET;
        static const char * const MSG_OUTLET;
    protected:
        virtual void processMessage(const char *inlet, const Message &message);
    private:
        Timer timer_;
        bool message_to_output_;
        Message last_message_;

        virtual void doTick();
        void outputLastMessageIfNeeded();
        bool isTimeToOutput();
};

} // end of namespace
} // end of namespace

#endif // ifndef

