/*
 * Copyright (C) 2011 Alexandre Quessy
 *
 * This file is part of Tempi.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software ither version 3 of the License, or
 * (at your option) any later version.
 * 
 * Tempi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with Tempi.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

#include "tempi/dynamicpatching.h"
#include "tempi/log.h"
#include "tempi/graph.h"
#include "tempi/node.h"
#include "tempi/utils.h"
#include <boost/regex.hpp>
#include <algorithm> // std::copy()

namespace tempi {
namespace dynamic {

bool split_path(const char *path, std::vector<std::string> &result)
{
    std::string s(path);
    boost::regex re("/");
    boost::sregex_token_iterator i(s.begin(), s.end(), re, -1);
    boost::sregex_token_iterator j;
 
    unsigned count = 0;
    while (i != j)
    {
        std::ostringstream os;
        os << (*i++);
        if (os.str() != "")
            result.push_back(os.str());
        count++;
    }
    return true;
}

/**
 * Handles messages meant to dynamically alter the Tempi entities managed by a Scheduler.
 *
 * - /graphs/graph0 connect [s:from] [s:outlet] [s:to] [s:inlet]
 * - /graphs/graph0 disconnect [s:from] [s:outlet] [s:to] [s:inlet]
 * - /graphs/graph0 create [s:type] [s:name]
 * - /graphs/graph0 destroy [s:name]
 * - /graphs/graph0 load [s:filename]
 * - /graphs/graph0 save [s:filename]
 * - /graphs/graph0 clear
 * - /graphs/graph0/nodes/metro0/attributes/interval set [i:1000]
 * - /graphs/graph0/nodes/metro0/attributes/interval reset
 * - /graphs/graph0/nodes/midiinput0 list
 *   
 * Shortcuts: (when there is no name clash)
 *
 * - /graph0 create [s:type] [s:name]
 * - /graph0/metro0/interval set [i:1000]
 * - /graph0 save [s:filename]
 */
bool handle_scheduler_message(Scheduler &scheduler, const Message &message)
{
    std::vector<std::string> path;
    std::string types = message.getTypes();

    if (Logger::isEnabledFor(INFO))
    {
        std::ostringstream os;
        os << __FUNCTION__ << ": " << message;
        Logger::log(INFO, os);
    }

    if (! utils::stringBeginsWith(types.c_str(), "s"))
    {
        std::ostringstream os;
        os << __FUNCTION__ << ": first arg should be a string.";
        Logger::log(ERROR, os);
        return false;
    }
    bool ok = split_path(message.getString(0).c_str(), path);
    unsigned int graph_name_index = 0;
    if (path[0] == "graphs" && path.size() > 1)
    {
        graph_name_index = 1;
    }
    std::string graph_name = path[graph_name_index].c_str();
    if (scheduler.hasGraph(graph_name.c_str()))
    {
        std::vector<std::string> new_path;
        std::copy(path.begin() + graph_name_index, path.end(), new_path.begin());
        Message new_message = message.cloneRange(graph_name_index, message.getSize());

        Graph::ptr graph = scheduler.getGraph(graph_name.c_str());
        return handle_graph_message(*graph.get(), new_path, new_message);
    }
    else
    {
        std::ostringstream os;
        os << __FUNCTION__ << ": no such graph: \"" << graph_name;
        Logger::log(ERROR, os);
        return false;
    }
    return false;
}

bool handle_graph_message(Graph &graph, const std::vector<std::string> &path, const Message &message)
{
    if (Logger::isEnabledFor(INFO))
    {
        std::ostringstream os;
        os << __FUNCTION__ << ": ";
        std::vector<std::string>::const_iterator iter;
        for (iter = path.begin(); iter != path.end(); iter++)
            os << (*iter) << " / ";
        os << message;
        Logger::log(INFO, os);
    }

    if (path.size() >= 1)
    {
        if (graph.hasNode(path[0].c_str()))
        {
            Node::ptr node = graph.getNode(path[0].c_str());
            std::vector<std::string> new_path;
            std::copy(path.begin() + 1, path.end(), new_path.begin());
            return handle_node_message(*node.get(), new_path, message);
        }
        else
        {
            std::ostringstream os;
            os << __FUNCTION__ << ": No node named " << path[0];
            Logger::log(ERROR, os);
            return false;
        }
    }

    std::string types = message.getTypes();
    if (utils::stringBeginsWith(types.c_str(), "sss")
        && (message.getString(0) == "setNodeAttribute"))
    {
        if (graph.hasNode(message.getString(1).c_str()))
        {
            Node::ptr node = graph.getNode(message.getString(1).c_str());
            std::vector<std::string> new_path;
            new_path.push_back(message.getString(2)); // attribute name
            Message new_message = message.cloneRange(3, message.getSize() - 1);
            return handle_node_message(*node.get(), new_path, new_message);
        }
        else
        {
            std::ostringstream os;
            os << __FUNCTION__ << ": No node named " << path[0];
            Logger::log(ERROR, os);
            return false;
        }
    }
    else if (utils::stringsMatch(types.c_str(), "sssss")
        && (message.getString(0) == "connect" || message.getString(0) == "disconnect"))
    {
        bool connect = message.getString(0) == "connect";
        std::string from = message.getString(1);
        std::string outlet = message.getString(2);
        std::string to = message.getString(3);
        std::string inlet = message.getString(4);
        std::string string0 = message.getString(0);

        if (Logger::isEnabledFor(DEBUG))
        {
            std::ostringstream os;
            os << __FUNCTION__ << ": ";
            if (! connect)
                os << "dis";
            os << "connect";
            os << message;
            Logger::log(DEBUG, os);
        }
        if (connect)
            return graph.connect(from.c_str(), outlet.c_str(),
                to.c_str(), inlet.c_str());
        else
            return graph.disconnect(from.c_str(), outlet.c_str(),
                to.c_str(), inlet.c_str());
    }
    if (utils::stringsMatch(types.c_str(), "sss")
        && message.getString(0) == "addNode")
    {
        std::string type = message.getString(1);
        std::string name = message.getString(2);
        bool ok = graph.addNode(type.c_str(), name.c_str());
        if (ok)
        {
            if (Logger::isEnabledFor(DEBUG))
            {
                std::ostringstream os;
                os << "did create node " << name << std::endl;
                Logger::log(DEBUG, os);
            }
        }
        else
        {
            std::ostringstream os;
            os << __FUNCTION__ << ": could not create node ";
            Logger::log(ERROR, os);
        }
        return ok;
    }
    if (utils::stringsMatch(types.c_str(), "ss")
        && message.getString(0) == "deleteNode")
    {
        std::string name = message.getString(1);
        return graph.deleteNode(name.c_str());
    }
    return false; // unhandled
}

bool handle_node_message(Node &node, const std::vector<std::string> &path, const Message &message)
{
    std::string attributeName;
    Message value;
    bool method_is_setAttribute = false;
    std::string types = message.getTypes();
    if (path.size() == 1) // attribute name is here
    {
        attributeName = path[0];
        value = message;
        method_is_setAttribute = true;
    }
    else if (utils::stringBeginsWith(types.c_str(), "ss")
        && message.getString(0) == "setAttribute")
    {
        attributeName = message.getString(1);
        value = message.cloneRange(2, message.getSize() - 1);
        method_is_setAttribute = true;
    }
    if (method_is_setAttribute)
    {
        try
        {
            node.setAttributeValue(attributeName.c_str(), value);
            return true;
        }
        catch (const BadAtomTypeException &e)
        {
            std::ostringstream os;
            os << __FUNCTION__ << ": " << e.what();
            Logger::log(ERROR, os);
        }
        catch (const BadIndexException &e)
        {
            std::ostringstream os;
            os << __FUNCTION__ << ": " << e.what();
            Logger::log(ERROR, os);
        }
    }
    else
    {
        std::ostringstream os;
        os << __FUNCTION__ << ": No handler for " << message;
        Logger::log(ERROR, os);
    }
    return false;
}

} // end of namespace
} // end of namespace
