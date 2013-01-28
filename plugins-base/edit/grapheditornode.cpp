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

#include "plugins-base/edit/grapheditornode.h"
#include "tempi/log.h"
#include "tempi/dynamicpatching.h"
#include <iostream>
#include <string>

namespace tempi {
namespace plugins_base {

const char * const GraphEditorNode::COMMAND_INLET = "command";
const char * const GraphEditorNode::SUCCESS_OUTLET = "success";

GraphEditorNode::GraphEditorNode() :
    Node()
{
    this->setShortDocumentation("Edits the current graph.");
    this->setLongDocumentation("This node allows one to edit the graph it belongs to.\n"
        "It does so by calling the tempi::dynamic::handle_graph_message function.\n"
        "\n"
        "* <node> setAttribute <attr> ...\n"
        "* connect <from> <outlet> <to> <inlet>\n"
        "* disconnect <from> <outlet> <to> <inlet>\n"
        "* addNode <type> <name>\n" // FIXME: rename to createNode
        "* deleteNode <name>\n" // FIXME: rename to destroyNode
        );
    this->addInlet(COMMAND_INLET, "Command to execute.");
    this->addOutlet(SUCCESS_OUTLET, "Success or not of the last command. Outputs a boolean.");
}

void GraphEditorNode::processMessage(const char *inlet, const Message &message)
{
    if (! utils::stringsMatch(inlet, COMMAND_INLET))
        return;
    if (Logger::isEnabledFor(DEBUG))
    {
        std::ostringstream os;
        os << "GraphEditorNode::" << __FUNCTION__ << ": " << message;
        Logger::log(DEBUG, os);
    }
    Graph *graph = this->getGraph();
    if (graph == 0)
    {
        std::ostringstream os;
        os << "GraphEditorNode: graph* is NULL";
        Logger::log(ERROR, os);
    }
    std::vector<std::string> path;
    Message args = message;
    // TODO: handle is there is some path provided
    bool success = dynamic::handle_graph_message(*graph, path, args);
    this->output(SUCCESS_OUTLET, Message("b", success));
}

} // end of namespace
} // end of namespace

