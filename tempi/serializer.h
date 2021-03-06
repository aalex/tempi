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
 * Loading and saving utilities.
 */
#ifndef __TEMPI_SERIALIZER_H__
#define __TEMPI_SERIALIZER_H__

#include "tempi/graph.h"
#include "tempi/sharedptr.h"
#include "tempi/sampler_region.h"
#include <string>

namespace tempi
{

namespace serializer {

const char * const ROOT_NODE = "tempi";
const char * const SOFTWARE_VERSION_ATTR = "version";
const char * const DEFAULT_FILE_NAME = "project.xml";

//const char * const GRAPHS_NODE = "graphs";
const char * const GRAPH_NODE = "graph";
const char * const GRAPH_ID_ATTR = "id";

//const char * const NODES_NODE = "nodes";
const char * const NODE_NODE = "node";
const char * const NODE_ID_PROPERTY = "name";
const char * const NODE_CLASS_PROPERTY = "class";

//const char * const ATTRIBUTES_NODE = "attributes";
const char * const ATTRIBUTE_NODE = "attribute";
const char * const ATTRIBUTE_NAME_PROPERTY = "name";

//const char * const CONNECTIONS_NODE = "connections";
const char * const CONNECTION_NODE = "connection";
const char * const CONNECTION_FROM_PROPERTY = "from";
const char * const CONNECTION_TO_PROPERTY = "to";
const char * const CONNECTION_OUTLET_PROPERTY = "outlet";
const char * const CONNECTION_INLET_PROPERTY = "inlet";

const char * const REGION_NODE = "region";
const char * const REGION_NAME_PROPERTY = "name";

const char * const EVENT_NODE = "event";
const char * const EVENT_TIMEPOSITION_PROPERTY = "time";

/** 
 * Saves a graph to an XML file.
 * @return success
 */
bool save(Graph &graph, const char *filename);
bool save(sampler::Region &region, const char *filename);
/** 
 * Loads a graph from an XML file.
 * @return success
 */
bool load(Graph &graph, const char *filename);
bool load(sampler::Region &region, const char *filename);
/**
 * Checks if a file exists.
 * @return success
 */
bool fileExists(const char *filename);
/**
 * Creates a directory, if it does not exist.
 * @return success
 */
bool createDirectory(const char *dirname);
/**
 * Cleans up XML backend specific memory usage.
 * Call before exiting your app.
 * Watch out : it's not thread-safe!
 */
bool cleanup();

// TODO: isADirectory
// static bool isADirectory(const char *dirname);

} // end of namespace
} // end of namespace

#endif // ifndef

