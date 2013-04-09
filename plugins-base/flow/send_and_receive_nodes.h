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
 * The SendNode and ReceiveNode classes.
 */
#ifndef __TEMPI_BASE_SEND_AND_RECEIVE_NODES_H__
#define __TEMPI_BASE_SEND_AND_RECEIVE_NODES_H__

#include "tempi/node.h"

namespace tempi {
namespace plugins_base {


/**
 * Sends messages.
 */
class SendNode : public Node
{
    public:
        SendNode();
        static const char * const IN_INLET;
        static const char * const IDENTIFIER_ATTR;
        static const char * const IS_GLOBAL_ATTR;
    private:
        virtual void processMessage(const char *inlet, const Message &message);
        void send(const Message &message);
};

/**
 * Receives messages.
 */
class ReceiveNode : public Node
{
    public:
        ReceiveNode();
        static const char * const IDENTIFIER_ATTR;
        static const char * const OUT_OUTLET;
        static const char * const IS_GLOBAL_ATTR;
        static const char * const NODE_TYPE_NAME;
        void receive(const Message &message);
    private:
        virtual void processMessage(const char *inlet, const Message &message);
};

} // end of namespace
} // end of namespace

#endif // ifndef

