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

#include "tempi/config.h"
#ifdef HAVE_CLUTTER

#include "miller-draw.h"
#include "miller-macros.h"
#include "tempi/log.h"
#include <iostream>
#include "miller-connection.h"

namespace miller {

std::string makePadName(const char *node_name, const char *pad_name, bool is_inlet)
{
    std::ostringstream os;
    os << node_name;
    if (is_inlet)
        os << ":inlet:";
    else
        os << ":outlet:";
    os << pad_name;
    return os.str();
}

std::string parseOnlyPadNameFromPadName(const char *pad_name)
{

}

std::string makeNodeName(const char *node_name)
{
    // just to make sure we are consistent
    return std::string(node_name);
}

ClutterActor* createPadActor(const char *node_name, const char *pad_name, bool is_inlet)
{
    ClutterLayoutManager *layout = clutter_bin_layout_new(
        CLUTTER_BIN_ALIGNMENT_CENTER,
        CLUTTER_BIN_ALIGNMENT_CENTER);
    ClutterActor *background = clutter_rectangle_new_with_color(&WHITE);
    clutter_actor_set_size(background, 1.0f, 1.0f);
    ClutterActor *box = clutter_box_new(layout);
    clutter_bin_layout_add(CLUTTER_BIN_LAYOUT(layout),
        background,
        CLUTTER_BIN_ALIGNMENT_FILL,
        CLUTTER_BIN_ALIGNMENT_FILL);
    ClutterActor *label = clutter_text_new_full(FONT_NAME, pad_name, &RED);
    clutter_bin_layout_add(CLUTTER_BIN_LAYOUT(layout),
        label,
        CLUTTER_BIN_ALIGNMENT_CENTER,
        is_inlet ? CLUTTER_BIN_ALIGNMENT_START : CLUTTER_BIN_ALIGNMENT_END);
    clutter_actor_set_name(box, makePadName(node_name, pad_name, is_inlet).c_str());
    return box;
}

ClutterActor* createNodeActor(tempi::Node &node)
{
    const std::string& node_type = node.getTypeName();
    std::string node_name = node.getName();

    std::ostringstream os;
    os << node_type << " (" << node_name << ")";

    ClutterLayoutManager *node_layout = clutter_bin_layout_new(
        CLUTTER_BIN_ALIGNMENT_CENTER,
        CLUTTER_BIN_ALIGNMENT_CENTER);
    ClutterActor *node_box = clutter_box_new(node_layout);

    // FIXME: ClutterRectangle is deprecated!
    ClutterActor *background = clutter_rectangle_new_with_color(&GRAY_LIGHT);
    clutter_actor_set_size(background, 1.0f, 1.0f);
    clutter_bin_layout_add(CLUTTER_BIN_LAYOUT(node_layout),
        background,
        CLUTTER_BIN_ALIGNMENT_FILL,
        CLUTTER_BIN_ALIGNMENT_FILL);

    ClutterLayoutManager *table_layout = clutter_table_layout_new();
    ClutterActor *table_box = clutter_box_new(table_layout);
    clutter_bin_layout_add(CLUTTER_BIN_LAYOUT(node_layout),
        table_box,
        CLUTTER_BIN_ALIGNMENT_CENTER,
        CLUTTER_BIN_ALIGNMENT_START);
    guint SPACING = 2;
    clutter_table_layout_set_row_spacing(CLUTTER_TABLE_LAYOUT(table_layout), SPACING);
    clutter_table_layout_set_column_spacing(CLUTTER_TABLE_LAYOUT(table_layout), SPACING);
    
    ClutterActor *label = clutter_text_new_full(FONT_NAME, os.str().c_str(), &BLUE);
    clutter_table_layout_pack(CLUTTER_TABLE_LAYOUT(table_layout), label,
        0, 0); // col, row
    clutter_table_layout_set_alignment(CLUTTER_TABLE_LAYOUT(table_layout), label,
        CLUTTER_TABLE_ALIGNMENT_START,
        CLUTTER_TABLE_ALIGNMENT_START);
    clutter_table_layout_set_span(CLUTTER_TABLE_LAYOUT(table_layout), label,
        2, 1); // col span, row span

    gint inlet_row = 1;
    gint outlet_row = 1;
    const gint inlet_col = 0;
    const gint outlet_col = 1;
    // inlets:
    std::map<std::string, tempi::Inlet::ptr> inlets = node.getInlets();
    std::map<std::string, tempi::Inlet::ptr>::const_iterator iter_inlet;
    for (iter_inlet = inlets.begin(); iter_inlet != inlets.end(); ++iter_inlet)
    {
        std::string pad_name = (*iter_inlet).first;
        ClutterActor *pad_actor = createPadActor(node_name.c_str(), pad_name.c_str(), true); // is an inlet
        clutter_table_layout_pack(CLUTTER_TABLE_LAYOUT(table_layout), pad_actor,
            inlet_col,
            inlet_row); // col, row
        clutter_table_layout_set_alignment(CLUTTER_TABLE_LAYOUT(table_layout), pad_actor,
            CLUTTER_TABLE_ALIGNMENT_START, // x_align
            CLUTTER_TABLE_ALIGNMENT_START); // y_align
        inlet_row++;
    }
    // outlets:
    std::map<std::string, tempi::Outlet::ptr> outlets = node.getOutlets();
    std::map<std::string, tempi::Outlet::ptr>::const_iterator iter_outlet;
    for (iter_outlet = outlets.begin(); iter_outlet != outlets.end(); ++iter_outlet)
    {
        std::string pad_name =  (*iter_outlet).first;
        ClutterActor *pad_actor = createPadActor(node_name.c_str(), pad_name.c_str(), false); // is not an inlet
        clutter_table_layout_pack(CLUTTER_TABLE_LAYOUT(table_layout), pad_actor,
            outlet_col,
            outlet_row); // col, row
        clutter_table_layout_set_alignment(CLUTTER_TABLE_LAYOUT(table_layout), pad_actor,
            CLUTTER_TABLE_ALIGNMENT_END, // x_align
            CLUTTER_TABLE_ALIGNMENT_START); // y_align
        outlet_row++;
    }
    clutter_actor_set_name(node_box, makeNodeName(node_name.c_str()).c_str());
    return node_box;
}

std::string makeConnectionName(const char *from_node, const char *outlet, const char *to_node, const char *inlet)
{
    std::ostringstream os;
    os << from_node << "." << outlet << "->" << to_node << "." << inlet;
    return os.str();
}

// a single connection
ClutterActor* createConnectionActor(const char *source_node, const char *outlet, const char *sink_node, const char *inlet)
{
    // assumes that the node actors are already created
    std::string outlet_name = makePadName(source_node, outlet, false);
    std::string inlet_name = makePadName(sink_node, inlet, true);
    std::string connection_name = makeConnectionName(source_node, outlet, sink_node, inlet);

    ClutterActor *actor = miller_connection_new_full(&MAGENTA, 2, 0.0f, 0.0f, 100.0f, 100.0f);
    // we call clutter_actor_set_size and clutter_actor_set_position in updateConnectionGeometry
    clutter_actor_set_name(actor, connection_name.c_str());
    return actor;
}

int findStringIndexInVector(const std::vector<std::string> vec, const char *value)
{
    if (vec.size() == 0)
        return -1; // not found
    else
    {
        unsigned int i;
        for (i = 0; i < vec.size() - 1; i++)
        {
            //std::cout << "check for " << value << " match " << vec[i] << std::endl;
            if (vec[i] == std::string(value))
                return i; // found
        }
        // not found
        return -1;
    }
}

void getPadPosition(tempi::Graph &graph, ClutterActor *nodesGroup, const char *node, const char *pad, bool is_inlet, gfloat *result_x, gfloat *result_y)
{
    std::string node_name = makeNodeName(node);
    std::string pad_name = makePadName(node, pad, is_inlet);
    ClutterActor *node_actor = clutter_container_find_child_by_name(
        CLUTTER_CONTAINER(nodesGroup), node_name.c_str());
    ClutterActor *pad_actor = clutter_container_find_child_by_name(
        CLUTTER_CONTAINER(nodesGroup), pad_name.c_str());

    // XXX: what is below doesn't work!
    // clutter_actor_get_transformed_position(pad_actor, result_x, result_y);
    // so we do the following:

    gfloat node_x;
    gfloat node_y;
    clutter_actor_get_position(node_actor, &node_x, &node_y);

    tempi::Node::ptr nodePtr = graph.getNode(node_name.c_str());
    std::vector<std::string> pads;
    if (is_inlet)
        pads = nodePtr->listInlets();
    else
        pads = nodePtr->listOutlets();

    // x position is easy:
    gfloat node_width = clutter_actor_get_width(node_actor);
    if (is_inlet)
        (*result_x) = node_x;
    else
        (*result_x) = node_x + node_width;

    int index = findStringIndexInVector(pads, pad); // _name.c_str());
    if (index == -1)
    {
        std::cout << "could not findStringIndexInVector " << node << " : " << (is_inlet ? "inlet : " : "outlet : ") << pad << "\n";
        std::cout << "vector: ";
        for (unsigned int i = 0; i < pads.size() - 1; i++)
            std::cout << pads[i] << " ";
        std::cout << std::endl;
        (*result_x) = node_x;
        (*result_y) = node_y;
    }
    else
    {
        // FIXME: this is a ugly hack
        static const int ARBITRARY_Y_FACTOR_PER_ROW = 12;
        static const int ARBITRARY_Y_OFFSET = 12;
        (*result_y) = node_y + ARBITRARY_Y_OFFSET + index * ARBITRARY_Y_FACTOR_PER_ROW;
    }
}

void updateConnectionGeometry(tempi::Graph &graph, ClutterActor *connectionsGroup, ClutterActor *nodesGroup, const char *source_node, const char *outlet, const char *sink_node, const char *inlet)
{
    gfloat x1;
    gfloat y1;
    gfloat x2;
    gfloat y2;

    getPadPosition(graph, nodesGroup, source_node, outlet, false, &x1, &y1);
    getPadPosition(graph, nodesGroup, sink_node, inlet, true, &x2, &y2);

    std::string connection_name = makeConnectionName(source_node, outlet, sink_node, inlet);
    ClutterActor *connection_actor = clutter_container_find_child_by_name(
        CLUTTER_CONTAINER(connectionsGroup), connection_name.c_str());

    miller_connection_set_x1(MILLER_CONNECTION(connection_actor), x1);
    miller_connection_set_y1(MILLER_CONNECTION(connection_actor), y1);
    miller_connection_set_x2(MILLER_CONNECTION(connection_actor), x2);
    miller_connection_set_y2(MILLER_CONNECTION(connection_actor), y2);
    {
        std::ostringstream os;
        os << __FUNCTION__ << "(): ";
        os << "Set connection coords " << connection_name << " go from (" << x1 << ", " << y1 << ") to (" << x2 << ", " << y2 << ")";
        tempi::Logger::log(tempi::DEBUG, os);
    }
}

void updateAllConnectionsGeometry(ClutterActor *connectionsGroup, ClutterActor *nodesGroup, tempi::Graph &graph)
{
    std::vector<tempi::Graph::Connection> connections = graph.getAllConnections();
    std::vector<tempi::Graph::Connection>::const_iterator iter;

    for (iter = connections.begin(); iter != connections.end(); ++iter)
    {
        std::string source_node = (*iter).get<0>();
        std::string outlet = (*iter).get<1>();
        std::string sink_node = (*iter).get<2>();
        std::string inlet = (*iter).get<3>();
        updateConnectionGeometry(graph, connectionsGroup, nodesGroup, source_node.c_str(), outlet.c_str(), sink_node.c_str(), inlet.c_str());
    }
}

// all the connections
ClutterActor* createConnectionsActor(tempi::Graph &graph)
{
    ClutterActor *group = clutter_group_new();
    // connections are a tuple of string, string, string, string
    // Tuple containing the name of the source node, outlet name, name of sink node, inlet name.
    std::vector<tempi::Graph::Connection> connections = graph.getAllConnections();
    std::vector<tempi::Graph::Connection>::const_iterator iter;

    for (iter = connections.begin(); iter != connections.end(); ++iter)
    {
        std::string source_node = (*iter).get<0>();
        std::string outlet = (*iter).get<1>();
        std::string sink_node = (*iter).get<2>();
        std::string inlet = (*iter).get<3>();

        ClutterActor * connection_actor = createConnectionActor(
            source_node.c_str(), outlet.c_str(), sink_node.c_str(), inlet.c_str());
        clutter_container_add_actor(CLUTTER_CONTAINER(group), connection_actor);
    }
    return group;
}

} // end of namespace

#endif // ifdef

