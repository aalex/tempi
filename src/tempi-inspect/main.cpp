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
 * @file The tempi-inspect application.
 */

#include "tempi/config.h"
#include "tempi/message.h"
#include "tempi/nodefactory.h"
#include "tempi/internals.h"
#include "tempi/outlet.h"
#include "tempi/inlet.h"
#include <boost/lexical_cast.hpp>
#include <boost/program_options.hpp>
#include <iostream>
#include <sstream>

// namespaces:
namespace po = boost::program_options;
using namespace tempi;

// String constants:
static const char *PROGRAM_NAME = "tempi-inspect";

// types:
typedef enum
{
    FIRST = 1,
    SECOND = 2,
    THIRD = 3
} TitleLevel;

// static functions:
static void print_n_times(unsigned int num, const char *text);
static void print_title(const char *text, TitleLevel level);
static void print_line_if_not_empty(const std::string &text);
static void print_bullet_line_if_not_empty(const std::string &text);

// classes:

/**
 * The TempiInspect class helps navigating in Tempi's doc.
 */
class TempiInspect
{
    public:
        TempiInspect();
        /**
         * Return -1 if it's ok to run the program, or retuns 0 or 1 if we should terminate it.
         * Call this begore launching the app.
         */
        int parse_options(int argc, char **argv);
        bool getVerbose() const;
        bool run();
        bool printClass(const std::string &name);
    private:
        bool verbose_;
        bool all_;
        std::string keyword_;
        tempi::NodeFactory factory_;
        void printAll();
        void printListOfTypes();
};

static void print_title(const std::string &text, TitleLevel level)
{
    using std::cout;
    using std::endl;
    char character = '=';
    switch (level)
    {
        case FIRST:
            character = '=';
            break;
        case SECOND:
            character = '-';
            break;
        case THIRD:
            character = '~';
            break;
        default:
            std::cerr << "Invalid title level" << std::endl;
    }
    cout << text << endl;
    print_n_times(text.size(), std::string((const char) character).c_str());
    cout << character;
    cout << endl;
    cout << endl;
}

static void print_line_if_not_empty(const std::string &text)
{
    if (text == "")
        std::cout << "(none)" << std::endl;
    else
        std::cout << text << std::endl;
}
static void print_bullet_line_if_not_empty(const std::string &text)
{
    if (text == "")
        std::cout << "* (none)" << std::endl;
    else
        std::cout << "* " << text << std::endl;
}

TempiInspect::TempiInspect() :
    verbose_(false)
{
    internals::loadInternals(factory_);
}

bool TempiInspect::getVerbose() const
{
    return verbose_;
}

static void print_n_times(unsigned int num, const char *text)
{
    for (int i = 0; i < num; ++i)
        std::cout << text;
}

bool TempiInspect::printClass(const std::string &name)
{
    using std::cout;
    using std::endl;
    using std::vector;
    using std::map;
    using std::string;

    if (factory_.hasType(name.c_str()))
    {
        Node::ptr node = factory_.create(name.c_str());
        node->init();
        //node->tick();

        // NODE TYPE NAME
        // ==============
        //cout << endl << endl;
        cout << "Node Type [" << name << "]" << endl;
        cout << "===========";
        print_n_times(node->getTypeName().size(), "=");
        cout << "=";
        cout << endl; // after =========
        cout << endl; // after =========
        if (node->getShortDocumentation() != "")
            cout << node->getShortDocumentation() << endl;
        else
            cout << "(Not documented)" << endl;
        //cout << endl;

        // --------------------------------- Attributes ------------
        cout << "Attributes" << endl;
        cout << "----------" << endl;
        cout << endl;
        {
            if (node->listAttributes().size() == 0)
                cout << "(No attributes)" << endl;
            else
            {
                vector<string> attributes = node->listAttributes();
                vector<string>::const_iterator iter;
                for (iter = attributes.begin(); iter != attributes.end(); ++iter)
                {
                    Attribute* attr = node->getAttribute((*iter).c_str());
                    cout << attr->getName() << endl;
                    print_n_times(attr->getName().size(), "~");
                    cout << endl; // after the ~~~~~
                    if (attr->getShortDocumentation() != "")
                        cout << attr->getShortDocumentation() << endl;
                    else
                        cout << "(Not documented)" << endl;
                    if (attr->isTypeStrict())
                        cout << "Type: " << attr->getValue().getTypes() << endl;
                    else
                        cout << "Type: This attribute has a *variable type*." << endl;
                    cout << "Default value: " << attr->getValue() << endl;
                    //cout << endl;
                }
            }
        }
        //cout << endl;
        // --------------------------------- Inlets ------------
        cout << "Inlets" << endl;
        cout << "------" << endl;
        cout << endl;
        //cout << "All inlets::" << endl << endl;
        if (node->getInlets().size() == 0)
            cout << "(No inlet)" << endl;
        else
        {
            map<string, Inlet::ptr> inlets = node->getInlets();
            map<string, Inlet::ptr>::const_iterator iter;
            for (iter = inlets.begin(); iter != inlets.end(); ++iter)
            {
                cout << "* Inlet \"" << (*iter).first << "\":";
                if (((*iter).second).get()->getDocumentation() != "")
                    cout << ((*iter).second).get()->getDocumentation() << endl;
                else
                    cout << "(Not documented)" << endl;
            }
        }
        //cout << endl;
        // --------------------------------- Outlets ------------
        cout << "Outlets" << endl;
        cout << "------" << endl;
        cout << endl;
        //cout << "All outlets::" << endl << endl;
        if (node->getOutlets().size() == 0)
            cout << "(No outlet)" << endl;
        else
        {
            map<string, Outlet::ptr> outlets = node->getOutlets();
            map<string, Outlet::ptr>::const_iterator iter;
            for (iter = outlets.begin(); iter != outlets.end(); ++iter)
            {
                cout << "* Outlet \"" << (*iter).first << "\":";
                if (((*iter).second).get()->getDocumentation() != "")
                    cout << ((*iter).second).get()->getDocumentation() << endl;
                else
                    cout << "(Not documented)" << endl;
            }
        }
        //cout << endl;
        // -----------------------------------------------------
        //cout << endl;
    }
    else
        std::cerr << "No such node type: \"" << name << "\"" << std::endl;
}

void TempiInspect::printListOfTypes()
{
    print_title(std::string("Tempi types:", FIRST));
    std::map<std::string, AbstractNodeType::ptr>::const_iterator iter;
    std::map<std::string, AbstractNodeType::ptr> entries = factory_.getEntries();
    for (iter = entries.begin(); iter != entries.end(); ++iter)
    {
        print_bullet_line_if_not_empty((*iter).first);
    }
}

void TempiInspect::printAll()
{
    print_title(std::string("Tempi types:", FIRST));
    std::map<std::string, AbstractNodeType::ptr>::const_iterator iter;
    std::map<std::string, AbstractNodeType::ptr> entries = factory_.getEntries();
    for (iter = entries.begin(); iter != entries.end(); ++iter)
    {
        printClass((*iter).first);
    }
}

bool TempiInspect::run()
{
    if (all_)
        printAll();
    else
    {
        if (keyword_ == "")
        {
            printListOfTypes();
        }
        else
        {
            printClass(keyword_);
        }
    }
    return true;
}

int TempiInspect::parse_options(int argc, char **argv)
{
    po::options_description desc("Options");
    desc.add_options()
        ("help,h", "Show this help message and exit")
        ("version", "Show program's version number and exit")
        ("keyword,k", po::value<std::string>()->default_value(""), "Sets the keyword to look for in Tempi's documentation")
        ("verbose,v", po::bool_switch(), "Enables a verbose output")
        ("all,a", po::bool_switch(), "Prints detailed info for all node types")
        ;
    po::variables_map options;
    try
    {
        // all positional options should be translated into "file" options
        po::positional_options_description p;
        p.add("keyword", -1);
        po::store(po::command_line_parser(argc, argv).
            options(desc).positional(p).run(), options);
        po::notify(options);
    }
    catch (const po::error &e)
    {
        std::cerr << e.what() << std::endl;
        std::cout << desc << std::endl;
        return 1;
    }

    verbose_ = options["verbose"].as<bool>();
    all_ = options["all"].as<bool>();
    keyword_ = options["keyword"].as<std::string>();
    // Options that makes the program exit:
    if (options.count("help"))
    {
        std::cout << desc << std::endl;
        return 0;
    }
    if (options.count("version"))
    {
        std::cout << PROGRAM_NAME << " " << PACKAGE_VERSION << std::endl;
        return 0;
    }
    if (verbose_)
        std::cout << "Looking for keyword \"" << keyword_ << "\"..." << std::endl;
    return -1;
}

int main(int argc, char *argv[])
{
    int ret;
    TempiInspect app;
    try
    {
        ret = app.parse_options(argc, argv);
    }
    catch (const boost::bad_any_cast &e)
    {
        std::cerr << "Error parsing options ";
        std::cerr << e.what() << std::endl;
        return 1;
    }
    if (ret != -1)
        return ret;

    bool ok = app.run();
    if (! ok)
    {
        if (app.getVerbose())
            std::cerr << "Error calling app.run()\n";
        return 1;
    }

    return 0;
}

