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
 * @file A graphical patcher.
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

#include "miller-macros.h"
#include "miller-command.h"
#include "miller-draw.h"
#include "tempi/message.h"
#include "tempi/scheduler.h"
#include "tempi/scheduler.h"
#include "tempi/threadedscheduler.h"
#include "tempi/serializer.h"
#include "tempi/log.h"
#include "tempi/wrapper.h"
#include "tempi/log.h"
#include "tempi/concurrentqueue.h"
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/program_options.hpp>
#include <clutter/clutter.h>
#include <glib.h>
#include <sstream>

namespace miller {

// String constants:
static const char * const PROGRAM_NAME = "miller";
static const char * const GRAPH_NAME = "graph0";
static const char * const NODES_GROUP = "group0";
static const char * const CONNECTIONS_ACTOR = "connections0";
static const char * const HELP_TEXT_ACTOR = "help0";

// Static functions:
static void key_event_cb(ClutterActor *actor, ClutterKeyEvent *event, gpointer user_data);
static void on_frame_cb(ClutterTimeline *timeline, guint *ms, gpointer user_data);
static void on_fullscreen(ClutterStage* stage, gpointer user_data);
static void on_unfullscreen(ClutterStage* stage, gpointer user_data);

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
        void toggle_fullscreen();
        void toggle_help();
    private:
        bool verbose_;
        bool debug_;
        bool graph_ok_;

        //GRand *random_generator_;
        tempi::ConcurrentQueue<Command::ptr> commands_queue_;
    public:
        std::string file_name_;
        tempi::ThreadedScheduler::ptr engine_; // FIXME
        tempi::Graph::ptr graph_; // FIXME
        bool saveGraph(); // FIXME
        ClutterActor *stage_;
    private:
        bool createGUI();
        bool setupGraph();
        // called by setupGraph() to create the clutter actors
        void drawGraph();
        static gboolean on_idle(gpointer data);
        void pushCommand(Command::ptr command);
        static gboolean on_group0_scrolled(ClutterActor *actor, ClutterEvent *event, gpointer user_data);
};

void App::toggle_help()
{
    ClutterActor *actor = clutter_container_find_child_by_name(CLUTTER_CONTAINER(this->stage_), HELP_TEXT_ACTOR);
    if (CLUTTER_ACTOR_IS_VISIBLE(actor))
        clutter_actor_hide(actor);
    else
        clutter_actor_show(actor);
}

bool SaveCommand::apply(App &app)
{
    tempi::Logger::log(tempi::WARNING, "will save the graph.");
    tempi::ScopedLock::ptr lock = app.engine_->acquireLock();
    tempi::Graph::ptr graph = app.graph_;
    bool ok = tempi::serializer::Serializer::save(*graph.get(), app.file_name_.c_str());
    if (ok)
        tempi::Logger::log(tempi::INFO, "Successfully saved the graph..");
    else
        tempi::Logger::log(tempi::ERROR, "Could not save the graph.");
    return ok;
}

void App::pushCommand(Command::ptr command)
{
    tempi::Logger::log(tempi::WARNING, "pushing a command.");
    commands_queue_.push(command);
}

App::App() :
    verbose_(false),
    debug_(false),
    graph_ok_(false)
{
    //random_generator_ = g_rand_new();
    file_name_ = "";
    stage_ = NULL;
}

App::~App()
{
    // pass
    if (this->engine_.get() != 0)
    {
        {
            std::ostringstream os;
            os << __FUNCTION__ << "(): Waiting for Scheduler's thread to join.";
            tempi::Logger::log(tempi::DEBUG, os);
        }
        this->engine_->stop();
    }
}

void App::toggle_fullscreen()
{
    if (verbose_)
        std::cout << "App::" << __FUNCTION__ << std::endl;
    if (clutter_stage_get_fullscreen(CLUTTER_STAGE(stage_)))
        clutter_stage_set_fullscreen(CLUTTER_STAGE(stage_), FALSE);
    else
        clutter_stage_set_fullscreen(CLUTTER_STAGE(stage_), TRUE);
}

void key_event_cb(ClutterActor *actor, ClutterKeyEvent *event, gpointer user_data)
{
    App *app = (App *) user_data;
    ClutterModifierType state = clutter_event_get_state((ClutterEvent*) event);
    bool ctrl_pressed = (state & CLUTTER_CONTROL_MASK ? true : false);
    switch (event->keyval)
    {
        case CLUTTER_KEY_F1:
            app->toggle_help();
            break;
        case CLUTTER_KEY_F11:
            app->toggle_fullscreen();
            break;
        case CLUTTER_KEY_q:
            if (ctrl_pressed)
                clutter_main_quit();
            break;
        case CLUTTER_KEY_s:
            if (ctrl_pressed)
                app->saveGraph();
            break;
    }
}

void on_fullscreen(ClutterStage* stage, gpointer user_data)
{
    UNUSED(stage);
    UNUSED(user_data);
}

void on_unfullscreen(ClutterStage* stage, gpointer user_data)
{
    UNUSED(stage);
    UNUSED(user_data);
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

gboolean App::on_idle(gpointer data)
{
    //clutter_threads_enter();
    App *app = static_cast<App *>(data);
    while (! app->commands_queue_.empty())
    {
        tempi::Logger::log(tempi::WARNING, "popping a command.");
        Command::ptr command;
        app->commands_queue_.try_pop(command);
        command->apply(*app);
    }
    // FIXME: seems to crash
    //clutter_threads_leave();
    g_usleep(1000);
    return TRUE;
}

bool App::launch()
{
    if (verbose_)
        tempi::Logger::getInstance().setLevel(tempi::INFO);
    if (debug_)
        tempi::Logger::getInstance().setLevel(tempi::DEBUG);
    if (stage_ == 0)
    {
        createGUI();
        if (! setupGraph())
            return false;
        if (verbose_)
            std::cout << "Running... Press ctrl-C in the terminal to quit. (or ctrl-Q in the GUI)" << std::endl;
        //g_idle_add(App::on_idle, this);
        clutter_threads_add_idle(App::on_idle, this);
        return true;
    }
    else
    {
        std::cerr << "App::" << __FUNCTION__ << "(): Already called\n";
        return false;
    }
}

void on_node_dragged(ClutterDragAction *action, ClutterActor *actor, gfloat delta_x, gfloat delta_y, gpointer user_data)
{
    UNUSED(action);
    App *app = static_cast<App *>(user_data);

    gfloat motion_x;
    gfloat motion_y;
    clutter_drag_action_get_motion_coords(action, &motion_x, &motion_y);

    tempi::ScopedLock::ptr lock = app->engine_->acquireLock();
    const gchar *name = clutter_actor_get_name(actor);
    tempi::Node::ptr node = app->graph_->getNode(name);
    node->setAttributeValue(tempi::Node::ATTRIBUTE_POSITION, tempi::Message("fff", motion_x, motion_y, 0.0f));

    // TODO: update only the connections we need to update
    updateAllConnectionsGeometry(
       clutter_container_find_child_by_name(CLUTTER_CONTAINER(app->stage_), CONNECTIONS_ACTOR),
       clutter_container_find_child_by_name(CLUTTER_CONTAINER(app->stage_), NODES_GROUP),
       *(app->graph_.get()));
}

void App::drawGraph()
{
    // the tempi scoped lock is already acquired be the caller
    // assumes that graph_ is a valid tempi::Graph.
    std::vector<std::string> names = graph_->getNodeNames();
    std::vector<std::string>::const_iterator iter;
    for (iter = names.begin(); iter != names.end(); ++iter)
    {
        //float x = (float) g_rand_double_range(random_generator_, 0.0, clutter_actor_get_width(stage_));
        //float y = (float) g_rand_double_range(random_generator_, 0.0, clutter_actor_get_height(stage_));
        std::string node_name = (*iter);
        tempi::Node::ptr node = graph_->getNode(node_name.c_str());
        tempi::Message pos = node->getAttributeValue(tempi::Node::ATTRIBUTE_POSITION);
        float x = pos.getFloat(0);
        float y = pos.getFloat(1);
        ClutterActor *actor = createNodeActor(*node.get());
        clutter_actor_set_position(actor, x, y);
        ClutterAction *action = clutter_drag_action_new();
        clutter_actor_add_action(actor, action);
        g_signal_connect(action, "drag-motion", G_CALLBACK(on_node_dragged), this);
        clutter_actor_set_reactive(actor, TRUE);
        clutter_container_add_actor(CLUTTER_CONTAINER(clutter_container_find_child_by_name(CLUTTER_CONTAINER(stage_), NODES_GROUP)), actor);
    }

    ClutterActor *connections_actor = createConnectionsActor(*graph_.get());
    clutter_actor_set_name(connections_actor, CONNECTIONS_ACTOR);
    clutter_container_add_actor(CLUTTER_CONTAINER(stage_), connections_actor);
    clutter_actor_show(connections_actor);

    updateAllConnectionsGeometry(connections_actor, clutter_container_find_child_by_name(CLUTTER_CONTAINER(stage_), NODES_GROUP), *graph_.get());
}

bool App::saveGraph()
{
    if (file_name_.c_str() == 0)
    {
        std::ostringstream os;
        os << "Cannot save graph to a file, since no file name has been provided.";
        tempi::Logger::log(tempi::WARNING, os);
        return false;
    }
    else
        pushCommand(Command::ptr(new SaveCommand));
    return true; // ???
}

bool App::setupGraph()
{
    using tempi::Message;
    if (graph_ok_)
    {
        std::cerr << "miller: TempiLauncher::" << __FUNCTION__ << ": already called.\n";
        return false;
    }
    // Create Scheduler
    {
        std::ostringstream os;
        os << "miller: Create ThreadedScheduler\n";
        tempi::Logger::log(tempi::DEBUG, os);
    }
    engine_.reset(new tempi::ThreadedScheduler);
    // TODO: make time precision configurable
    engine_->start(5); // time precision in ms
    tempi::ScopedLock::ptr lock = engine_->acquireLock();
    {
        std::ostringstream os;
        os << "miller: Create Graph\n";
        tempi::Logger::log(tempi::DEBUG, os);
    }
    engine_->createGraph(GRAPH_NAME);

    if (this->file_name_ == "")
    {
        tempi::Logger::log(tempi::WARNING, "No file name provided.");
        return true;
    }
    // Check for XML file
    if (! tempi::serializer::Serializer::fileExists(this->file_name_.c_str()))
    {
        std::cerr << "miller: ERROR: File \"" << this->file_name_ << "\" not found!\n";
        return false;
    }
    else
    {
        std::ostringstream os;
        os << "miller: Found " << this->file_name_;
        tempi::Logger::log(tempi::DEBUG, os);
    }
    //if (verbose_)
    //    std::cout << (*engine_.get()) << std::endl;

    graph_ = engine_->getGraph(GRAPH_NAME);

    // load graph
    tempi::serializer::Serializer::load(*graph_.get(), this->file_name_.c_str());
    graph_->tick(); // FIXME

    this->drawGraph();

    graph_ok_ = true;
    if (debug_)
    {
        //engine_->sleepThisThread(6.0f);
        std::cout << (*engine_.get()) << std::endl;
    }
    {
        std::ostringstream os;
        os << "miller: Loaded " << this->file_name_;
        tempi::Logger::log(tempi::DEBUG, os);
    }
    return true;
}

void on_frame_cb(ClutterTimeline * /*timeline*/, guint * /*ms*/, gpointer user_data)
{
    App *context = static_cast<App*>(user_data);
    context->poll();
}

gboolean App::on_group0_scrolled(ClutterActor *actor, ClutterEvent *event, gpointer user_data)
{
    ClutterScrollDirection direction;
    direction = clutter_event_get_scroll_direction(event);
    gdouble current_x_factor;
    gdouble current_y_factor;
    clutter_actor_get_scale(actor, &current_x_factor, &current_y_factor);
    static const gdouble SCALE_UP = 5.0 / 4.0;
    static const gdouble SCALE_DOWN = 3.0 / 4.0;

    std::ostringstream os;
    os << "scale is " << current_x_factor << " x " << current_y_factor;
    tempi::Logger::log(tempi::WARNING, os.str().c_str());

    switch (direction)
    {
        case CLUTTER_SCROLL_UP:
            clutter_actor_set_scale(actor,
              current_x_factor * SCALE_UP,
              current_y_factor * SCALE_UP);
            break;
        case CLUTTER_SCROLL_DOWN:
            clutter_actor_set_scale(actor,
              current_x_factor * SCALE_DOWN,
              current_y_factor * SCALE_DOWN);
            break;
      }

    return CLUTTER_EVENT_STOP; /* event has been handled */
}


bool App::createGUI()
{
    if (stage_ == 0)
    {
        //std::cout << "Creating GUI.\n";
    }
    else
    {
        std::cerr << "App::" << __FUNCTION__ << ": Stage already created.\n";
        return false;
    }
    stage_ = clutter_stage_get_default();
    clutter_actor_set_size(stage_, 800, 600);
    clutter_stage_set_user_resizable(CLUTTER_STAGE(stage_), TRUE);
    clutter_stage_set_color(CLUTTER_STAGE(stage_), &BLACK);
    g_signal_connect(stage_, "destroy", G_CALLBACK(clutter_main_quit), NULL);
    g_signal_connect(G_OBJECT(stage_), "fullscreen", G_CALLBACK(on_fullscreen), this);
    g_signal_connect(G_OBJECT(stage_), "unfullscreen", G_CALLBACK(on_unfullscreen), this);

    // timeline to attach a callback for each frame that is rendered
    ClutterTimeline *timeline;
    timeline = clutter_timeline_new(60); // ms
    clutter_timeline_set_loop(timeline, TRUE);
    clutter_timeline_start(timeline);

    g_signal_connect(timeline, "new-frame", G_CALLBACK(on_frame_cb), this);
    g_signal_connect(stage_, "key-press-event", G_CALLBACK(key_event_cb), this);

    // create misc actors in the stage:
    ClutterActor *group0 = clutter_group_new();
    clutter_actor_set_name(group0, NODES_GROUP);
    clutter_actor_set_reactive(group0, TRUE);
    g_signal_connect(group0, "scroll-event", G_CALLBACK(App::on_group0_scrolled), this);
    clutter_container_add_actor(CLUTTER_CONTAINER(stage_), group0);

    ClutterActor *help_text = clutter_text_new_full(FONT_NAME, HELP_TEXT_CONTENTS, &WHITE);
    clutter_container_add_actor(CLUTTER_CONTAINER(stage_), help_text);
    clutter_actor_hide(help_text);
    clutter_actor_set_name(help_text, HELP_TEXT_ACTOR);

    clutter_actor_show(stage_);
    return true;
}

int App::parse_options(int argc, char **argv)
{
    namespace po = boost::program_options;

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

    // Important, since we use threads:
    g_thread_init(NULL);
    clutter_threads_init();

    if (clutter_init(&argc, &argv) != CLUTTER_INIT_SUCCESS)
        return 1;

    if (app.launch())
        clutter_main();
    else
        return 1;
    return 0;
}

#endif //HAVE_CLUTTER

