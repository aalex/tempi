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

#include "plugins-base/serial/blobtobytes.h"
#include "tempi/utils.h"
#include "tempi/log.h"

namespace tempi {
namespace plugins_base {

const char * const BlobToBytesNode::BLOB_INLET = "blob";
const char * const BlobToBytesNode::BYTES_OUTLET = "bytes";

BlobToBytesNode::BlobToBytesNode() :
    Node()
{
    this->setShortDocumentation("Converts blobs to lists of bytes.");
    this->setLongDocumentation("Send messages containing a single blob. It will output a message containing a list of integers.");

    this->addInlet(BLOB_INLET, "Messages to convert to a list of bytes.");
    this->addOutlet(BYTES_OUTLET, "Resulting list of bytes.");
}

std::vector<int> blobToBytes(atom::BlobValue::ptr &blob) // FIXME: const
{
    std::vector<int> result;
    size_t size = blob->getSize();
    atom::Byte *value = blob->getValue();
    for (size_t i = 0; i < size; i++)
    {
        result.push_back((int) value[i]);
    }
    return result;
}

bool blobMessageToIntMessage(Message &result, const Message &message)
{
    if (message.getSize() != 1)
    {
        std::ostringstream os;
        os << "Unsuported type tag: " << message.getTypes();
        Logger::log(ERROR, os);
        return false;
    }
    atom::BlobValue::ptr blob = message.getBlob(0);
    std::vector<int> bytes = blobToBytes(blob);
    std::vector<int>::const_iterator iter;
    for (iter = bytes.begin(); iter != bytes.end(); iter++)
    {
        result.appendInt(*iter);
    }
    return true;
}

void BlobToBytesNode::processMessage(const char *inlet, const Message &message)
{
    if (utils::stringsMatch(BLOB_INLET, inlet))
    {
        Message result;
        bool ok = blobMessageToIntMessage(result, message);
        if (ok)
        {
            this->output(BYTES_OUTLET, result);
        }
    }
}

} // end of namespace
} // end of namespace

