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

#include "plugins-base/serial/serialdevicenode.h"
#include "tempi/utils.h"
#include "tempi/log.h"
#include <iostream>

namespace tempi {
namespace plugins_base {

const char * const SerialDeviceNode::DATA_INLET = "data";
const char * const SerialDeviceNode::DATA_OUTLET = "data";
const char * const SerialDeviceNode::IS_OPEN_ATTR = "is_open";
const char * const SerialDeviceNode::BAUD_RATE_ATTR = "baud_rate";
const char * const SerialDeviceNode::DEVICE_FILE_ATTR = "device";

SerialDeviceNode::SerialDeviceNode() :
    Node()
{
    setShortDocumentation("Communicates with serial devices.");
    setLongDocumentation("Right now, it only supports 8N1 without flow control. It opens the device when you set it to a valid file name.");

    // File name
    addAttribute(Attribute::ptr(new Attribute(DEVICE_FILE_ATTR, Message("s", ""), "UNIX-like device file name. example: /dev/ttyUSB0")));
    // Baud rate
    addAttribute(Attribute::ptr(new Attribute(BAUD_RATE_ATTR, Message("i", 9600), "Baud rate. Valid values are 4800, 9600, 19200, 38400, 57600, 115200.")));
    addAttribute(Attribute::ptr(new Attribute(IS_OPEN_ATTR, Message("b", false), "Tells you whether the communication is open or not.")));
    getAttribute(IS_OPEN_ATTR)->setMutable(false);

    addInlet(DATA_INLET, "Messages to send to the device. Must be a single blob.");
    addOutlet(DATA_OUTLET, "Messages received from the device. Single blobs.");
}

void SerialDeviceNode::processMessage(const char *inlet, const Message &message)
{
    if (utils::stringsMatch(DATA_INLET, inlet))
    {
        if (device_.get() == 0)
        {
            std::ostringstream os;
            os << "SerialDeviceNode::" << __FUNCTION__ << ": Device file has not been specified.";
            Logger::log(ERROR, os);
        }
        else if (message.typesMatch("B"))
        {
            atom::BlobValue::ptr blob = message.getBlob(0);
            device_->writeBlob(blob->getValue(), blob->getSize());
        }
        else if (message.typesMatch("s"))
        {
            std::string string = message.getString(0);
            device_->writeBlob(string.c_str(), string.size() + 1);
        }
        else
        {
            std::ostringstream os;
            os << "SerialDeviceNode::" << __FUNCTION__ << ": Message must be a blob."; // or a string for now
            Logger::log(ERROR, os);
        }
    }
}

bool SerialDeviceNode::onNodeAttributeChanged(const char *name, const Message &value)
{
    //std::cout << "SerialDeviceNode::" << __FUNCTION__ << "(" << name << ", " << value << ")" << std::endl;
    if (utils::stringsMatch(DEVICE_FILE_ATTR, name))
    {
        // TODO: do not open it twice
        std::string device = value.getString(0);
        if (device == "")
        {
            this->device_.reset();
            this->setAttributeValue(IS_OPEN_ATTR, Message("b", false));
            return true;
        }
        if (Logger::isEnabledFor(NOTICE))
        {
            std::ostringstream os;
            os << "SerialDeviceNode: Change serial device to " << device;
            Logger::log(NOTICE, os);
        }
        // TODO: validate that file name exists
        this->device_.reset(new SerialDevice(
            device.c_str(),
            this->getAttributeValue(BAUD_RATE_ATTR).getInt(0)));
        bool success = this->device_->openDevice();
        this->setAttributeValue(IS_OPEN_ATTR, Message("b", success));
        return true;
    }
    else if (utils::stringsMatch(name, BAUD_RATE_ATTR))
    {
        int rate = value.getInt(0);
        // validate baud rate
        switch (rate)
        {
            case 4800:
            case 9600:
            case 19200:
            case 38400:
            case 57600:
            case 115200:
                break;
            default:
                return false;
        }
        if (Logger::isEnabledFor(NOTICE))
        {
            std::ostringstream os;
            os << "SerialDeviceNode: Change baud rate to " << rate;
            Logger::log(NOTICE, os);
        }
    }
    return true;
}

void SerialDeviceNode::doTick()
{
    if (this->device_ && this->device_->isOpen())
    {
        size_t max_length = 256;
        char result[256];
        memset(result, 0, 256);
        size_t total_num_read;
        unsigned long long timeout_ms = 50;
        // TODO: do not use until_char, but simply output it all as a blob
        static const char until_char = '\n';
        bool use_until_char = true;
        // TODO: use a mutex! (it's blocking, right now!)
        bool did_read_some = this->device_->readUntil(result, max_length, total_num_read, timeout_ms, until_char, use_until_char);
        if (did_read_some)
        {
            // TODO: output a blob, not a string
            // we remove the trailing new line
            std::string tmp_string = std::string(result);
            Message to_output_message;
            to_output_message.appendString(tmp_string.c_str()); //tmp_string.substr(0, total_num_read - 1).c_str());
            output(DATA_OUTLET, to_output_message);
        }
    }
}

} // end of namespace
} // end of namespace

