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

#include <iostream>
#include "plugins-base/flow/send_and_receive_nodes.h"
#include "tempi/graph.h"
#include "tempi/log.h"
#include "tempi/utils.h"

namespace tempi {
namespace plugins_base {

const char * const SendNode::IN_INLET = "in";
const char * const SendNode::IDENTIFIER_ATTR = "identifier";
const char * const SendNode::IS_GLOBAL_ATTR = "is_global";
const char * const ReceiveNode::IDENTIFIER_ATTR = "identifier";
const char * const ReceiveNode::OUT_OUTLET = "out";
const char * const ReceiveNode::IS_GLOBAL_ATTR = "is_global";
const char * const ReceiveNode::NODE_TYPE_NAME = "base.flow.receive";

SendNode::SendNode() :
    Node()
{
    this->setShortDocumentation("Sends messages to the receiver nodes with the same identifier.");
    this->addInlet(IN_INLET, "Messages to send.");
    this->addAttribute(Attribute::ptr(new Attribute(IDENTIFIER_ATTR, Message("s", "default"), "The identifier of this sender/receiver.")));
    this->addAttribute(Attribute::ptr(new Attribute(IS_GLOBAL_ATTR, Message("b", false), "If true, will be able to communicate with any global sender/receiver in any graph. Otherwise, it is limited to the ones in the same graph.")));
}

void SendNode::processMessage(const char *inlet, const Message &message)
{
    if (utils::stringsMatch(inlet, IN_INLET))
    {
        this->send(message);
    }
}

void SendNode::send(const Message &message)
{
    if (this->getGraph() == 0)
    {
        std::ostringstream os;
        os << "SendNode." << __FUNCTION__ << ": " <<
        "this node must belong to a graph.";
        Logger::log(ERROR, os);
    }
    else
    {
        bool is_global = this->getAttributeValue(IS_GLOBAL_ATTR).getBoolean(0);
        if (is_global)
        {
            std::ostringstream os;
            os << "SendNode." << __FUNCTION__ << ": " <<
            "global send/receive symbols are not yet supported.";
            Logger::log(WARNING, os);
        }

        std::vector<std::string> nodes = this->getGraph()->getNodeNames();

        std::vector<std::string>::const_iterator iter;
        std::string identifier = this->getAttributeValue(IDENTIFIER_ATTR).getString(0);
        for (iter = nodes.begin(); iter != nodes.end(); ++iter)
        {
            Node::ptr node = this->getGraph()->getNode((*iter).c_str());
            if (node->getTypeName() == ReceiveNode::NODE_TYPE_NAME)
            {
                if (Logger::isEnabledFor(INFO))
                {
                    std::ostringstream os;
                    os << "Will cast node* to ReceiveNode*";
                    Logger::log(INFO, os);
                }
                ReceiveNode *receiver = (ReceiveNode*) node.get(); // FIXME
                if (receiver->getAttributeValue(ReceiveNode::IDENTIFIER_ATTR).getString(0) == identifier)
                    receiver->receive(message);
            }
        } // for
    }
}

ReceiveNode::ReceiveNode() :
    Node()
{
    this->setShortDocumentation("Receives messages from the sender nodes with the same identifier.");
    this->addOutlet(OUT_OUTLET, "Received messages.");
    this->addAttribute(Attribute::ptr(new Attribute(IDENTIFIER_ATTR, Message("s", "default"), "The identifier of this sender/receiver.")));
    this->addAttribute(Attribute::ptr(new Attribute(IS_GLOBAL_ATTR, Message("b", false), "If true, will be able to communicate with any global sender/receiver in any graph. Otherwise, it is limited to the ones in the same graph.")));
}

void ReceiveNode::processMessage(const char *inlet, const Message &message)
{
    return;
}

void ReceiveNode::receive(const Message &message)
{
    this->output(OUT_OUTLET, message);
}

} // end of namespace
} // end of namespace

