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

#include "plugins-base/serial/bytestoblobnode.h"
#include "tempi/utils.h"
#include "tempi/log.h"

namespace tempi {
namespace plugins_base {

const char * const BytesToBlobNode::BYTES_INLET = "bytes";
const char * const BytesToBlobNode::BLOB_OUTLET = "blob";

BytesToBlobNode::BytesToBlobNode() :
    Node()
{
    this->setShortDocumentation("Converts lists of bytes to blobs.");
    this->setLongDocumentation("Send messages containing a list of integers to its bytes input. It will output a message containing a single blob to its output.");

    this->addInlet(BYTES_INLET, "Messages to convert to a blob. Must be only integers between 0 and 255.");
    this->addOutlet(BLOB_OUTLET, "Resulting blobs.");
}

static atom::BlobValue::ptr createEmptyBlob()
{
    char empty = 0;
    atom::Value::ptr value = atom::BlobValue::create(&empty, 0);
    return atom::BlobValue::convert(value);
}

atom::BlobValue::ptr bytesToBlob(const std::vector<int> &bytes)
{
    atom::BlobValue::ptr blob = createEmptyBlob();
    std::vector<int>::const_iterator iter;
    for (iter = bytes.begin(); iter != bytes.end(); iter++)
    {
        char byte = (char) *iter;
        blob->append(&byte, 1);
    }
    return blob;
}

bool messageToBytes(std::vector<int> &bytes, const Message &message)
{
    bool ok = true;
    for (unsigned int i = 0; i < message.getSize(); i++)
    {
        AtomType type;
        message.getAtomType(i, type);
        if (type == INT)
        {
            bytes.push_back((int) (message.getInt(i) & 0xff));
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

void BytesToBlobNode::processMessage(const char *inlet, const Message &message)
{
    if (utils::stringsMatch(BYTES_INLET, inlet))
    {
        std::vector<int> bytes;
        bool ok = messageToBytes(bytes, message);
        if (! ok)
        {
            std::ostringstream os;
            os << "BytesToBlobNode::" << __FUNCTION__ << ": Could not convert all atoms to bytes." << message.getTypes();
            Logger::log(ERROR, os);
        }
        atom::BlobValue::ptr blob = bytesToBlob(bytes);
        Message result;
        result.appendBlob(blob);
        this->output(BLOB_OUTLET, result);
    }
}

} // end of namespace
} // end of namespace

