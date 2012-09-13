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
#include "tempi/log.h"

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
        tempi::serializer::Serializer::ptr saver_;
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
    if (! saver_->fileExists(fileName_.c_str()))
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
    saver_->load(*graph_.get(), fileName_.c_str());
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
    po::options_description desc("Options");
    desc.add_options()
        ("help,h", "Show this help message and exit")
        ("version", "Show program's version number and exit")
        ("file,f", po::value<std::string>()->default_value(""), "Sets the XML file to load")
        ("verbose,v", po::bool_switch(), "Enables a verbose output")
        ("debug,d", po::bool_switch(), "Enables a very verbose output")
        ;
    po::variables_map options;
    try
    {
        // all positional options should be translated into "file" options
        po::positional_options_description p;
        p.add("file", -1);
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
    debug_ = options["debug"].as<bool>();
    fileName_ = options["file"].as<std::string>();
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
    {
        tempi::Logger::getInstance().setLevel(tempi::INFO);
        tempi::Logger::log(INFO, "Set logging level to INFO");
    }
    if (debug_)
    {
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

static gboolean on_idle(gpointer data)
{
    g_usleep(1000000);
    TempiLauncher *app = (TempiLauncher *) data;
    (void) app;
    //if (app->getVerbose())
    //   std::cout << __FUNCTION__ << std::endl;
    return TRUE; // stay registered
}

int main(int argc, char *argv[])
{
    int ret;
    TempiLauncher app;
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

    bool ok = app.launch();
    if (! ok)
    {
        tempi::Logger::log(tempi::CRITICAL, "Error calling app.launch()");
        return 1;
    }

    g_thread_init(NULL);
    GMainLoop *mainLoop = g_main_loop_new(NULL, FALSE);
    g_idle_add(on_idle, (gpointer) &app);
    tempi::Logger::log(DEBUG, "Run main loop.");
    g_main_loop_run(mainLoop);
    g_main_loop_unref(mainLoop);
    return 0;
}

#endif // HAVE_GLIB

