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
#include <algorithm> // std::erase
#include <boost/algorithm/string/classification.hpp> // for tokenizer
#include <boost/algorithm/string/split.hpp> // for tokenizer
#include <boost/algorithm/string.hpp> // for trim
#include <boost/foreach.hpp> // for tokenizer
#include <boost/lexical_cast.hpp>

namespace tempi {
namespace plugins_base {

const char * const SerialDeviceNode::DATA_INLET = "data";
const char * const SerialDeviceNode::DATA_OUTLET = "data";
const char * const SerialDeviceNode::IS_OPEN_ATTR = "is_open";
const char * const SerialDeviceNode::BAUD_RATE_ATTR = "baud_rate";
const char * const SerialDeviceNode::DEVICE_FILE_ATTR = "device";
const char * const SerialDeviceNode::FUDI_INLET = "fudi";
const char * const SerialDeviceNode::FUDI_OUTLET = "fudi";

SerialDeviceNode::SerialDeviceNode() :
    Node()
{
    this->thread_should_be_running_ = false;

    this->setShortDocumentation("Communicates with serial devices.");
    this->setLongDocumentation("Right now, it only supports 8N1 without flow control. It opens the device when you set it to a valid file name.");

    // File name
    this->addAttribute(Attribute::ptr(new Attribute(DEVICE_FILE_ATTR, Message("s", ""), "UNIX-like device file name. example: /dev/ttyUSB0")));
    // Baud rate
    // FIXME: right now, it opens the device when you set the device attribute.
    // FIXME: when you set the baud_rate attribute, it doesn't actually change it if the device has already been open.
    this->addAttribute(Attribute::ptr(new Attribute(BAUD_RATE_ATTR, Message("i", 9600), "Baud rate. Valid values are 4800, 9600, 19200, 38400, 57600, 115200.")));
    this->addAttribute(Attribute::ptr(new Attribute(IS_OPEN_ATTR, Message("b", false), "Tells you whether the communication is open or not.")));
    this->getAttribute(IS_OPEN_ATTR)->setMutable(false);

    this->addInlet(DATA_INLET, "Messages to send to the device. Must be a single blob.");
    this->addOutlet(DATA_OUTLET, "Messages received from the device. Single blobs.");
    this->addInlet(FUDI_INLET, "Messages to send to the device. Must be a single blob.");
    this->addOutlet(FUDI_OUTLET, "Messages received from the device. Single blobs.");

}

SerialDeviceNode::~SerialDeviceNode()
{
    this->stop_thread();
}

atom::BlobValue::ptr stringToBlob(const std::string &text)
{
    size_t size = text.size() + 1;
    atom::BlobValue::ptr blob = atom::BlobValue::convert(atom::BlobValue::create(text.c_str(), size));
    // atom::BlobValue::ptr(new atom::BlobValue(size));
    return blob;
}

std::string stripTrailingCharacter(const std::string &text, char character)
{
    std::string tmp = text;
    tmp.erase(std::remove(tmp.begin(), tmp.end(), character), tmp.end());
    return tmp;
}

std::vector<std::string> tokenize(const std::string &text)
{
    std::vector<std::string> result;
    typedef std::vector<std::string> Tokens;
    Tokens tokens;
    boost::split(tokens, text, boost::is_any_of(" "));
    return tokens;
    //std::cout << tokens.size() << " tokens" << std::endl;
    //BOOST_FOREACH(const std::string& i, tokens)
    //{
    //    result.push_back(i);
    //}
    //return result;
}

Message stringToFudi(const std::string &text)
{
    Message ret;
    std::string tmp = stripTrailingCharacter(text, '\n');
    //boost::algorithm::trim(tmp);
    std::vector<std::string> tokens = tokenize(tmp);
    BOOST_FOREACH(const std::string& token, tokens)
    {
        try
        {
            ret.appendInt(boost::lexical_cast<int>(token));
        }
        catch (...)
        {
            ret.appendString(token.c_str());
        }
    }
    return ret;
}

std::string fudiToString(const Message &message)
{
    std::string result;
    if (message.getSize() == 0)
        return result;

    std::ostringstream os;
    for (unsigned int i = 0; i < message.getSize(); i++)
    {
        bool did_append = true;
        AtomType type;
        message.getAtomType(i, type);
        switch (type)
        {
            case INT:
                os << message.getInt(i);
                break;
            case STRING:
                os << message.getString(i);
                break;
            default:
            {
                std::ostringstream os2;
                os2 << __FUNCTION__ << ": Unsupported atom type: " << type;
                Logger::log(ERROR, os2);
                did_append = false;
                break;
            }
        }
        // message size if not 0 for sure
        if (did_append && i != (message.getSize() - 1))
        {
            os << " ";
        }
    }
    return os.str();
}

void SerialDeviceNode::processMessage(const char *inlet, const Message &message)
{
    if (device_.get() == 0)
    {
        std::ostringstream os;
        os << "SerialDeviceNode::" << __FUNCTION__ << ": Device file has not been specified.";
        Logger::log(ERROR, os);
        return;
    }
    if (utils::stringsMatch(DATA_INLET, inlet))
    {
        if (message.typesMatch("B"))
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
    else if (utils::stringsMatch(FUDI_INLET, inlet))
    {
        if (Logger::isEnabledFor(DEBUG))
        {
            std::ostringstream os2;
            os2 << "SerialDeviceNode::" << __FUNCTION__ << ": Will convert to FUDI " << message;;
            Logger::log(DEBUG, os2);
        }
        std::string text = fudiToString(message);
        std::ostringstream os;
        os << text;
        if (Logger::isEnabledFor(DEBUG))
        {
            std::ostringstream os2;
            os2 << "SerialDeviceNode: send string \"" << os.str(); // << "\\n\"";
            os2 << "\".";
            Logger::log(DEBUG, os2);
        }
        os << "\n"; // append newline
        atom::BlobValue::ptr blob = stringToBlob(os.str());
        device_->writeBlob(blob->getValue(), blob->getSize());
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
            this->stop_thread();
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
        this->stop_thread();
        this->device_.reset(new SerialDevice(
            device.c_str(),
            this->getAttributeValue(BAUD_RATE_ATTR).getInt(0)));
        this->start_thread();
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
    // TODO: try pop from asyc queue
    // TODO: output
    bool some_to_output = false;
    if (this->thread_should_be_running_)
    {
        do // begin do-while
        {
            Message to_output_message;
            some_to_output = this->queue_.try_pop(to_output_message);
            if (some_to_output)
            {
                this->output(FUDI_OUTLET, to_output_message);
            }
        }
        while (some_to_output); // end of do-while
    }
}

void SerialDeviceNode::try_to_read()
{
    if (this->device_ && this->device_->isOpen())
    {
        size_t max_length = 256;
        char result[256];
        memset(result, 0, 256);
        size_t total_num_read;
        unsigned long long timeout_ms = 1000;
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
            int last_char_pos = tmp_string.size() - 2;
            if (last_char_pos >= 0)
                tmp_string = tmp_string.substr(0, last_char_pos); // remove last char
            else
                tmp_string = tmp_string.substr(0, tmp_string.size() - 1); // remove last char
            Message to_output_message = stringToFudi(tmp_string);
            if (Logger::isEnabledFor(DEBUG))
            {
                std::ostringstream os;
                os << "SerialDeviceNode." << __FUNCTION__ << " did read " << to_output_message;
                Logger::log(DEBUG, os);
            }
            // TODO: stringToFudi
            //to_output_message.appendString(tmp_string.c_str()); //tmp_string.substr(0, total_num_read - 1).c_str());
            //output(DATA_OUTLET, to_output_message);
            //this->output(FUDI_OUTLET, to_output_message);
            this->queue_.push(to_output_message);
        }
    }
}

void SerialDeviceNode::start_thread()
{
    if (this->thread_should_be_running_)
    {
        std::ostringstream os;
        os << "SerialDeviceNode::" << __FUNCTION__ << ": " << "Thread is already running!";
        Logger::log(ERROR, os);
    }
    else
    {
        this->thread_ = boost::thread(&SerialDeviceNode::run_thread, this);
    }
}

void SerialDeviceNode::run_thread()
{
    // The thread's main
    boost::posix_time::milliseconds sleep_ms(15);
    this->thread_should_be_running_ = true;
    while (this->thread_should_be_running_)
    {
        this->try_to_read();
        boost::this_thread::sleep(sleep_ms);
    }
}

void SerialDeviceNode::stop_thread()
{
    if (this->thread_should_be_running_)
    {
        this->thread_should_be_running_ = false;
        this->thread_.join();
    }
}

} // end of namespace
} // end of namespace

