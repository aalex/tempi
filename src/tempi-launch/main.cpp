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

#include "tempi/config.h"
#include "tempi/message.h"
#include "tempi/scheduler.h"
#include "tempi/threadedscheduler.h"
#include "tempi/serializer.h"
#include "tempi/log.h"
#include <boost/lexical_cast.hpp>
#include <glib.h>
#include <boost/program_options.hpp>
#include <iostream>
#include <sstream>

// namespaces:
namespace po = boost::program_options;

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
    private:
        std::string fileName_;
        bool graph_ok_;
        bool verbose_;
        tempi::ThreadedScheduler::ptr engine_;
        tempi::serializer::Serializer::ptr saver_;
        tempi::Graph::ptr graph_;
        bool setupGraph();
};

TempiLauncher::TempiLauncher() :
    verbose_(false),
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
        if (verbose_)
            std::cout << __FUNCTION__ << "(): Waiting for Scheduler's thread to join." << std::endl;
        engine_->stop();
    }
}

bool TempiLauncher::setupGraph()
{
    using tempi::Message;
    if (graph_ok_)
    {
        std::cerr << "TempiLauncher::" << __FUNCTION__ << ": already called.\n";
        return false;
    }

    // Check for XML file
    if (! saver_->fileExists(fileName_.c_str()))
    {
        std::cerr << "ERROR: File \"" << fileName_ << "\" not found!\n";
        return false;
    }
    else if (verbose_)
        std::cout << "Found " << fileName_ << std::endl;

    // Create Scheduler
    if (verbose_)
        std::cout << "Create ThreadedScheduler\n";
    engine_.reset(new tempi::ThreadedScheduler);
    engine_->start(5); // time precision in ms
    // TODO: make time precision configurable
    //if (verbose_)
    //    std::cout << (*engine_.get()) << std::endl;
    tempi::ScopedLock::ptr lock = engine_->acquireLock();
    if (verbose_)
        std::cout << "Create Graph\n";
    
    engine_->createGraph("graph0");
    graph_ = engine_->getGraph("graph0");

    // load graph
    saver_->load(*graph_.get(), fileName_.c_str());
    graph_->tick(); // FIXME

    graph_ok_ = true;
    if (verbose_)
    {
        //engine_->sleepThisThread(6.0f);
        std::cout << (*engine_.get()) << std::endl;
    }
    if (verbose_)
        std::cout << "Loaded " << fileName_ << std::endl;
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
    fileName_ = options["file"].as<std::string>();
    if (verbose_)
        std::cout << "XML file to load: " << fileName_ << std::endl;
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
        tempi::log::Logger& logger = tempi::log::Logger::getInstance();
        logger.setLevel(tempi::log::DEBUG);
    }
    return -1;
}

static gboolean on_idle(gpointer data)
{
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
        std::cerr << "Error calling app.launch()\n";
        return 1;
    }

    g_thread_init(NULL);
    GMainLoop *mainLoop = g_main_loop_new(NULL, FALSE);
    g_idle_add(on_idle, (gpointer) &app);
    if (app.getVerbose())
        std::cout << "Run main loop.\n";
    g_main_loop_run(mainLoop);

    g_main_loop_unref(mainLoop);
    return 0;
}

