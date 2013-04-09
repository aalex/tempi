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
 * @file
 * The ToDmxUsbProNode class.
 */
#ifndef __TEMPI_SERIAL_TODMXUSBPRONODE_H__
#define __TEMPI_SERIAL_TODMXUSBPRONODE_H__

#include "tempi/node.h"
#include <vector>

namespace tempi {
namespace plugins_base {

/**
 * Node that converts a list of bytes to a list of bytes for the Enttec DMX USB Pro.
 */
class ToDmxUsbProNode : public Node
{
    public:
        ToDmxUsbProNode();
        static const char * const CHANNELS_INLET;
        static const char * const INIT_INLET;
        static const char * const BLOB_OUTLET;
        static const char * const NUM_CHANNELS_ATTR;
        static const int MAX_NUM_CHANNELS;
    protected:
        virtual void processMessage(const char *inlet, const Message &message);
        bool onNodeAttributeChanged(const char *name, const Message &value);
        virtual void doTick();
    private:
        std::vector<int> channel_values_;
        bool channel_values_has_changed_;
};

} // end of namespace
} // end of namespace

#endif // ifndef

