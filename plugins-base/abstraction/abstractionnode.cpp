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

#include "plugins-base/abstraction/abstractionnode.h"
#include "plugins-base/abstraction/outletnode.h"
#include "plugins-base/abstraction/inletnode.h"
#include "tempi/serializer.h"
#include "tempi/utils.h"
#include "tempi/log.h"
#include <iostream>

namespace tempi {
namespace plugins_base {

const char * const AbstractionNode::INLET_NODE_NAME = "abs.inlet";
const char * const AbstractionNode::OUTLET_NODE_NAME = "abs.outlet";

static std::vector<std::string> listNodeWhoseTypeIs(
    const Graph &graph, const char *typeName);

static bool hasNodeOfType(
    Graph &graph, const std::string &nodeName, const std::string &nodeType);

AbstractionNode::AbstractionNode() :
    Node(),
    file_path_(""),
    scheduler_(new SynchronousScheduler)
{
    this->setShortDocumentation("Loads a Graph from an XML file and allows one to send and receives messages to and from its nodes.");
    this->setLongDocumentation(
        "Tempi XML files should comply with the Tempi standard. "
        "If there are [abs.inlet] or [abs.outlet] nodes in the XML file that is loaded, Tempi will create inlets and outlets to this [abs.abstraction] node, and these pads will be named the same way as the nodes within the abstractions. "
        "That means that if you have a [abs.inlet] whose name is \"hello\" in the XML file loaded by the [abs.abstraction] node, the [abs.abstraction] will have an inlet named \"hello\". "
        );
    this->addAttribute(Attribute::ptr(new Attribute("file_path", Message("s", ""), "Path to the XML file to load the Tempi Graph from.")));
}

bool AbstractionNode::onNodeAttributeChanged(const char *name, const Message &value)
{
    if (Logger::isEnabledFor(DEBUG))
    {
        std::ostringstream os;
        os << "AbstractionNode::" << __FUNCTION__ << "(\"" << name << "\", " << value << ")";
        Logger::log(DEBUG, os.str().c_str());
    }
    if (utils::stringsMatch("file_path", name))
    {
        std::string path = value.getString(0);
        if (path == "")
        {
            if (Logger::isEnabledFor(INFO))
            {
                std::ostringstream os;
                os << "AbstractionNode::" << __FUNCTION__ << ": Emtpy file_path string doesn't load any patch." << path;
                Logger::log(INFO, os.str().c_str());
            }
            return false;
        }
        if (path == file_path_)
        {
            std::ostringstream os;
            os << "AbstractionNode::" << __FUNCTION__ << " already loaded file at path " << path;
            Logger::log(INFO, os.str().c_str());
            return false;
        }
        file_path_ = path;
        {
            std::ostringstream os;
            os << "AbstractionNode::" << __FUNCTION__ << " load file " << file_path_;
            Logger::log(INFO, os.str().c_str());
        }
        if (file_path_ == "")
            deleteGraph();
        else
            loadGraph();
        return true;
    }
}

bool AbstractionNode::deleteGraph()
{
    if (scheduler_->hasGraph("graph0"))
    {
        scheduler_->removeGraph("graph0");
        return true;
    }
    return false;
}

bool AbstractionNode::loadGraph()
{
    scheduler_->setFactory(this->getGraph()->getScheduler()->getFactory()); // FIXME

    if (scheduler_->hasGraph("graph0"))
        scheduler_->removeGraph("graph0");
    scheduler_->createGraph("graph0");

    // Check for XML file
    if (! serializer::fileExists(file_path_.c_str()))
    {
        std::ostringstream os;
        os << "AbstractionNode: ERROR: File \"" << file_path_ << "\" not found!\n";
        tempi::Logger::log(ERROR, os.str().c_str());
        return false;
    }
    else
    {
        std::ostringstream os;
        os << "AbstractionNode: Found " << file_path_;
        tempi::Logger::log(INFO, os.str().c_str());
    }
    graph_ = scheduler_->getGraph("graph0");

    // load graph
    bool ok = serializer::load(*graph_.get(), file_path_.c_str());
    if (! ok)
    {
        std::ostringstream os;
        os << "AbstractionNode: ERROR: Serializer could not load abstraction \"" << file_path_ << "\" even if we found the file!\n";
        tempi::Logger::log(ERROR, os.str().c_str());
        deleteGraph();
        return false;
    }
    graph_->tick(); // FIXME

    {
        std::ostringstream os;
        os << "AbstractionNode: Loaded " << file_path_;
        tempi::Logger::log(DEBUG, os.str().c_str());
    }
    loadInletsAndOutlets();
    return true;
}

bool AbstractionNode::loadInletsAndOutlets()
{
    std::vector<std::string>::const_iterator iter;
    for (iter = inlet_names_.begin(); iter != inlet_names_.end(); ++iter)
        this->removeInlet((*iter).c_str());
    for (iter = outlet_names_.begin(); iter != outlet_names_.end(); ++iter)
        this->removeOutlet((*iter).c_str());

    inlet_names_.clear();
    outlet_names_.clear();

    std::vector<std::string> new_inlet_names =
        listNodeWhoseTypeIs(*(graph_.get()), INLET_NODE_NAME);
    std::vector<std::string> new_outlet_names =
        listNodeWhoseTypeIs(*(graph_.get()), OUTLET_NODE_NAME);

    for (iter = new_inlet_names.begin(); iter != new_inlet_names.end(); ++iter)
    {
        if (! this->hasInlet((*iter).c_str()))
        {
            inlet_names_.push_back(*iter);
            this->addInlet((*iter).c_str(), "Inlet in the abstraction.");
        }
    }
    for (iter = new_outlet_names.begin(); iter != new_outlet_names.end(); ++iter)
    {
        if (! this->hasOutlet((*iter).c_str()))
        {
            outlet_names_.push_back(*iter);
            this->addOutlet((*iter).c_str(), "Outlet in the abstraction.");
        }
    }
}

bool hasNodeOfType(Graph &graph, const std::string &nodeName, const char *nodeType)
{
    if (graph.hasNode(nodeName.c_str()))
    {
        Node *node = graph.getNode(nodeName.c_str()).get();
        if (node->getTypeName() == nodeType)
        {
            return true;
        }
        else
        {
            std::ostringstream os;
            os << "AbstractionNode." << __FUNCTION__ << ": Wrong type for node " << nodeName << ": expected " << nodeType << " but got " << node->getTypeName();
            Logger::log(ERROR, os.str().c_str());
            return false;
        }
    }
    else
    {
        std::ostringstream os;
        os << "AbstractionNode." << __FUNCTION__ << ": No such node: " << nodeName;
        Logger::log(ERROR, os.str().c_str());
        return false;
    }
}

void AbstractionNode::doTick()
{
    if (graph_.get() != 0)
    {
        //graph_->tick();
        scheduler_->tick();
    }
    // poll the abstraction's outlets
    std::vector<std::string>::const_iterator iter;
    for (iter = outlet_names_.begin(); iter != outlet_names_.end(); ++iter)
    {
        std::vector<Message> messages;
        if (hasNodeOfType(*(graph_.get()), (*iter), OUTLET_NODE_NAME))
        {
            Node *node = graph_->getNode((*iter).c_str()).get();
            OutletNode *outletNode = dynamic_cast<OutletNode*>(node);
            std::vector<Message> messages;
            outletNode->flush(messages);
            std::vector<Message>::const_iterator msg_iter;
            for (msg_iter = messages.begin(); msg_iter != messages.end(); ++msg_iter)
            {
                if (this->hasOutlet((*iter).c_str()))
                    this->output((*iter).c_str(), (*msg_iter));
                else
                {
                    std::ostringstream os;
                    os << "AbstractionNode." << __FUNCTION__ << ": No such outlet: " << (*iter);
                    Logger::log(ERROR, os.str().c_str());
                } // if has such outlet
            } // for message
        } // if flow.outlet with that name
    } // for outlet_names_
}

void AbstractionNode::processMessage(const char *inlet, const Message &message)
{
    std::vector<std::string>::const_iterator iter;
    for (iter = inlet_names_.begin(); iter != inlet_names_.end(); ++iter)
    {
        if (utils::stringsMatch(inlet, (*iter).c_str()))
        {
            if (this->hasInlet((*iter).c_str()))
            {
                if (hasNodeOfType(*(graph_.get()), (*iter), INLET_NODE_NAME))
                {
                    Node *node = graph_->getNode((*iter).c_str()).get();
                    InletNode *inletNode = dynamic_cast<InletNode*>(node);
                    inletNode->push(message);
                }
            }
            else
            {
                std::ostringstream os;
                os << "AbstractionNode." << __FUNCTION__ << ": No such inlet: " << (*iter);
                Logger::log(ERROR, os.str().c_str());
            } // if has such inlet
        }
    }
}

std::vector<std::string> listNodeWhoseTypeIs(const Graph &graph, const char *typeName)
{
    std::vector<std::string> ret;
    std::vector<std::string> nodeNames = graph.getNodeNames();
    std::vector<std::string>::const_iterator iter;
    for (iter = nodeNames.begin(); iter != nodeNames.end(); ++iter)
    {
        if (graph.getNode((*iter).c_str())->getTypeName() == typeName)
            ret.push_back((*iter));
    }
    return ret;
}

} // end of namespace
} // end of namespace

