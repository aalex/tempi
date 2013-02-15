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
#include "tempi/utils.h"
#include "tempi/log.h"
#include <atom/blobvalue.h>

namespace tempi {
namespace plugins_base {

const char * const ToDmxUsbProNode::NUMBERS_INLET = "in";
const char * const ToDmxUsbProNode::BYTES_OUTLET = "out";

ToDmxUsbProNode::ToDmxUsbProNode() :
    Node()
{
    this->setShortDocumentation("Cook lists of numbers for the Enttec DMX USB Pro device.");
    this->setLongDocumentation("Send messages containing a list of integers to its \"in\" input. It will output a message containing a list of integers so that you can convert it to a blob and then send it to the serial device that is the Enttec DMX USB Pro.");

    this->addInlet(NUMBERS_INLET, "Messages to cook. Must be only integers.");
    this->addInlet(INIT_INLET, "Send anything to this inlet to output an initialization message for the DMX USB Pro.");
    this->addOutlet(BYTES_OUTLET, "Resulting lists of bytes.");
}

bool messageToInts(std::vector<int> &ints, const Message &message)
{
    bool ok = true;
    for (unsigned int i = 0; i < message.getSize(); i++)
    {
        AtomType type;
        message.getAtomType(i, type);
        if (type == INT)
        {
            bytes.push_back((char) message.getInt(i));
        }
        else
        {
            std::ostringstream os;
            os << "Unsuported type tag: " << type;
            Logger::log(WARNING, os);
            ok = false;
        }
    }
    return ok;
}

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

/**
 * Prepares some data to send to the Enttec DMX USB Pro.
 * Fills the blob with bytes of data.
 * flag and the contents of data must be in the range that fits into a byte.
 */
static void toEnttecDMX(atom::BlobValue::ptr &blob, int flag, const std::vector<int> &data)
{   
    // Begin
    char begin_char = 0x7e;
    blob->append(&begin_char, 1);
    
    // Flag
    char flag_char = (char) flag;
    blob->append(&flag_char, 1);
    
    // Data size
    char size_first_char = (char) (data.size() & 0xff);
    char size_second_char = (char) ((data.size() >> 8) & 0xff);
    blob->append(&size_first_char, 1);
    blob->append(&size_second_char, 1);
    
    // Data
    std::vector<int>::const_iterator iter;
    for (iter = data.begin(); iter != data.end(); ++iter)
    {   
        char data_char = (char) *iter;
        blob->append(&data_char, 1);
    }
}


void ToDmxUsbProNode::processMessage(const char *inlet, const Message &message)
{
    if (utils::stringsMatch(NUMBERS_INLET, inlet))
    {
        std::vector<int> ints;
        bool ok = messageToInts(ints, message);
        if (! ok)
        {
            std::ostringstream os;
            os << "ToDmxUsbProNode::" << __FUNCTION__ << ": Could not convert all atoms to ints." << message.getTypes();
            Logger::log(ERROR, os);
            return; // XXX
        }
        Message result;
        std::vector<int>::const_iterator iter;
        for (iter = ints.begin(); iter != ints.end(); ++iter)
        {
            result.appendInt(*iter);
        }
        this->output(BYTES_OUTLET, result);
    }
}

} // end of namespace
} // end of namespace

