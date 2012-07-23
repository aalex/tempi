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
 * Functions to draw user interface elements.
 */

#ifndef __MILLER_DRAW_H__
#define __MILLER_DRAW_H__

#include "tempi/config.h"
#ifdef HAVE_CLUTTER

#include <clutter/clutter.h>
#include "tempi/graph.h"
#include <sstream>

namespace miller {

std::string makePadName(const char *node_name, const char *pad_name, bool is_inlet);
std::string makeNodeName(const char *node_name);
std::string makeConnectionName(const char *from_node, const char *outlet, const char *to_node, const char *inlet);

ClutterActor* createNodeActor(tempi::Node &node);
ClutterActor* createConnectionActor(const char *source_node, const char *outlet, const char *sink_node, const char *inlet);
ClutterActor* createConnectionsActor(tempi::Graph &graph);
void updateAllConnectionsGeometry(ClutterActor *connectionsGroup, ClutterActor *nodesGroup, tempi::Graph &graph);

} // end of namespace

#endif // ifdef
#endif // ifndef

