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
#include <boost/filesystem.hpp>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <stdio.h> // for snprintf
#include <string>

namespace // anonymous
{
    bool node_name_is(xmlNode *node, const std::string &name)
    {
        return (node->type == XML_ELEMENT_NODE && node->name && (xmlStrcmp(node->name, XMLSTR name.c_str()) == 0));
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
} // end of anonymous namespace

namespace tempi
{

Serializer::Serializer()
{
    // pass
}

bool Serializer::save(Graph &graph, const char *filename)
{
    using namespace serializer;
    char buff[256]; // buff for node names and int properties
    xmlDocPtr doc = xmlNewDoc(XMLSTR "1.0");
    // "project" node with its "name" attribute
    xmlNodePtr root_node = xmlNewNode(NULL, XMLSTR ROOT_NODE);
    xmlDocSetRootElement(doc, root_node);
    xmlNewProp(root_node, XMLSTR SOFTWARE_VERSION_ATTR, XMLSTR PACKAGE_VERSION);
    //sprintf(buff, "%d", get_current_clip_number());
    //xmlNewProp(root_node, XMLSTR ss::CURRENT_CLIP_ATTR, XMLSTR buff);
    // "clips" node
    xmlNodePtr clips_node = xmlNewChild(root_node, NULL, XMLSTR ss::CLIPS_NODE, NULL); // No text contents
    return false; // not implemented
}

bool Serializer::load(Graph &graph, const char *filename)
{
    return false; // not implemented
}

bool Serializer::fileExists(const char *filename)
{
    namespace fs = boost::filesystem;
    fs::path path = fs::path(filename);
    return fs::exists(path);
}

bool Serializer::isADirectory(const char *dirname)
{
    return false; // not implemented
}

bool Serializer::createDirectory(const char *dirname)
{
    namespace fs = boost::filesystem;
    fs::path directory = fs::path(dirname);
    return fs::create_directory(directory);
}

} // end of namespace

