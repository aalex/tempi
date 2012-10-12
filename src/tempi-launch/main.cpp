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
 * @file The tempi application.
 */

#include <iostream>
#include "tempi/config.h"

#ifndef HAVE_GLIB
int main(int argc, char *argv[])
{
    std::cout << "No glib support" << std::endl;
    return 1;
}
#else // HAVE_GLIB

#include "tempi/message.h"
#include "tempi/scheduler.h"
#include "tempi/threadedscheduler.h"
#include "tempi/serializer.h"
#include "tempi/log.h"
#include <boost/lexical_cast.hpp>
#include <glib.h>
#include <boost/program_options.hpp>
#include <sstream>

// namespaces:
namespace po = boost::program_options;
using tempi::INFO;
using tempi::DEBUG;

// String constants:
static const char *PROGRAM_NAME = "tempi-launch";

static gboolean help = FALSE;
static gboolean version = FALSE;
static gboolean verbose = FALSE;
static gboolean debug = FALSE;
static gchar** filenames = NULL;

static GOptionEntry entries[] =
{
    {"version", 0, 0, G_OPTION_ARG_NONE, &version, "Show program's version number and exit"},
    {"verbose", 'v', 0, G_OPTION_ARG_NONE, &verbose, "Enables a verbose output"},
    {"debug", 'd', 0, G_OPTION_ARG_NONE, &debug, "Enables a very verbose output"},
    {G_OPTION_REMAINING, 0, 0, G_OPTION_ARG_FILENAME_ARRAY, &filenames, "Tempi files to read"},
    {NULL}
};

/**
 * The TempiLauncher class is the tempi-launch application.
 */
class TempiLauncher
{
    public:
        TempiLauncher();
        ~TempiLauncher();
        /**
         * Return -1 if it's ok to run the program, or retuns 0 or 1 if we should terminate it.
         * Call this begore launching the app.
         */
        int parse_options(int argc, char **argv);
        /**
         * Creates the tempi::Graph, and the ClutterStage
         */
        bool launch();
        bool getVerbose() const;
        bool getDebug() const;
    private:
        std::string fileName_;
        bool graph_ok_;
        bool verbose_;
        bool debug_;
        tempi::ThreadedScheduler::ptr engine_;
        tempi::Graph::ptr graph_;
        bool setupGraph();
};

TempiLauncher::TempiLauncher() :
    verbose_(false),
    debug_(false),
    graph_ok_(false)
{
}

bool TempiLauncher::getVerbose() const
{
    return verbose_;
}

TempiLauncher::~TempiLauncher()
{
    if (engine_.get() != 0)
    {
        {
            std::ostringstream os;
            os << __FUNCTION__ << "(): Waiting for Scheduler's thread to join.";
            tempi::Logger::log(DEBUG, os.str().c_str());
        }
        engine_->stop();
    }
}

bool TempiLauncher::setupGraph()
{
    using tempi::Message;
    if (graph_ok_)
    {
        std::cerr << "tempi-launch: TempiLauncher::" << __FUNCTION__ << ": already called.\n";
        return false;
    }

    // Check for XML file
    if (! tempi::serializer::fileExists(fileName_.c_str()))
    {
        std::cerr << "tempi-launch: ERROR: File \"" << fileName_ << "\" not found!\n";
        return false;
    }
    {
        std::ostringstream os;
        os << "tempi-launch: Found " << fileName_;
        tempi::Logger::log(DEBUG, os.str().c_str());
    }
    // Create Scheduler
    {
        std::ostringstream os;
        os << "tempi-launch: Create ThreadedScheduler\n";
        tempi::Logger::log(DEBUG, os.str().c_str());
    }
    engine_.reset(new tempi::ThreadedScheduler);
    engine_->start(5); // time precision in ms
    // TODO: make time precision configurable
    //if (verbose_)
    //    std::cout << (*engine_.get()) << std::endl;
    tempi::ScopedLock::ptr lock = engine_->acquireLock();
    {
        std::ostringstream os;
        os << "tempi-launch: Create Graph\n";
        tempi::Logger::log(DEBUG, os.str().c_str());
    }

    engine_->createGraph("graph0");
    graph_ = engine_->getGraph("graph0");

    // load graph
    tempi::serializer::load(*graph_.get(), fileName_.c_str());
    graph_->tick(); // FIXME

    graph_ok_ = true;
    if (debug_)
    {
        //engine_->sleepThisThread(6.0f);
        std::cout << (*engine_.get()) << std::endl;
    }
    {
        std::ostringstream os;
        os << "tempi-launch: Loaded " << fileName_;
        tempi::Logger::log(DEBUG, os.str().c_str());
    }
    return true;
}

bool TempiLauncher::launch()
{
    if (graph_ok_)
    {
        std::cerr << "TempiLauncher::" << __FUNCTION__ << "(): Already called\n";
        return false;
    }
    else
    {
        if (setupGraph())
        {
            if (verbose_)
                std::cout << "Running... Press ctrl-C in the terminal to quit." << std::endl;
            return true;
        }
        else
            return false;
    }
}

int TempiLauncher::parse_options(int argc, char **argv)
{
    GError* error = NULL;
    GOptionContext* context;

    context = g_option_context_new(" - tempi-launch");
    g_option_context_add_main_entries(context, entries, NULL);
    //g_option_context_add_group(context, g_option_context_get_main_group(context));
    
    if(!g_option_context_parse(context, &argc, &argv, &error))
    {
        return 1;
    }

    if(version)
    {
        std::cout << PROGRAM_NAME << " " << PACKAGE_VERSION << std::endl;
        return 0;
    }
    if(filenames != NULL)
    {
        guint numFiles;
        numFiles = g_strv_length(filenames);

        for(guint i=0; i<numFiles; ++i)
        {
            g_print("Trying to load file: %s\n", filenames[i]);
        }

        // At the moment we load just the first file specified
        fileName_ = filenames[0];
        std::cout << fileName_ << std::endl;
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

    {
        std::ostringstream os;
        os << "XML file to load: " << fileName_;
        tempi::Logger::log(DEBUG, os.str().c_str());
    }

    return -1;
}


int main(int argc, char *argv[])
{
    int ret;
    TempiLauncher app;

    ret = app.parse_options(argc, argv); 
    if (ret != -1)
        return ret;

    bool ok = app.launch();
    if (! ok)
    {
        tempi::Logger::log(tempi::CRITICAL, "Error calling app.launch()");
        return 1;
    }

    g_thread_init(NULL);
    GMainLoop *mainLoop = g_main_loop_new(NULL, FALSE);
    //g_idle_add(on_idle, (gpointer) &app);
    tempi::Logger::log(DEBUG, "Run main loop.");
    g_main_loop_run(mainLoop);
    g_main_loop_unref(mainLoop);
    return 0;
}

#endif // HAVE_GLIB

