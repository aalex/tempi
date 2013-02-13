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
#include "tempi/concurrentqueue.h"
#include <tr1/memory>
#include <boost/thread.hpp>

namespace tempi {
namespace plugins_base {

atom::BlobValue::ptr stringToBlob(const std::string &text);
// Converts a message to FUDI-like string.
// only supports int and string.
// doesn't append a semi-colon
std::string fudiToString(const Message &message);
// Converts a FUDI-like string to a message.
// only works with int and strings for now
Message stringToFudi(const std::string &text);
// Removes a trailing character from a string, if found.
std::string stripTrailingCharacter(const std::string &text, char character = '\n');

/**
 * Node that communicates with a serial device.
 */
class SerialDeviceNode : public Node
{
    public:
        SerialDeviceNode();
        ~SerialDeviceNode();
        static const char * const DATA_INLET;
        static const char * const DATA_OUTLET;
        static const char * const IS_OPEN_ATTR;
        static const char * const BAUD_RATE_ATTR;
        static const char * const DEVICE_FILE_ATTR;
        static const char * const FUDI_INLET;
        static const char * const FUDI_OUTLET;
    protected:
        virtual void processMessage(const char *inlet, const Message &message);
        virtual void doTick();
        virtual bool onNodeAttributeChanged(const char *name, const Message &value);
    private:
        std::tr1::shared_ptr<SerialDevice> device_;
        bool thread_should_be_running_;
        ConcurrentQueue<Message> queue_;
        boost::thread thread_;
        // the thread's main
        void run_thread();
        // stop polling
        void stop_thread();
        // start polling
        void start_thread();
        void try_to_read();
};

} // end of namespace
} // end of namespace

#endif // ifndef

