/*
 * Copyright (C) 2011 Alexandre Quessy
 * 
 * This file is part of Tempi.
 * 
 * Tempi is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
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

#include "tempi/serializer.h"
#include "tempi/config.h"
#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <stdio.h> // for snprintf
#include <string>
#include <vector>

#ifndef XMLSTR
#define XMLSTR BAD_CAST
#endif

namespace tempi
{

namespace serializer
{

/**
 * Checks if an XML node has a given name.
 * @return success or not.
 */
bool node_name_is(xmlNode *node, const std::string &name);

/** Returns a pointer to the XML child with the given name
 * @return A pointer to the data, not a copy of it.
 */
xmlNode *seek_child_named(xmlNode *parent, const std::string &name);

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

Serializer::Serializer()
{
    // pass
}

bool Serializer::save(Graph &graph, const char *filename)
{
    char buff[256]; // buff for node names and int properties
    xmlDocPtr doc = xmlNewDoc(XMLSTR "1.0");
    // "tempi" node with its "version" attribute
    xmlNodePtr root_node = xmlNewNode(NULL, XMLSTR ROOT_NODE);
    xmlDocSetRootElement(doc, root_node);
    xmlNewProp(root_node, XMLSTR SOFTWARE_VERSION_ATTR, XMLSTR PACKAGE_VERSION);
    // "graphs" node
    xmlNodePtr graphs_node = xmlNewChild(root_node, NULL, XMLSTR GRAPHS_NODE, NULL);
    // "graph" node:
    xmlNodePtr graph_node = xmlNewChild(graphs_node, NULL, XMLSTR GRAPH_NODE, NULL);
    // TODO: add Graph name attribute
    // nodes node: 
    xmlNodePtr nodes_node = xmlNewChild(graph_node, NULL, XMLSTR NODES_NODE, NULL);
    std::vector<std::string> node_names = graph.getNodeNames();
    std::vector<std::string>::const_iterator iter;
    for (iter = node_names.begin(); iter != node_names.end(); ++iter)
    {
        xmlNodePtr node_node = xmlNewChild(nodes_node, NULL, XMLSTR NODE_NODE, NULL);
        xmlNewProp(node_node, XMLSTR NODE_ID_PROPERTY, XMLSTR (*iter).c_str());
        Node::ptr node = graph.getNode((*iter).c_str());
        // node type attribute
        xmlNewProp(node_node, XMLSTR NODE_CLASS_PROPERTY, 
            XMLSTR node->getTypeName().c_str());
        // node attributes nodes
        std::vector<std::string> attribute_names = node->getAttributesNames();
        std::vector<std::string>::const_iterator iter2;
        for (iter2 = attribute_names.begin(); iter2 != attribute_names.end(); ++iter2)
        {
            xmlNodePtr attr_node = xmlNewChild(node_node, NULL, 
                XMLSTR ATTRIBUTE_NODE, NULL);
            xmlNewProp(attr_node, XMLSTR ATTRIBUTE_NAME_PROPERTY, 
                XMLSTR (*iter2).c_str());
            Message attr_value = node->getAttributeValue((*iter2).c_str());
            for (unsigned int i = 0; i < attr_value.getSize(); ++i)
            {
                ArgumentType atom_type_char;
                attr_value.getArgumentType(i, atom_type_char);
                std::string atom_type = boost::lexical_cast<std::string>(
                    (char) atom_type_char);
                std::string atom_value;
                switch (atom_type_char)
                {
                    case BOOLEAN:
                        atom_value = boost::lexical_cast<std::string>(
                            attr_value.getBoolean(i));
                        break;
                    case CHAR:
                        atom_value = boost::lexical_cast<std::string>(
                            attr_value.getChar(i));
                        break;
                    case UNSIGNED_CHAR:
                        atom_value = boost::lexical_cast<std::string>(
                            attr_value.getUnsignedChar(i));
                        break;
                    case DOUBLE:
                        atom_value = boost::lexical_cast<std::string>(
                            attr_value.getDouble(i));
                        break;
                    case FLOAT:
                        atom_value = boost::lexical_cast<std::string>(
                            attr_value.getFloat(i));
                        break;
                    case INT:
                        atom_value = boost::lexical_cast<std::string>(
                            attr_value.getInt(i));
                        break;
                    case LONG:
                        atom_value = boost::lexical_cast<std::string>(
                            attr_value.getLong(i));
                        break;
                    case STRING:
                        atom_value = attr_value.getString(i);
                        break;
                    case POINTER:
                        atom_value = boost::lexical_cast<std::string>(
                            attr_value.getPointer(i));
                        break;
                    default:
                        std::cerr << __FILE__ << " " << __FUNCTION__ <<
                            ": Unsupported type tag: " << atom_type_char <<
                            std::endl;
                        atom_value = "UNSUPPORTED";
                        break;
                } // end of switch/case
                xmlNodePtr atom_node = xmlNewChild(attr_node, NULL, 
                    XMLSTR atom_type.c_str(), XMLSTR atom_value.c_str());
            } // for atoms
        } // for attributes
    } // for nodes
    
    // connections
    std::vector<Graph::Connection> connections = graph.getAllConnections();
    std::vector<Graph::Connection>::const_iterator iter4;
    for (iter4 = connections.begin(); iter4 != connections.end(); ++iter4)
    {
        xmlNodePtr connection_node = xmlNewChild(graph_node, NULL,
            XMLSTR CONNECTION_NODE, NULL);

        Graph::Connection conn = (*iter4);
        std::string from = boost::lexical_cast<std::string>(conn.get<0>());
        std::string outlet = boost::lexical_cast<std::string>(conn.get<1>());
        std::string to = boost::lexical_cast<std::string>(conn.get<2>());
        std::string inlet = boost::lexical_cast<std::string>(conn.get<3>());

        xmlNewProp(connection_node, XMLSTR CONNECTION_FROM_PROPERTY,
            XMLSTR from.c_str());
        xmlNewProp(connection_node, XMLSTR CONNECTION_OUTLET_PROPERTY,
            XMLSTR outlet.c_str());
        xmlNewProp(connection_node, XMLSTR CONNECTION_TO_PROPERTY,
            XMLSTR to.c_str());
        xmlNewProp(connection_node, XMLSTR CONNECTION_INLET_PROPERTY,
            XMLSTR inlet.c_str());
    }

    // Save document to file
    xmlSaveFormatFileEnc(filename, doc, "UTF-8", 1);
    //if (config_->get_verbose())
    std::cout << "Saved the graph to " << filename << std::endl;
    // Free the document + global variables that may have been
    // allocated by the parser.
    xmlFreeDoc(doc);
    xmlCleanupParser();
    return true;
}

bool Serializer::load(Graph &graph, const char *filename)
{
    bool verbose = true;
    // parse the file and get the DOM tree
    xmlDoc *doc = xmlReadFile(filename, NULL, 0);
    if (doc == NULL)
    {
        printf("error: could not parse file %s\n", filename);
        return false;
    }
    else if (verbose)
    {
        std::cout << "Loading project file " << filename << std::endl;
    }
    xmlNode *root = xmlDocGetRootElement(doc);

    // GRAPHS:
    xmlNode *graphs_node = seek_child_named(root, GRAPHS_NODE);
    if (graphs_node != NULL)
    {
        if (verbose)
            std::cout << "graphs\n";
        for (xmlNode *graph_node = graphs_node->children;
            graph_node;
            graph_node = graph_node->next)
        {
            // is a graph:
            if (node_name_is(graph_node, GRAPH_NODE))
            {
                if (verbose)
                    std::cout << "* graph\n";
                // NODES:
                xmlNode *nodes_node = seek_child_named(graph_node, NODES_NODE);
                if (nodes_node != NULL)
                {
                    // for each node:
                    for (xmlNode *node_node = nodes_node->children;
                        node_node;
                        node_node = node_node->next)
                    {
                        // is a node:
                        if (node_name_is(node_node, NODE_NODE))
                        {
                            xmlChar *node_type = xmlGetProp(node_node, 
                                XMLSTR NODE_CLASS_PROPERTY);
                            xmlChar *node_name = xmlGetProp(node_node, 
                                XMLSTR NODE_ID_PROPERTY);
                            if (verbose)
                                std::cout << "  * node " << node_name << " of type " << node_type << std::endl;
                            if (node_type != NULL && node_name != NULL)
                            {
                                graph.addNode((char *) node_type, (char *) node_name);
                                // TODO: load attributes
                                // ATTRIBUTES:
                                for (xmlNode *attribute_node = node_node->children;
                                    attribute_node;
                                    attribute_node = attribute_node->next)
                                {
                                    Message attr_value;
                                    // is an attribute:
                                    if (node_name_is(attribute_node, ATTRIBUTE_NODE))
                                    {
                                        xmlChar *attr_name = xmlGetProp(attribute_node,
                                            XMLSTR ATTRIBUTE_NAME_PROPERTY);
                                        std::cout << "   * attr " << attr_name << std::endl;
                                        // for each atom:
                                        for (xmlNode *atom_node =
                                            attribute_node->children;
                                            atom_node;
                                            atom_node = atom_node->next)
                                        {
                                            // get atom type:
                                            ArgumentType atom_typetag = static_cast<ArgumentType>('\0');
                                            xmlChar *tmp_atom_value = xmlNodeGetContent(
                                                atom_node);
                                            std::string atom_value = std::string((char *) tmp_atom_value);
                                            xmlFree(tmp_atom_value);
                                            if (atom_node->type == XML_ELEMENT_NODE
                                                && atom_node->name)
                                            {
                                                std::string tmp_typetag =
                                                    std::string(
                                                        (char *) atom_node->name);
                                                    if (tmp_typetag.size() == 1)
                                                    {
                                                        atom_typetag = static_cast<ArgumentType>(tmp_typetag[0]);
                                                    } // size
                                                    else
                                                    {
                                                        std::cerr << "Atom typetags should be only one char. Got " << tmp_typetag << std::endl;
                                                    }
            // begin odd indentation block:
            // begin odd indentation block:
            // begin odd indentation block:
            // begin odd indentation block:
            try
            {
                switch (atom_typetag)
                {
                    case BOOLEAN:
                        attr_value.appendBoolean(
                            boost::lexical_cast<bool>(atom_value));
                        break;
                    case CHAR:
                        attr_value.appendChar(
                            boost::lexical_cast<char>(atom_value));
                        break;
                    case UNSIGNED_CHAR:
                        attr_value.appendUnsignedChar(
                            boost::lexical_cast<unsigned char>(atom_value));
                        break;
                    case DOUBLE:
                        attr_value.appendDouble(
                            boost::lexical_cast<double>(atom_value));
                        break;
                    case FLOAT:
                        attr_value.appendFloat(
                            boost::lexical_cast<float>(atom_value));
                        break;
                    case INT:
                        attr_value.appendInt(
                            boost::lexical_cast<int>(atom_value));
                        break;
                    case LONG:
                        attr_value.appendLong(
                            boost::lexical_cast<unsigned long long>(atom_value));
                        break;
                    case STRING:
                        attr_value.appendString(atom_value.c_str());
                        break;
                    case POINTER:
                    default:
                        std::cerr << "ERROR: " << __FILE__ << ":" << __LINE__ << " " << __FUNCTION__ <<
                            ": Unsupported type tag: " << (char) atom_typetag <<
                            std::endl;
                        break;
                } // switch typetag
                if (verbose)
                    std::cout << "    * atom " << (char) atom_typetag << ":" <<  atom_value  << std::endl;
            } catch (const boost::bad_lexical_cast &e)
            {
                std::cerr << __FILE__ << " " << __FUNCTION__ << e.what() << std::endl;
            }
            // end of odd indentation block
            // end of odd indentation block
            // end of odd indentation block
            // end of odd indentation block
                                            } // if ok
                                        } // for each atom
                                        xmlFree(attr_name);

                                    } // is an attribute
                                } // each child of node
                            }
                            xmlFree(node_type);
                            xmlFree(node_name);
                        } // is a node
                    } // for each node
                } // is a set of nodes

                graph.tick(); // FIXME this is annoying

                if (verbose)
                {
                    std::cout << "The Graph is now:\n";
                    std::cout << graph << std::endl;
                }

                // CONNECTIONS:
                xmlNode *connections_node = seek_child_named(root, CONNECTIONS_NODE);
                if (connections_node != NULL)
                {
                    // for each connection:
                    for (xmlNode *connection_node = connections_node->children;
                        connection_node; connection_node = connection_node->next)
                    {
                        // is a connection:
                        if (node_name_is(connection_node, CONNECTION_NODE))
                        {
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
                                if (verbose)
                                    std::cout << "Connect " << from << ":" << outlet << " -> " << to << ":" << inlet << std::endl;
                                graph.connect((char *) from, (char *) outlet, (char *) to, (char *) inlet);
                            }
                            xmlFree(from);
                            xmlFree(outlet);
                            xmlFree(to);
                            xmlFree(inlet);
                        } // is a connection
                    } // for each connection
                } // is a set of connections
            } // is a graph
        } // for each graph
    } // is a set of graphs

    // Free the document + global variables that may have been allocated by the parser.
    xmlFreeDoc(doc);
    xmlCleanupParser();
    return true;
}

bool Serializer::fileExists(const char *filename)
{
    namespace fs = boost::filesystem;
    fs::path path = fs::path(filename);
    return fs::exists(path);
}

// bool Serializer::isADirectory(const char *dirname)
// {
//     return false; // not implemented
// }

bool Serializer::createDirectory(const char *dirname)
{
    namespace fs = boost::filesystem;
    fs::path directory = fs::path(dirname);
    return fs::create_directory(directory);
}

} // end of namespace

} // end of namespace

