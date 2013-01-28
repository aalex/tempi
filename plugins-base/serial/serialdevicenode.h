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
 * The SerialDeviceNode class.
 */
#ifndef __TEMPI_SERIAL_SERIALDEVICENODE_H__
#define __TEMPI_SERIAL_SERIALDEVICENODE_H__

#include "tempi/node.h"
#include "plugins-base/serial/serialdevice.h"
#include <tr1/memory>

namespace tempi {
namespace plugins_base {

/**
 * Node that communicates with a serial device.
 */
class SerialDeviceNode : public Node
{
    public:
        SerialDeviceNode();
        static const char * const DATA_INLET;
        static const char * const DATA_OUTLET;
        static const char * const IS_OPEN_ATTR;
        static const char * const BAUD_RATE_ATTR;
        static const char * const DEVICE_FILE_ATTR;
    protected:
        virtual void processMessage(const char *inlet, const Message &message);
        virtual void doTick();
        virtual bool onNodeAttributeChanged(const char *name, const Message &value);
    private:
        std::tr1::shared_ptr<SerialDevice> device_;
};

} // end of namespace
} // end of namespace

#endif // ifndef

