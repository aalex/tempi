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

#include "plugins-base/serial/todmxusbpro.h"
#include "plugins-base/serial/blobutils.h"
#include "tempi/utils.h"
#include "tempi/log.h"
#include <atom/blobvalue.h>
#include <algorithm> // std::copy

namespace tempi {
namespace plugins_base {

const char * const ToDmxUsbProNode::CHANNELS_INLET = "channels";
const char * const ToDmxUsbProNode::INIT_INLET = "init";
const char * const ToDmxUsbProNode::BLOB_OUTLET = "out";
const char * const ToDmxUsbProNode::NUM_CHANNELS_ATTR = "num_channels";
const int ToDmxUsbProNode::MAX_NUM_CHANNELS = 512;

ToDmxUsbProNode::ToDmxUsbProNode() :
    Node()
{
    this->setShortDocumentation("Cook control data for the Enttec DMX USB Pro device.");
    this->setLongDocumentation("Send messages to its channels or init inlets, containing a list of integers to its \"in\" input. It will output a message containing a list of integers so that you can convert it to a blob and then send it to the serial device that is the Enttec DMX USB Pro.");

    this->addInlet(CHANNELS_INLET, "Channel messages to cook. Must be two ints: i:<channel> i:<value>", "The first number is the channel, the second the value. Both are clipped within the range [0, 255]", "ii");
    this->addInlet(INIT_INLET, "Send anything to this inlet to output an initialization message for the DMX USB Pro.");
    this->addOutlet(BLOB_OUTLET, "Resulting blobs to send to the serial device.");
    this->addAttribute(Attribute::ptr(new Attribute(NUM_CHANNELS_ATTR, Message("i", 64), "Number of DMX channels to control. Can be up to 512.")));

    for (int i = 0; i < MAX_NUM_CHANNELS; i++)
    {
        this->channel_values_.push_back(0);
    }
    this->channel_values_has_changed_ = false;
}

bool ToDmxUsbProNode::onNodeAttributeChanged(const char *name, const Message &value)
{
    if (utils::stringsMatch(NUM_CHANNELS_ATTR, name))
    {
        static const int MIN_NUM = 64;
        int num = value.getInt(0);
        bool range_ok = true;
        if (num < MIN_NUM)
        {
            // TODO: clip it: num = MIN_NUM;
            range_ok = false;
        }
        else if (num > MAX_NUM_CHANNELS)
        {
            // TODO: clip it: num = MAX_NUM_CHANNELS;
            range_ok = false;
        }
        if (range_ok)
        {
            return true;
        }
        else
        {
            std::ostringstream os;
            os << "ToDmxUsbProNode::" << __FUNCTION__ << ": " << name << ": " << "Number must be within the range [0, 512]. Got " << value.getInt(0) << ".";
            Logger::log(ERROR, os);
            return false;
        }
    }
}

// bool messageToInts(std::vector<int> &ints, const Message &message)
// {
//     bool ok = true;
//     for (unsigned int i = 0; i < message.getSize(); i++)
//     {
//         AtomType type;
//         message.getAtomType(i, type);
//         if (type == INT)
//         {
//             bytes.push_back((char) message.getInt(i));
//         }
//         else
//         {
//             std::ostringstream os;
//             os << "Unsuported type tag: " << type;
//             Logger::log(WARNING, os);
//             ok = false;
//         }
//     }
//     return ok;
// }

/**
 * DMX output break time in 10.67 microsend units.
 */
static const int DMX_BREAK = 9;
/**
 * DMX output Mark After Break time in 10.67 microsend units.
 */
static const int DMX_MARK = 1;
/**
 * DMX output rate in packets per second.
 */
static const int DMX_RATE = 40;

/**
 * Prepares some data to send to the Enttec DMX USB Pro.
 * Fills the blob with bytes of data.
 * flag and the contents of data must be in the range that fits into a byte.
 */
static void toEnttecDMX(atom::BlobValue::ptr &blob, int flag, const std::vector<int> &data)
{   
    // Begin
    static const atom::Byte BEGIN_CHAR = 0x7e;
    blob->append(&BEGIN_CHAR, 1);
    
    // Flag
    atom::Byte flag_char = (atom::Byte) flag;
    blob->append(&flag_char, 1);
    
    // Data size
    // least significant byte
    atom::Byte size_first_char = (atom::Byte) (data.size() & 0xff);
    // most significant byte
    atom::Byte size_second_char = (atom::Byte) ((data.size() >> 8) & 0xff);
    blob->append(&size_first_char, 1);
    blob->append(&size_second_char, 1);
    
    // Data
    std::vector<int>::const_iterator iter;
    for (iter = data.begin(); iter != data.end(); ++iter)
    {   
        atom::Byte data_char = (atom::Byte) *iter;
        blob->append(&data_char, 1);
    }

    // End
    static const atom::Byte END_CHAR = 0xe7;
    blob->append(&END_CHAR, 1);
}

/**
 * Prepares the initialization message for the DMX USB Pro.
 */
static void initEnttecDMX(atom::BlobValue::ptr &blob)
{   
    static const int FLAG = 4;
    std::vector<int> data;
    data.push_back(0);
    data.push_back(0);
    data.push_back(DMX_BREAK);
    data.push_back(DMX_MARK);
    data.push_back(DMX_RATE);

    toEnttecDMX(blob, FLAG, data);
}

void ToDmxUsbProNode::processMessage(const char *inlet, const Message &message)
{
    if (utils::stringsMatch(INIT_INLET, inlet))
    {
        atom::BlobValue::ptr blob = blobutils::createEmptyBlob();
        initEnttecDMX(blob);
        Message result;
        result.appendBlob(blob);
        this->output(BLOB_OUTLET, result);
        return;
    }
    else if (utils::stringsMatch(CHANNELS_INLET, inlet))
    {
        // type tag is "ii"
        int channel = message.getInt(0);
        int value = message.getInt(1);
        int num_channels = this->getAttributeValue(NUM_CHANNELS_ATTR).getInt(0);
        int max_channel_index = num_channels - 1; // FIXME: is this right?
        if (channel < 0 || channel > max_channel_index)
        {
            std::ostringstream os;
            os << "ToDmxUsbProNode::" << __FUNCTION__ << ": " << inlet << " " << message;
            os << ": " << "Channel is out or range!";
            Logger::log(ERROR, os);
            return;
        }
        if (value < 0 || value > 511)
        {
            std::ostringstream os;
            os << "ToDmxUsbProNode::" << __FUNCTION__ << ": " << inlet << " " << message;
            os << ": " << "Value is out or range!";
            Logger::log(ERROR, os);
            return;
        }

        this->channel_values_[channel] = value;
        this->channel_values_has_changed_ = true;
    }
}

void ToDmxUsbProNode::doTick()
{
    static const int CHANNELS_DATA_FLAG = 6;

    if (this->channel_values_has_changed_)
    {
        if (Logger::isEnabledFor(INFO))
        {
            std::ostringstream os;
            os << "ToDmxUsbProNode::" << __FUNCTION__ << ": channel_values_has_changed_ is true. Now output channels data blob.";
            Logger::log(INFO, os);
        }
        this->channel_values_has_changed_ = false;

        atom::BlobValue::ptr blob = blobutils::createEmptyBlob();
        int num_channels = this->getAttributeValue(NUM_CHANNELS_ATTR).getInt(0);
        std::vector<int> channels_to_send(num_channels);
        std::copy(this->channel_values_.begin(), this->channel_values_.begin() + num_channels, channels_to_send.begin());
        toEnttecDMX(blob, CHANNELS_DATA_FLAG, channels_to_send);

        Message result;
        result.appendBlob(blob);
        this->output(BLOB_OUTLET, result);
    }
}

} // end of namespace
} // end of namespace

