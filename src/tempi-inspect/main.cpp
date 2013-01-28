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
#include "tempi/log.h"
#include "tempi/utils.h"
#include <glib.h>
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
static void print_rst_title(const std::string &text, TitleLevel level);
static void print_line_if_not_empty(const std::string &text);
static void print_bullet_line_if_not_empty(const std::string &text);
static bool stringIsInVector(const std::string &text, const std::vector<std::string> &vec);

static std::vector<std::string> getUndesiredInlets();
static std::vector<std::string> getUndesiredOutlets();
static std::vector<std::string> getUndesiredAttributes();

static gboolean help = FALSE;
static gboolean version = FALSE;
static gboolean verbose = FALSE;
static gboolean debug = FALSE;
static gboolean all = FALSE;
static gchar** keywords = NULL;

static GOptionEntry entries[] =
{
    {"version", 0, 0, G_OPTION_ARG_NONE, &version, "Show program's version number and exit"},
    {"verbose", 'v', 0, G_OPTION_ARG_NONE, &verbose, "Enables a verbose output"},
    {"debug", 'd', 0, G_OPTION_ARG_NONE, &debug, "Enables a very verbose output"},
    {"all", 'a', 0, G_OPTION_ARG_NONE, &all, "Prints detailed info for all node types"},
    {G_OPTION_REMAINING, 0, 0, G_OPTION_ARG_FILENAME_ARRAY, &keywords, "Sets the node type names to look for in Tempi's documentation. There can be many."},
    {NULL}
};

// classes:
//

class Printer
{
    public:
        virtual void printAllClassInfo(Node::ptr &node) = 0;
};

class RstPrinter: public Printer
{
    public:
        virtual void printAllClassInfo(Node::ptr &node);
};

// class CsvPrinter: public Printer
// {
//     public:
//         virtual void printAllClassInfo(Node::ptr &node);
// };
//

std::vector<std::string> getUndesiredInlets()
{
    std::vector<std::string> ret;
    ret.push_back(std::string("__attr__"));
    ret.push_back(std::string("__call__"));
    return ret;
}

std::vector<std::string> getUndesiredOutlets()
{
    std::vector<std::string> ret;
    ret.push_back(std::string("__attr__"));
    ret.push_back(std::string("__return__"));
    return ret;
}

std::vector<std::string> getUndesiredAttributes()
{
    std::vector<std::string> ret;
    ret.push_back(std::string("__position__"));
    ret.push_back(std::string("__data__"));
    return ret;
}

void RstPrinter::printAllClassInfo(Node::ptr &node)
{
    using std::cout;
    using std::endl;
    using std::vector;
    using std::map;
    using std::string;

    std::vector<std::string> undesired_inlets = getUndesiredInlets();
    std::vector<std::string> undesired_outlets = getUndesiredOutlets();
    std::vector<std::string> undesired_attributes = getUndesiredAttributes();

    cout << "[" << node->getTypeName() << "]" << endl;
    cout << "=";
    print_n_times(node->getTypeName().size(), "=");
    cout << "=";
    cout << endl;
    cout << endl;
    if (node->getShortDocumentation() != "")
        cout << node->getShortDocumentation(); // << endl;
    else
        cout << "(node type not documented)"; // << endl;
    if (node->getLongDocumentation() != "")
    {
        cout << endl;
        cout << endl;
        cout << node->getLongDocumentation(); // << endl;
        cout << endl;
    }
    cout << endl;
    cout << endl;
    // ATTRIBUTES:
    {
        if (node->listAttributes().size() == 0)
            cout << "* (No attributes)" << endl; // << endl;
        else
        {
            vector<string> attributes = node->listAttributes();
            vector<string>::const_iterator iter;
            for (iter = attributes.begin(); iter != attributes.end(); ++iter)
            {
                if (! stringIsInVector((*iter), undesired_attributes))
                {
                    Attribute* attr = node->getAttribute((*iter).c_str());
                    cout << "* Attribute \"" << attr->getName() << "\" : ";
                    if (attr->isTypeStrict())
                        cout << "(Type: " << attr->getValue().getTypes() << ")";
                    else
                        cout << "(variable type)";
                    cout << " ";
                    if (attr->getShortDocumentation() != "")
                        cout << attr->getShortDocumentation();
                    else
                        cout << "(attribute not documented)";
                    cout << " ";
                    cout << "Default value: " << attr->getValue();
                    cout << endl;
                }
            }
        }
    }
    // METHODS:
    {
        if (node->listMethods().size() == 0)
        {
            // cout << "* (No methods)" << endl;
        }
        else
        {
            vector<string> methods = node->listMethods();
            vector<string>::const_iterator iter;
            for (iter = methods.begin(); iter != methods.end(); ++iter)
            {
                EntityMethod* method = node->getMethod((*iter).c_str());
                cout << "* Method \"" << method->getName() << "\" : ";
                cout << "(Arguments types: " << method->getArgumentsType() << ")";
                cout << "(Return types: " << method->getReturnType() << ")";
                cout << " ";
                if (method->getShortDocumentation() != "")
                    cout << method->getShortDocumentation();
                else
                    cout << "(method not documented)";
                cout << " ";
                cout << endl;
            }
        }
    }
    // INLETS:
    if (node->getInlets().size() == 0)
        cout << "* (No inlet)" << endl;
    else
    {
        map<string, Inlet::ptr> inlets = node->getInlets();
        map<string, Inlet::ptr>::const_iterator iter;
        for (iter = inlets.begin(); iter != inlets.end(); ++iter)
        {
            if (! stringIsInVector((*iter).first, undesired_inlets))
            {
                cout << "* Inlet \"" << (*iter).first << "\" : ";
                if (((*iter).second).get()->getShortDocumentation() != "")
                    cout << ((*iter).second).get()->getShortDocumentation() << endl;
                else
                    cout << "(Not documented)" << endl;
            }
        }
    }
    // OUTLETS:
    if (node->getOutlets().size() == 0)
        cout << "* (No outlet)" << endl;
    else
    {
        map<string, Outlet::ptr> outlets = node->getOutlets();
        map<string, Outlet::ptr>::const_iterator iter;
        for (iter = outlets.begin(); iter != outlets.end(); ++iter)
        {
            if (! stringIsInVector((*iter).first, undesired_outlets))
            {
                cout << "* Outlet \"" << (*iter).first << "\" : ";
                if (((*iter).second).get()->getShortDocumentation() != "")
                    cout << ((*iter).second).get()->getShortDocumentation() << endl;
                else
                    cout << "(Not documented)" << endl;
            }
        }
    }
    cout << endl;
}

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
        bool debug_;
        bool all_;
        std::vector<std::string> keywords_;
        tempi::NodeFactory factory_;
        std::tr1::shared_ptr<Printer> printer_;
        void printAll();
        void printListOfTypes();
};

bool stringIsInVector(const std::string &text, const std::vector<std::string> &vec)
{
    // FIXME: optimize this
    std::vector<std::string>::const_iterator iter;
    for (iter = vec.begin(); iter != vec.end(); ++iter)
    {
        if ((*iter) == text)
            return true;
    }
    return false;
}

void print_rst_title(const std::string &text, TitleLevel level)
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
    cout << endl;
    cout << text << endl;
    print_n_times(text.size(), utils::charToString(character).c_str());
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
    verbose_(false),
    debug_(false),
    all_(false)
{
    this->printer_ = std::tr1::shared_ptr<Printer>(new RstPrinter);
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
        this->printer_->printAllClassInfo(node);
    }
    else
        std::cerr << "No such node type: \"" << name << "\"" << std::endl;
}

void TempiInspect::printListOfTypes()
{
    print_rst_title(std::string("Tempi types:"), FIRST);
    std::map<std::string, AbstractNodeType::ptr>::const_iterator iter;
    std::map<std::string, AbstractNodeType::ptr> entries = factory_.getEntries();
    for (iter = entries.begin(); iter != entries.end(); ++iter)
    {
        print_bullet_line_if_not_empty((*iter).first);
    }
}

void TempiInspect::printAll()
{
    // print_rst_title(std::string("Tempi types:"), FIRST);
    std::cout << "Tempi base plugins version " << PACKAGE_VERSION << std::endl << std::endl;
    std::map<std::string, AbstractNodeType::ptr>::const_iterator iter;
    std::map<std::string, AbstractNodeType::ptr> entries = factory_.getEntries();
    for (iter = entries.begin(); iter != entries.end(); ++iter)
    {
        printClass((*iter).first);
    }
}

bool TempiInspect::run()
{
    internals::loadInternals(factory_);
    if (all_)
        printAll();
    else
    {
        if (keywords_.size() == 0)
        {
            printListOfTypes();
        }
        else
        {
            std::vector<std::string>::const_iterator iter;
            for (iter = keywords_.begin(); iter != keywords_.end(); ++iter)
                printClass((*iter));
        }
    }
    return true;
}

int TempiInspect::parse_options(int argc, char **argv)
{
    GError* error = NULL;
    GOptionContext* context;

    context = g_option_context_new(" - tempi-inspect");
    g_option_context_add_main_entries(context, entries, NULL);

    
    if(!g_option_context_parse(context, &argc, &argv, &error))
    {
        return 1;
    }

    if(version)
    {
        std::cout << PROGRAM_NAME << " " << PACKAGE_VERSION << std::endl;
        return 0;
    }
    if(keywords != NULL)
    {
        guint numWords;
        numWords = g_strv_length(keywords);

        for(guint i=0; i<numWords; ++i)
        {
            keywords_.push_back(keywords[i]);
        }
    }
    if(all == TRUE)
    {
        std::cout << "Detailed info for all node types:" << std::endl;
        all_ = true;
    }
    if(verbose)
    {
        verbose_ = verbose;

        tempi::Logger::getInstance().setLevel(tempi::INFO);
        tempi::Logger::log(INFO, "Set logging level to INFO");
    }
    if(debug)
    {
        debug_ = debug;

        tempi::Logger::getInstance().setLevel(tempi::DEBUG);
        tempi::Logger::log(INFO, "Set logging level to DEBUG");
    }

    /*if(keywords == NULL && all != TRUE)
        return 1;
    else
        return -1;*/
    return -1;
}

int main(int argc, char *argv[])
{
    int ret;
    TempiInspect app;
    /*try
    {
        ret = app.parse_options(argc, argv);
    }
    catch (const boost::bad_any_cast &e)
    {
        std::cerr << "Error parsing options ";
        std::cerr << e.what() << std::endl;
        return 1;
    }*/

    ret = app.parse_options(argc, argv);
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

