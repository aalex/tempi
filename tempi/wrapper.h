 /*
 * Copyright (C) 2011 Alexandre Quessy
 * Copyright (C) 2011 Michal Seta
 * Copyright (C) 2012 Nicolas Bouillot
 *
 * This file is part of Tempi.
 *
 * This program is free software: you can redistribute it and/or
 * modify it under the terms of, either version 3 of the License, or
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
        Wrapper(bool synchronous=true);
        ~Wrapper();
        /**
         * Set log level.
         * @param level Any of "DEBUG", "INFO",
         * "NOTICE", "WARNING", "CRITICAL" or "ERROR".
         */
        bool setLogLevel(const char *level);
        /**
         * Loads a graph from an XML file.
         */
        bool loadGraph(const char *name, const char *fileName);
        bool saveGraph(const char *name, const char *fileName);
        bool getSynchronous() const;
        /**
         * Call only if synchronous is false.
         */
        bool tick();

        bool listGraphs(std::vector<std::string> &names) const;
        bool createGraph(const char *name);
        bool destroyGraph(const char *name);
        bool hasGraph(const char *name);

        bool createNode(const char *graph, const char *nodeType,
            const char *nodeName);
        bool destroyNode(const char *graph, const char *nodeName);
        bool messageInlet(const char *graph, const char *nodeName,
            const char *nodeInlet, const Message &message);
        bool connect(const char *graph,
            const char *from, const char *outlet,
            const char *to, const char *inlet);
        bool disconnect(const char *graph,
            const char *from, const char *outlet,
            const char *to, const char *inlet);

        bool listNodeTypes(std::vector<std::string> &names) const;

        bool setNodeAttributeValue(
            const char *graph,
            const char *node,
            const char *attribute,
            const Message &value);

        bool listNodeAttributes(
            const char *graph,
            const char *node,
            std::vector<std::string> &names) const;
        bool listNodeMethods(
            const char *graph,
            const char *node,
            std::vector<std::string> &names) const;
        bool listNodeInlets(
            const char *graph,
            const char *node,
            std::vector<std::string> &names) const;
        bool listNodeOutlets(
            const char *graph,
            const char *node,
            std::vector<std::string> &names) const;
        bool getNodeTypeName(
            const char *graph,
            const char *node,
            std::string &typeName) const;
        bool getNodeAttributeValue(
            const char *graph,
            const char *node,
            const char *attribute,
            Message &value) const;
        bool getNodeAttributeDocumentation(
            const char *graph,
            const char *node,
            const char *attribute,
            std::string &value) const;
        bool getNodeInletDocumentation(
            const char *graph,
            const char *node,
            const char *inlet,
            std::string &value) const;
        bool getNodeOutletDocumentation(
            const char *graph,
            const char *node,
            const char *outlet,
            std::string &value) const;

        // TODO
        bool addLibraryPath(const char *path);
        // TODO
        bool loadLibrary(const char *name);

    private:
        bool synchronous_;
        Scheduler::ptr scheduler_;
        serializer::Serializer::ptr saver_;
};

} // end of namespace

#endif // ifndef

