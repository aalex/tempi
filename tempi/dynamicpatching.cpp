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
#include <boost/regex.hpp>

namespace tempi {
namespace dynamic {

// /**
//  * Return true if handled.
//  */
// bool Graph::handleMessage(const Message &message)
// {
//     //std::cout << "Graph::" << __FUNCTION__ << "(" << message << ")" << std::endl;
//     std::string types = message.getTypes();
//     if (utils::stringBeginsWith(types.c_str(), "s")
//         && message.getString(0) == "__tempi__")
//     {
//         //std::cout << __FILE__ << "::" << __FUNCTION__ << ": starts with tempi" << std::endl;
//         return handleTempiMessage(
//             message.cloneRange(1, message.getSize() - 1));
//     }
//     else if (utils::stringBeginsWith(types.c_str(), "s"))
//     {
//         std::string receiveSlot = message.getString(0);
//         std::cout << "TODO: Graph::" << __FUNCTION__ << "(" << message << ")" << std::endl;
//         return false;
//     }
// }
// 
// /**
//  * Handles messages meant to dynamically patch the graph.
//  * - ,ssisi: connect [from] [outlet] [to] [inlet]
//  * - ,sss: addNode [type] [name]
//  * - ,ss: deleteNode [name]
//  * - ,ss...: setNodeAttribute [nodeName] [prop] ...
//  */
// bool Graph::handleTempiMessage(const Message &message)
// {
//     std::string types = message.getTypes();
//     if (utils::stringsMatch(types.c_str(), "ssisi")
//         && message.getString(0) == "connect")
//     {
//         std::string from = message.getString(1);
//         unsigned int outlet = (unsigned) message.getInt(2);
//         std::string to = message.getString(3);
//         unsigned int inlet = (unsigned) message.getInt(4);
//         std::string string0 = message.getString(0);
//         return connect(from.c_str(), outlet,
//             to.c_str(), inlet);
//     }
//     if (utils::stringsMatch(types.c_str(), "sss")
//         && message.getString(0) == "addNode")
//     {
//         std::string type = message.getString(1);
//         std::string name = message.getString(2);
//         bool ok = addNode(type.c_str(), name.c_str());
//         if (ok)
//             std::cout << "did create node " << name << std::endl;
//         return ok;
//     }
//     if (utils::stringsMatch(types.c_str(), "ss")
//         && message.getString(0) == "deleteNode")
//     {
//         std::string name = message.getString(1);
//         return deleteNode(name.c_str());
//     }
//     if (utils::stringBeginsWith(types.c_str(), "sss")
//         && message.getString(0) == "setNodeAttribute")
//     {
//         std::string nodeName = message.getString(1);
//         std::string attributeName = message.getString(2);
//         Message value = message.cloneRange(3, message.getSize() - 1);
//         return setNodeAttribute(nodeName.c_str(),
//             attributeName.c_str(), value);
//     }
//     return false; // unhandled
// }
//


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
bool handle_message(Scheduler &scheduler, const Message &message)
{
    return false;
}

} // end of namespace
} // end of namespace
