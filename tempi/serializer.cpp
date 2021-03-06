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

#include "tempi/serializer.h"
#include "tempi/config.h"
#include "tempi/timeposition.h"
#include "tempi/utils.h"
#include "tempi/log.h"
#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <stdio.h> // for snprintf
#include <string>
#include <sstream>
#include <vector>

#ifndef XMLSTR
#define XMLSTR BAD_CAST
#endif

namespace tempi {
namespace serializer {

/**
 * Checks if an XML node has a given name.
 * @return success or not.
 */
static bool node_name_is(xmlNode *node, const std::string &name);
static bool save_message(xmlNodePtr message_node, const Message &value);
static bool save_connections(xmlNodePtr graph_node, Graph &graph);
static bool save_region(xmlNodePtr root_node, sampler::Region &region);
static bool load_graph_connections(xmlNodePtr graph_node, Graph &graph);
/**
 * Note: internally calls Graph.tick()
 */
static bool load_graph(xmlNodePtr graph_node, Graph &graph);
static TimePosition stringToTimePosition(const char *event_time);
static bool load_message(xmlNodePtr message_node, Message &value);

/**
 * Converts a char to a string.
 */
static std::string char_to_string(char c);

/** Returns a pointer to the XML child with the given name
 * @return A pointer to the data, not a copy of it.
 */
static xmlNode *seek_child_named(xmlNode *parent, const std::string &name);

// implementations:

std::string char_to_string(char c)
{
    std::stringstream ss;
    ss << c;
    std::string s;
    ss >> s;
    return s;
}

static TimePosition stringToTimePosition(const char *event_time)
{
    try
    {
        return boost::lexical_cast<TimePosition>(event_time);
    }
    catch (const boost::bad_lexical_cast &e)
    {
        Logger::log(ERROR, e.what());
    }
}

bool node_name_is(xmlNode *node, const std::string &name)
{
    return (node->type == XML_ELEMENT_NODE && node->name &&
        (xmlStrcmp(node->name, XMLSTR name.c_str()) == 0));
}

/** Returns a pointer to the XML child with the given name
 * @return A pointer to the data, not a copy of it.
 */
xmlNode *seek_child_named(xmlNode *parent, const std::string &name)
{
    if (parent == NULL)
        return NULL;
    for (xmlNode *node = parent->children; node != NULL; node = node->next)
    {
        if (node_name_is(node, name))
        {
            return node;
        }
    }
    return NULL;
}

bool save_connections(xmlNodePtr graph_node, Graph &graph)
{
    std::vector<Graph::Connection> connections = graph.getAllConnections();
    std::vector<Graph::Connection>::const_iterator iter4;
    for (iter4 = connections.begin(); iter4 != connections.end(); ++iter4)
    {
        xmlNodePtr connection_node = xmlNewChild(graph_node, NULL,
            XMLSTR CONNECTION_NODE, NULL);

        Graph::Connection conn = (*iter4);
        std::string from = boost::lexical_cast<std::string>(
            conn.get<0>());
        std::string outlet = boost::lexical_cast<std::string>(
            conn.get<1>());
        std::string to = boost::lexical_cast<std::string>(
            conn.get<2>());
        std::string inlet = boost::lexical_cast<std::string>(
            conn.get<3>());

        xmlNewProp(connection_node,
            XMLSTR CONNECTION_FROM_PROPERTY,
            XMLSTR from.c_str());
        xmlNewProp(connection_node,
            XMLSTR CONNECTION_OUTLET_PROPERTY,
            XMLSTR outlet.c_str());
        xmlNewProp(connection_node,
            XMLSTR CONNECTION_TO_PROPERTY,
            XMLSTR to.c_str());
        xmlNewProp(connection_node,
            XMLSTR CONNECTION_INLET_PROPERTY,
            XMLSTR inlet.c_str());
    }
    return true;
}

bool save_nodes(xmlNodePtr graph_node, Graph &graph)
{
    std::vector<std::string> node_names = graph.getNodeNames();
    std::vector<std::string>::const_iterator iter;
    for (iter = node_names.begin(); iter != node_names.end(); ++iter)
    {
        xmlNodePtr node_node = xmlNewChild(graph_node, NULL, XMLSTR NODE_NODE, NULL);
        // node type attribute
        Node::ptr node = graph.getNode((*iter).c_str());
        xmlNewProp(node_node, XMLSTR NODE_CLASS_PROPERTY,
            XMLSTR node->getTypeName().c_str());
        // and its id
        xmlNewProp(node_node, XMLSTR NODE_ID_PROPERTY, XMLSTR (*iter).c_str());
        // node attributes nodes
        std::vector<std::string> attribute_names = node->listAttributes();
        std::vector<std::string>::const_iterator iter2;
        for (iter2 = attribute_names.begin(); iter2 != attribute_names.end(); ++iter2)
        {
            bool save_it = true;
            Message attr_value = node->getAttributeValue((*iter2).c_str());
            static const Message default_pos_attr = Message("fff", 0.0, 0.0, 0.0);
            static const Message default_data_attr = Message("s", "");
            // Do not save if __data__ == "" || __position__ == (0.0, 0.0, 0.0)
            if ((*iter2) == Node::ATTRIBUTE_DATA)
            {
                if (attr_value.getString(0) == default_data_attr.getString(0))
                {
                    save_it = false;
                }
            }
            else if ((*iter2) == Node::ATTRIBUTE_POSITION)
            {
                if (attr_value == default_pos_attr)
                //attr_value.getFloat(0) == default_pos_attr.getFloat(0)
                //&& attr_value.getFloat(1) == default_pos_attr.getFloat(1)
                //&& attr_value.getFloat(2) == default_pos_attr.getFloat(2)
                {
                    save_it = false;
                }
            }
            if (save_it)
            {
                xmlNodePtr attr_node = xmlNewChild(node_node, NULL,
                    XMLSTR ATTRIBUTE_NODE, NULL);
                xmlNewProp(attr_node, XMLSTR ATTRIBUTE_NAME_PROPERTY,
                    XMLSTR (*iter2).c_str());
                save_message(attr_node, attr_value);
            }
        } // for attributes
    } // for nodes
    return true;
}

bool save(Graph &graph, const char *filename)
{
    xmlDocPtr doc = xmlNewDoc(XMLSTR "1.0");
    // "tempi" node with its "version" attribute
    xmlNodePtr root_node = xmlNewNode(NULL, XMLSTR ROOT_NODE);
    xmlDocSetRootElement(doc, root_node);
    xmlNewProp(root_node, XMLSTR SOFTWARE_VERSION_ATTR, XMLSTR PACKAGE_VERSION);
    // "graphs" node
    //xmlNodePtr graphs_node = xmlNewChild(root_node, NULL, XMLSTR GRAPHS_NODE, NULL);
    // "graph" node:
    xmlNodePtr graph_node = xmlNewChild(root_node, NULL, XMLSTR GRAPH_NODE, NULL);
    // TODO: add Graph name attribute
    // nodes node:
    //xmlNodePtr nodes_node = xmlNewChild(graph_node, NULL, XMLSTR NODES_NODE, NULL);
    save_nodes(graph_node, graph);

    // connections
    save_connections(graph_node, graph);

    // Save document to file
    xmlSaveFormatFileEnc(filename, doc, "UTF-8", 1);
    if (Logger::isEnabledFor(INFO))
    {
        std::ostringstream os;
        os << __FUNCTION__ << ": Saved graph " << graph.getName() << " to " << filename;
        Logger::log(INFO, os);
    }
    // Free the document + global variables that may have been
    // allocated by the parser.
    xmlFreeDoc(doc);
    return true;
}

bool load_graph_connections(xmlNodePtr graph_node, Graph &graph)
{
    bool overall_success = true;
    for (xmlNode *connection_node = graph_node->children;
        connection_node; connection_node = connection_node->next)
    { // for each connection
        // is a connection:
        if (node_name_is(connection_node, CONNECTION_NODE))
        {
            Logger::log(DEBUG, "Entering connection:");
            xmlChar *from = xmlGetProp(connection_node,
                XMLSTR CONNECTION_FROM_PROPERTY);
            xmlChar *outlet = xmlGetProp(connection_node,
                XMLSTR CONNECTION_OUTLET_PROPERTY);
            xmlChar *to = xmlGetProp(connection_node,
                XMLSTR CONNECTION_TO_PROPERTY);
            xmlChar *inlet = xmlGetProp(connection_node,
                XMLSTR CONNECTION_INLET_PROPERTY);
            if (from != NULL && outlet != NULL
                && to != NULL && inlet != NULL)
            {
                if (Logger::isEnabledFor(DEBUG))
                {
                    std::ostringstream os;
                    os << "serializer::" << __FUNCTION__ << "(): Connect "
                        << from << ":" << outlet << " -> " << to << ":" << inlet;
                    Logger::log(DEBUG, os);
                }
                bool success = graph.connect((char *) from, (char *) outlet, (char *) to, (char *) inlet);
                if (success)
                {
                }
                else
                {
                    std::ostringstream os;
                    os << "serializer::" << __FUNCTION__ << "(): Could not connect "
                        << from << ":" << outlet << " -> " << to << ":" << inlet;
                    Logger::log(ERROR, os);
                    overall_success = false;
                }
            }
            xmlFree(from);
            xmlFree(outlet);
            xmlFree(to);
            xmlFree(inlet);
        } // is a connection
        else if (
            ! node_name_is(connection_node, NODE_NODE) &&
            ! node_name_is(connection_node, CONNECTION_NODE) &&
            ! node_name_is(connection_node, REGION_NODE) &&
            connection_node->type == XML_ELEMENT_NODE)
        {
            std::ostringstream os;
            os << "Found unknown XML tag: \"" << connection_node->name << "\"";
            Logger::log(ERROR, os);
        }
    } // for each connection
    return overall_success;
}

bool load_message(xmlNodePtr message_node, Message &message)
{
    bool success = true;
    for (xmlNode *atom_node =
        message_node->children;
        atom_node;
        atom_node = atom_node->next)
    { // for each atom:
        // get atom type:
        AtomType atom_typetag = static_cast<AtomType>('\0');
        xmlChar *tmp_atom_value = xmlNodeGetContent(
            atom_node);
        std::string atom_value = std::string((char *) tmp_atom_value);
        xmlFree(tmp_atom_value);
        if (atom_node->type == XML_ELEMENT_NODE
            && atom_node->name)
        { // is atom node
            std::string tmp_typetag = std::string(
                    (char *) atom_node->name);
            if (tmp_typetag.size() == 1)
            {
                atom_typetag = static_cast<AtomType>(tmp_typetag[0]);
            } // size
            else
            {
                std::cerr << "Atom typetags should be only one char. Got " <<
                tmp_typetag << std::endl;
                success = false;
            }
            try
            {
                utils::appendArgumentFromString(message, atom_value.c_str(), atom_typetag);
                if (Logger::isEnabledFor(DEBUG))
                {
                    std::ostringstream os;
                    os << "    * atom " <<
                        (char) atom_typetag << ":" <<
                        atom_value;
                    Logger::log(DEBUG, os);
                }
            }
            catch (const BadAtomTypeException &e)
            {
                std::ostringstream os;
                os << __FILE__ << ": " << __FUNCTION__ << " " << e.what();
                Logger::log(ERROR, os);
                success = false;
            }
            if (Logger::isEnabledFor(DEBUG))
            {
                std::ostringstream os;
                os << "    * atom results in " << message;
                Logger::log(DEBUG, os);
            }
        } // is atom node
    } // for each atom
    return success;
}

bool load_node_attributes(xmlNodePtr node_node, Node &node)
{
    // ATTRIBUTES:
    for (xmlNode *attribute_node = node_node->children;
        attribute_node;
        attribute_node = attribute_node->next)
    { // each child of node
        if (node_name_is(attribute_node, ATTRIBUTE_NODE))
        { // is an attribute:
            xmlChar *attr_name = xmlGetProp(attribute_node,
                XMLSTR ATTRIBUTE_NAME_PROPERTY);
            //std::cout << "   * attr " << attr_name << std::endl;
            if (Logger::isEnabledFor(DEBUG))
            {
                std::ostringstream os;
                os << "   * attr \"" << attr_name << "\":";
                Logger::log(DEBUG, os);
            }
            Message attr_value;
            load_message(attribute_node, attr_value);
            try
            {
                node.setAttributeValue((char *) attr_name, attr_value);
            }
            catch (const BadIndexException &e)
            {
                std::ostringstream os;
                os << __FUNCTION__ << ": " << e.what() << std::endl;
                Logger::log(ERROR, os);
            }
            catch (const BadAtomTypeException &e)
            {
                std::ostringstream os;
                os << __FUNCTION__ << ": " << e.what() << std::endl;
                Logger::log(ERROR, os);
            }
            xmlFree(attr_name);
        } // is an attribute
    } // each child of node
    return true;
}

bool load_graph(xmlNodePtr graph_node, Graph &graph)
{
    bool loaded_graph_without_error = true;
    for (xmlNode *node_node = graph_node->children;
        node_node;
        node_node = node_node->next)
    { // for each node:
        if (node_name_is(node_node, NODE_NODE))
        { // is a node
            // TODO: check if it has the property
            xmlChar *node_type = xmlGetProp(node_node,
                XMLSTR NODE_CLASS_PROPERTY);
            // TODO: check if it has the property
            xmlChar *node_name = xmlGetProp(node_node,
                XMLSTR NODE_ID_PROPERTY);
            if (Logger::isEnabledFor(DEBUG))
            {
                std::ostringstream os;
                os << "  * node " << node_name << " of type " << node_type;
                Logger::log(DEBUG, os);
            }
            if (node_type != NULL && node_name != NULL)
            { // node has name
                bool success = graph.addNode((char *) node_type, (char *) node_name);
                if (success)
                {
                    Node::ptr node = graph.getNode((char *) node_name);
                    load_node_attributes(node_node, *(node.get()));
                }
                else
                {
                    std::ostringstream os;
                    os << "Could not create node " << node_name << " of type " << node_type;
                    Logger::log(ERROR, os);
                    loaded_graph_without_error = false;
                }
            } // node has name
            xmlFree(node_type);
            xmlFree(node_name);
        } // is a node
    } // for each node

    if (Logger::isEnabledFor(DEBUG))
    {
        std::ostringstream os;
        os << "(we will now tick the graph)";
        Logger::log(DEBUG, os);
    }
    graph.tick(); // FIXME this is annoying

    if (! load_graph_connections(graph_node, graph))
        loaded_graph_without_error = false;
    return loaded_graph_without_error;
}

bool load_region(xmlNodePtr region_node, sampler::Region &region)
{
    for (xmlNode *event_node = region_node->children;
        event_node;
        event_node = event_node->next)
    { // for each node:
        if (node_name_is(event_node, EVENT_NODE))
        { // is an event
            // TODO: check if it has the property
            xmlChar *event_time = xmlGetProp(event_node,
                XMLSTR EVENT_TIMEPOSITION_PROPERTY);
            if (Logger::isEnabledFor(DEBUG))
            {
                std::ostringstream os;
                os << "  * event " << event_time;
                Logger::log(DEBUG, os);
            }
            if (event_time != NULL)
            { // event has time
                TimePosition timePosition = stringToTimePosition((char *) event_time);
                Message value;
                load_message(event_node, value);
                region.add(timePosition, value);
            } // event has time
            xmlFree(event_time);
        } // is an event node
    } // for each event
    return true;
}

bool load(Graph &graph, const char *filename)
{
    // parse the file and get the DOM tree
    xmlDoc *doc = xmlReadFile(filename, NULL, 0);
    if (doc == NULL)
    {
        std::ostringstream os;
        os << "Serializer could not parse file " <<  filename;
        Logger::log(ERROR, os);
        return false;
    }
    if (Logger::isEnabledFor(DEBUG))
    {
        std::ostringstream os;
        os << "Loading a graph from file " << filename;
        Logger::log(DEBUG, os);
    }
    xmlNode *root = xmlDocGetRootElement(doc);

    // FIXME: this method might load many graphs into one!
    Logger::log(DEBUG, "Graphs");
    for (xmlNode *graph_node = root->children;
        graph_node;
        graph_node = graph_node->next)
    {
        if (node_name_is(graph_node, GRAPH_NODE))
        { // is a graph:
            Logger::log(DEBUG, " * Graph");
            // Loads nodes and their attributes
            // ticks the Graph
            // and loads the connections between nodes:
            load_graph(graph_node, graph);
        } // is a graph
    } // for each graph

    if (Logger::isEnabledFor(DEBUG))
    {
        std::ostringstream os;
        os << "(we will now tick the graph)";
        Logger::log(DEBUG, os);
    }
    graph.tick();
    if (Logger::isEnabledFor(DEBUG))
    {
        std::ostringstream os;
        os << "(we will now loadbang the graph)";
        Logger::log(DEBUG, os);
    }
    graph.loadBang();

    if (Logger::isEnabledFor(INFO))
    {
        std::ostringstream os;
        os << "Loaded graph " << graph.getName() << " from " << filename;
        Logger::log(INFO, os);
    }
    // Free the document + global variables that may have been allocated by the parser.
    xmlFreeDoc(doc);
    return true;
}

bool fileExists(const char *filename)
{
    namespace fs = boost::filesystem;
    fs::path path = fs::path(filename);
    return fs::exists(path);
}

// bool isADirectory(const char *dirname)
// {
//     return false; // not implemented
// }

bool createDirectory(const char *dirname)
{
    namespace fs = boost::filesystem;
    fs::path directory = fs::path(dirname);
    return fs::create_directory(directory);
}

bool save_message(xmlNodePtr message_node, const Message &value)
{
    for (unsigned int i = 0; i < value.getSize(); ++i)
    {
        std::string atom_value;
        AtomType atom_type_char;
        value.getAtomType(i, atom_type_char);
        std::string atom_type = char_to_string(atom_type_char);
        try
        {
            atom_value = utils::argumentToString(value, i);
            xmlNodePtr atom_node = xmlNewChild(message_node, NULL,
                XMLSTR atom_type.c_str(), XMLSTR atom_value.c_str());
        }
        catch (const BadAtomTypeException &e)
        {
            std::ostringstream os;
            os << e.what() << std::endl;
            Logger::log(ERROR, os);
        }
    } // for atoms
}

bool save_region(xmlNodePtr root_node, sampler::Region &region)
{
    xmlNodePtr region_node = xmlNewChild(root_node, NULL, XMLSTR REGION_NODE, NULL);
    xmlNewProp(region_node, XMLSTR REGION_NAME_PROPERTY, XMLSTR region.getName().c_str());
    const std::vector<sampler::Region::Event>& events = region.getAllEvents();
    std::vector<sampler::Region::Event>::const_iterator iter;
    for (iter = events.begin(); iter != events.end(); ++iter)
    {
        xmlNodePtr event_node = xmlNewChild(region_node, NULL, XMLSTR EVENT_NODE, NULL);
        sampler::Region::Event event = (*iter);
        std::string pos = boost::lexical_cast<std::string>(event.get<0>());
        xmlNewProp(event_node, XMLSTR EVENT_TIMEPOSITION_PROPERTY,
            XMLSTR pos.c_str());
        Message value = (*iter).get<1>();
        save_message(event_node, value);
    } // for events
    return true;
}

bool save(sampler::Region &region, const char *filename)
{
    xmlDocPtr doc = xmlNewDoc(XMLSTR "1.0");
    // "tempi" node with its "version" attribute
    xmlNodePtr root_node = xmlNewNode(NULL, XMLSTR ROOT_NODE);
    xmlDocSetRootElement(doc, root_node);
    xmlNewProp(root_node, XMLSTR SOFTWARE_VERSION_ATTR, XMLSTR PACKAGE_VERSION);
    save_region(root_node, region);

    // Save document to file
    xmlSaveFormatFileEnc(filename, doc, "UTF-8", 1);
    {
        std::ostringstream os;
        os << "Saved the region to " << filename;
        Logger::log(INFO, os);
    }
    // Free the document + global variables that may have been
    // allocated by the parser.
    xmlFreeDoc(doc);
    return true;
}

bool load(sampler::Region &region, const char *filename)
{
    // parse the file and get the DOM tree
    xmlDoc *doc = xmlReadFile(filename, NULL, 0);
    if (doc == NULL)
    {
        std::ostringstream os;
        os << "Serializer could not parse sampler region file " <<  filename;
        Logger::log(ERROR, os);
        return false;
    }
    if (Logger::isEnabledFor(INFO))
    {
        std::ostringstream os;
        os << "Loading sampler region file " << filename;
        Logger::log(INFO, os);
    }
    xmlNode *root = xmlDocGetRootElement(doc);

    // FIXME: this method might load many regions into one!
    Logger::log(DEBUG, "regions");
    for (xmlNode *region_node = root->children;
        region_node;
        region_node = region_node->next)
    {
        if (node_name_is(region_node, REGION_NODE))
        { // is a Region:
            Logger::log(DEBUG, " * Region");
            load_region(region_node, region);
        } // is a Region
    } // for each Region

    if (Logger::isEnabledFor(INFO))
    {
        std::ostringstream os;
        os << "Loaded a sampler region from " << filename;
        Logger::log(INFO, os);
    }
    // Free the document + global variables that may have been allocated by the parser.
    xmlFreeDoc(doc);
    return true;
}

bool cleanup()
{
    xmlCleanupParser();
}

} // end of namespace
} // end of namespace

