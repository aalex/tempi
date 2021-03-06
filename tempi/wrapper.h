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

/**
 * @file
 * The Wrapper class
 */

#ifndef __TEMPI_WRAPPER_H__
#define __TEMPI_WRAPPER_H__

#include <iostream>
#include <string>
#include <vector>
#include "tempi/message.h"
#include "tempi/documented.h"
#include "tempi/scheduler.h"
#include "tempi/serializer.h"

namespace tempi {

/**
 * Wraps the whole Tempi library for easy use in C++ programs.
 */
class Wrapper
{
    public:
        /**
         * Constructor.
         * @param synchronous If true, you must call tick() repeatedly so that the graphs do their thing, (30 Hz is fine) otherwise it is ticked in a separate thread and there is no need to tick it.
         */
        Wrapper(); //bool synchronous);
        ~Wrapper();
        /**
         * Set log level.
         * @param level Any of "DEBUG", "INFO",
         * "NOTICE", "WARNING", "CRITICAL" or "ERROR".
         */
        bool setLogLevel(const std::string &level);
        /**
         * Loads a graph from an XML file.
         * Graph name will be created, and should not exist prior to calling this.
         */
        bool loadGraph(const std::string &name, const std::string &fileName);
        /**
         * save a graph to an XML file.
         */
        bool saveGraph(const std::string &name, const std::string &fileName);
        bool getSynchronous() const;
        /**
         * Call only if synchronous is false.
         */
        bool tick();

        // TODO: return vector<string>
        bool listGraphs(std::vector<std::string> &names) const;
        bool createGraph(const std::string &name);
        bool destroyGraph(const std::string &name);
        bool hasGraph(const std::string &name);

        bool createNode(const std::string &graph, const std::string &nodeType,
            const std::string &nodeName);
        bool getNode(const std::string &graph, const std::string &nodeName, Node::ptr &result);
        bool destroyNode(const std::string &graph, const std::string &nodeName);
        bool messageInlet(const std::string &graph, const std::string &nodeName,
            const std::string &nodeInlet, const Message &message);
        bool connect(const std::string &graph,
            const std::string &from, const std::string &outlet,
            const std::string &to, const std::string &inlet);
        bool disconnect(const std::string &graph,
            const std::string &from, const std::string &outlet,
            const std::string &to, const std::string &inlet);

        // TODO: return vector<string>
        bool listNodeTypes(std::vector<std::string> &names) const;

        bool setNodeAttributeValue(
            const std::string &graph,
            const std::string &node,
            const std::string &attribute,
            const Message &value);

        // TODO: return vector<string>
        bool listNodeAttributes(
            const std::string &graph,
            const std::string &node,
            std::vector<std::string> &names) const;

        // TODO: return vector<string>
        bool listNodeMethods(
            const std::string &graph,
            const std::string &node,
            std::vector<std::string> &names) const;

        // TODO: return vector<string>
        bool listNodeInlets(
            const std::string &graph,
            const std::string &node,
            std::vector<std::string> &names) const;

        // TODO: return vector<string>
        bool listNodeOutlets(
            const std::string &graph,
            const std::string &node,
            std::vector<std::string> &names) const;

        // TODO: return string
        bool getNodeTypeName(
            const std::string &graph,
            const std::string &node,
            std::string &typeName) const;

        // TODO: return Message
        bool getNodeAttributeValue(
            const std::string &graph,
            const std::string &node,
            const std::string &attribute,
            Message &value) const;

        // TODO: return string
        bool getNodeDocumentation(
            const std::string &graph,
            const std::string &node,
            std::string &value) const;

        // TODO: return string
        bool getNodeAttributeDocumentation(
            const std::string &graph,
            const std::string &node,
            const std::string &attribute,
            std::string &value) const;

        // TODO: return string
        bool getNodeInletDocumentation(
            const std::string &graph,
            const std::string &node,
            const std::string &inlet,
            std::string &value) const;

        // TODO: return string
        bool getNodeOutletDocumentation(
            const std::string &graph,
            const std::string &node,
            const std::string &outlet,
            std::string &value) const;

        // TODO
        bool addLibraryPath(const std::string &path);

        // TODO
        bool loadLibrary(const std::string &name);

        std::vector<std::string> listNodes(const std::string &graph);
        bool nodeHasAttribute(const std::string &graph, const std::string &nodeName, const std::string &attributeName);

        bool callNodeMethod(
            const std::string &graph,
            const std::string &node,
            const std::string &method,
            const Message &arguments,
            Message &return_value);

        bool sleep(double duration_ms);
        bool waitUntilAllNodesAreInitiated(const std::string &graph);
        bool waitUntilNodesAreInitiated(const std::string &graph, const std::vector<std::string> &nodes);

        bool isConnected(const std::string &graph, const std::string &node_from, const std::string &outlet, const std::string &node_to, const std::string &inlet);
        /**
         * Result will be a list of string in the form "from:outlet:to:inlet", with semicolons between each entity name.
         * That is possible since semicolons are forbidden in entity names.
         */
        bool listConnections(const std::string &graph, std::vector<std::string> &result);
    private:
        bool synchronous_;
        Scheduler *scheduler_;
        // TODO
        bool setSynchronous(bool synchronous);
};

} // end of namespace

#endif // ifndef

