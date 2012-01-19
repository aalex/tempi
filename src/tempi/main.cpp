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
#include <boost/lexical_cast.hpp>
#include <glib.h>
#include <boost/program_options.hpp>
#include <iostream>
#include <sstream>

// namespaces:
namespace po = boost::program_options;

// String constants:
static const char *PROGRAM_NAME = "tempi";

/**
 * The TempiApp class manages the tempi::Graph
 */
class TempiApp
{
    public:
        TempiApp();
        ~TempiApp();
        /**
         * Return -1 if it's ok to run the program, or retuns 0 or 1 if we should terminate it.
         * Call this begore launching the app.
         */
        int parse_options(int argc, char **argv);
        /**
         * Creates the tempi::Graph, and the ClutterStage
         */
        bool launch();
        /**
         * Polls the tempi::Graph.
         */
        bool poll();
    private:
        std::string fileName_;
        bool graph_ok_;
        bool verbose_;
        tempi::ThreadedScheduler::ptr engine_;
        tempi::serializer::Serializer::ptr saver_;
        tempi::Graph::ptr graph_;
        bool setupGraph();
};

TempiApp::TempiApp() :
    verbose_(false),
    graph_ok_(false)
{
}

TempiApp::~TempiApp()
{
    if (engine_.get() != 0)
    {
        if (verbose_)
            std::cout << "Waiting for Scheduler's thread to join." << std::endl;
        engine_->stop();
    }
}

bool TempiApp::poll()
{
    if (graph_ok_)
    {
        //engine_->tick();
        return true;
    }
    else
    {
        std::cerr << "TempiApp::" << __FUNCTION__ << "(): Error: must call launch() first.\n";
        return false;
    }
}

bool TempiApp::setupGraph()
{
    using tempi::Message;
    if (graph_ok_)
    {
        std::cerr << "TempiApp::" << __FUNCTION__ << ": already called.\n";
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
    if (verbose_)
        std::cout << (*engine_.get()) << std::endl;
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
}

bool TempiApp::launch()
{
    if (graph_ok_)
    {
        std::cerr << "TempiApp::" << __FUNCTION__ << "(): Already called\n";
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

int TempiApp::parse_options(int argc, char **argv)
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
        po::store(po::parse_command_line(argc, argv, desc), options);
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
    return -1;
}

static gboolean on_idle(gpointer data)
{
    //if (verbose_)
    //    std::cout << "Loaded " << fileName_ << std::endl;
    // std::cout << __FUNCTION__ << std::endl;
    TempiApp *app = (TempiApp *) data;
    app->poll();
    return TRUE;
}

int main(int argc, char *argv[])
{
    int ret;
    TempiApp app;
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
        return 1;

    GMainLoop *mainLoop = g_main_loop_new(NULL, FALSE);
    g_idle_add(on_idle, (gpointer) &app);
    g_main_loop_run(mainLoop);

    return 0;
}

