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
 * The BytesToBlobNode class.
 */
#ifndef __TEMPI_SERIAL_BYTESTOBLOBNODE_H__
#define __TEMPI_SERIAL_BYTESTOBLOBNODE_H__

#include "tempi/node.h"
#include <vector>

namespace tempi {
namespace plugins_base {

atom::BlobValue::ptr bytesToBlob(const std::vector<char> &bytes);
bool messageToBytes(std::vector<char> &bytes, const Message &message);

/**
 * Node that converts a list of bytes to a blob.
 */
class BytesToBlobNode : public Node
{
    public:
        BytesToBlobNode();
        static const char * const BYTES_INLET;
        static const char * const BLOB_OUTLET;
    protected:
        virtual void processMessage(const char *inlet, const Message &message);
};

} // end of namespace
} // end of namespace

#endif // ifndef

