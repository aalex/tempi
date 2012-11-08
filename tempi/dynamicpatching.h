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
 * Dynamic patching utilities.
 */
#ifndef __TEMPI_DYNAMIC_PATCHING_H__
#define __TEMPI_DYNAMIC_PATCHING_H__

#include "tempi/graph.h"
#include "tempi/message.h"
#include "tempi/scheduler.h"
#include <vector>
#include <string>

namespace tempi {
namespace dynamic {

bool handle_scheduler_message(Scheduler &scheduler, const Message &message);
bool handle_graph_message(Graph &graph, const std::vector<std::string> &path, const Message &message);
bool handle_node_message(Node &node, const std::vector<std::string> &path, const Message &message);
bool split_path(const char *path, std::vector<std::string> &result);

} // end of namespace
} // end of namespace

#endif // ifndef

