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

/**
 * @file An OSC looper.
 */

#include <iostream>
#include "tempi/config.h"
#ifndef HAVE_CLUTTER
int main(int argc, char *argv[])
{
    std::cout << "No Clutter support" << std::endl;
    return 1;
}
#else //HAVE_CLUTTER

#include "tempi/message.h"
#include "tempi/scheduler.h"
#include "tempi/wrapper.h"
#include "tempi/log.h"
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/program_options.hpp>
#include <clutter/clutter.h>
#include <glib.h>
#include <sstream>

// namespaces:
namespace po = boost::program_options;

// Clutter legacy macro aliases:
#if CLUTTER_CHECK_VERSION(1,4,0)
#else
#define CLUTTER_KEY_Down CLUTTER_Down
#define CLUTTER_KEY_Escape CLUTTER_Escape
#define CLUTTER_KEY_F1 CLUTTER_F1
#define CLUTTER_KEY_Left CLUTTER_Left
#define CLUTTER_KEY_Return CLUTTER_Return
#define CLUTTER_KEY_Right CLUTTER_Right
#define CLUTTER_KEY_Tab CLUTTER_Tab
#define CLUTTER_KEY_Up CLUTTER_Up
#define CLUTTER_KEY_q CLUTTER_q
#define CLUTTER_KEY_space CLUTTER_space
#define CLUTTER_KEY_Delete CLUTTER_Delete
#define CLUTTER_KEY_BackSpace CLUTTER_BackSpace
#endif

namespace miller {

// String constants:
static const char *PROGRAM_NAME = "miller";
static const char *GRAPH_NAME = "graph0";

// Color constants:
static ClutterColor black = { 0x00, 0x00, 0x00, 0xff };
static ClutterColor red = { 0xcc, 0x33, 0x33, 0xff };
static ClutterColor green = { 0x33, 0xcc, 0x33, 0xff };
static ClutterColor gray = { 0x99, 0x99, 0x99, 0xff };
static ClutterColor white = { 0xcc, 0xcc, 0xcc, 0xff };

// Static functions:
static void key_event_cb(ClutterActor *actor, ClutterKeyEvent *event, gpointer user_data);
static void on_frame_cb(ClutterTimeline *timeline, guint *ms, gpointer user_data);

/**
 * The App class manages the tempi::Graph and the Clutter GUI.
 */
class App
{
    public:
        App();
        ~App();
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
        bool verbose_;
        bool debug_;
        std::string file_name_;
        tempi::Wrapper engine_;
        ClutterActor *stage_;
        bool createGUI();
        void setupGraph();
};

App::App() :
    verbose_(false),
    debug_(false)
{
    file_name_ = "";
    stage_ = NULL;
}

App::~App()
{
    // pass
}

static void key_event_cb(ClutterActor *actor, ClutterKeyEvent *event, gpointer user_data)
{
    App *app = (App *) user_data;
    ClutterModifierType state = clutter_event_get_state((ClutterEvent*) event);
    bool ctrl_pressed = (state & CLUTTER_CONTROL_MASK ? true : false);
    switch (event->keyval)
    {
        case CLUTTER_KEY_q:
            if (ctrl_pressed)
                clutter_main_quit();
            break;
    }
}

bool App::poll()
{
    if (stage_ == 0)
    {
        std::cerr << "App::" << __FUNCTION__ << "(): Error: must call launch() first.\n";
        return false;
    }
    else
    {
        return true;
    }
}


bool App::launch()
{
    if (stage_ == 0)
    {
        createGUI();
        setupGraph();
        if (verbose_)
            std::cout << "Running... Press ctrl-C in the terminal to quit. (or ctrl-Q in the GUI)" << std::endl;
        return true;
    }
    else
    {
        std::cerr << "App::" << __FUNCTION__ << "(): Already called\n";
        return false;
    }
}

void App::setupGraph()
{
    if (verbose_)
        this->engine_.setLogLevel("INFO");
    if (debug_)
        this->engine_.setLogLevel("DEBUG");
    if (this->file_name_ == "")
    {
        tempi::Logger::log(tempi::WARNING, "No file name provided.");
    }
    else
    {
        std::ostringstream os;
        os << "Loading file " << this->file_name_;
        tempi::Logger::log(tempi::INFO, os);
        this->engine_.loadGraph(GRAPH_NAME, this->file_name_);
    }
}

static void on_frame_cb(ClutterTimeline * /*timeline*/, guint * /*ms*/, gpointer user_data)
{
    App *context = static_cast<App*>(user_data);
    context->poll();
}

bool App::createGUI()
{
    if (stage_ == 0)
    {
        std::cout << "Creating GUI.\n";
    }
    else
    {
        std::cerr << "App::" << __FUNCTION__ << ": Stage already created.\n"; 
        return false;
    }
    stage_ = clutter_stage_get_default();
    clutter_actor_set_size(stage_, 800, 600);
    clutter_stage_set_color(CLUTTER_STAGE(stage_), &black);
    g_signal_connect(stage_, "destroy", G_CALLBACK(clutter_main_quit), NULL);
    
    // timeline to attach a callback for each frame that is rendered
    ClutterTimeline *timeline;
    timeline = clutter_timeline_new(60); // ms
    clutter_timeline_set_loop(timeline, TRUE);
    clutter_timeline_start(timeline);

    g_signal_connect(timeline, "new-frame", G_CALLBACK(on_frame_cb), this);
    g_signal_connect(stage_, "key-press-event", G_CALLBACK(key_event_cb), this);

    clutter_actor_show(stage_);
    return true;
}

int App::parse_options(int argc, char **argv)
{
    po::options_description desc("Options");
    desc.add_options()
        ("help,h", "Show this help message and exit")
        ("version", "Show program's version number and exit")
        ("file,f", po::value<std::string>(), // ->default_value("")
            "Specifies a Tempi file to read.")
        ("verbose,v", po::bool_switch(), "Enables a verbose output")
        ("debug,d", po::bool_switch(), "Enables a very verbose output")
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
    debug_ = options["debug"].as<bool>();
    if (options.count("file"))
    {
        file_name_ = options["file"].as<std::string>();
    }
    if (verbose_)
    {
        std::cout << "File name: " << file_name_ << std::endl;
        std::cout << "Verbose: " << verbose_ << std::endl;
        std::cout << "Debug: " << debug_ << std::endl;
    }
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

} // end of namespace

int main(int argc, char *argv[])
{
    using namespace miller;
    int ret;
    App app;
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

    if (clutter_init(&argc, &argv) != CLUTTER_INIT_SUCCESS)
        return 1;

    app.launch();
    clutter_main();
    return 0;
}

#endif //HAVE_CLUTTER

