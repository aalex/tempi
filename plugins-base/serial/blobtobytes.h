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
 * The BlobToBytesNode class.
 */
#ifndef __TEMPI_SERIAL_BLOBTOBYTESNODE_H__
#define __TEMPI_SERIAL_BLOBTOBYTESNODE_H__

#include "tempi/node.h"
#include <vector>

namespace tempi {
namespace plugins_base {

std::vector<int> blobToBytes(atom::BlobValue::ptr &blob); // FIXME: should be const

/**
 * Node that converts a blob message to a list of bytes message.
 */
class BlobToBytesNode : public Node
{
    public:
        BlobToBytesNode();
        static const char * const BLOB_INLET;
        static const char * const BYTES_OUTLET;
    protected:
        virtual void processMessage(const char *inlet, const Message &message);
};

} // end of namespace
} // end of namespace

#endif // ifndef

