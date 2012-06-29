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

// namespaces:
namespace po = boost::program_options;
using tempi::INFO;
using tempi::DEBUG;

// Clutter legacy macro aliases:
#if CLUTTER_CHECK_VERSION(1,4,0)
#else
#define CLUTTER_KEY_Down CLUTTER_Down
#define CLUTTER_KEY_Escape CLUTTER_Escape
#define CLUTTER_KEY_F1 CLUTTER_F1
#define CLUTTER_KEY_F2 CLUTTER_F2
#define CLUTTER_KEY_F3 CLUTTER_F3
#define CLUTTER_KEY_F4 CLUTTER_F4
#define CLUTTER_KEY_F11 CLUTTER_F11
#define CLUTTER_KEY_Left CLUTTER_Left
#define CLUTTER_KEY_Return CLUTTER_Return
#define CLUTTER_KEY_Right CLUTTER_Right
#define CLUTTER_KEY_Tab CLUTTER_Tab
#define CLUTTER_KEY_Up CLUTTER_Up
#define CLUTTER_KEY_q CLUTTER_q
#define CLUTTER_KEY_s CLUTTER_s
#define CLUTTER_KEY_space CLUTTER_space
#define CLUTTER_KEY_Delete CLUTTER_Delete
#define CLUTTER_KEY_BackSpace CLUTTER_BackSpace
#endif

#ifndef UNUSED
#define UNUSED(x) ((void)(x))
#endif

namespace miller {

// String constants:
static const char * const PROGRAM_NAME = "miller";
static const char * const GRAPH_NAME = "graph0";
static const char * const NODES_GROUP = "group0";
static const char * const FONT_NAME = "Monospace Bold 12px";

// Color constants:
static const ClutterColor BLACK = { 0x00, 0x00, 0x00, 0xff };
static const ClutterColor BLUE = { 0x33, 0x33, 0xcc, 0xff };
static const ClutterColor CYAN = { 0x33, 0xcc, 0xcc, 0xff };
static const ClutterColor GRAY_LIGHT = { 0xcc, 0xcc, 0xcc, 0xff };
static const ClutterColor GRAY_MEDIUM = { 0x99, 0x99, 0x99, 0xff };
static const ClutterColor GREEN = { 0x33, 0xcc, 0x33, 0xff };
static const ClutterColor MAGENTA = { 0xcc, 0x33, 0xcc, 0xff };
static const ClutterColor RED = { 0xcc, 0x33, 0x33, 0xff };
static const ClutterColor WHITE = { 0xff, 0xff, 0xff, 0xff };
static const ClutterColor YELLOW = { 0xcc, 0xcc, 0x33, 0xff };

// Static functions:
static void key_event_cb(ClutterActor *actor, ClutterKeyEvent *event, gpointer user_data);
static void on_frame_cb(ClutterTimeline *timeline, guint *ms, gpointer user_data);
static void on_fullscreen(ClutterStage* stage, gpointer user_data);
static void on_unfullscreen(ClutterStage* stage, gpointer user_data);

class App;

class Command
{
    public:
        typedef std::tr1::shared_ptr<Command> ptr;
        Command() {}
        virtual bool apply(App &app) = 0;
};

class SaveCommand : public Command
{
    public:
        SaveCommand() : Command() {}
        virtual bool apply(App &app);
};

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
        void toggle_help() {}
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
        tempi::serializer::Serializer::ptr saver_; // FIXME
        bool saveGraph(); // FIXME
    private:
        ClutterActor *stage_;
        bool createGUI();
        bool setupGraph();
        // called by setupGraph() to create the clutter actors
        void drawGraph();
        ClutterActor* createNodeActor(tempi::Node &node);
        static gboolean on_idle(gpointer data);
        void pushCommand(Command::ptr command);
};

bool SaveCommand::apply(App &app)
{
    tempi::Logger::log(tempi::WARNING, "will save the graph.");
    tempi::ScopedLock::ptr lock = app.engine_->acquireLock();
    tempi::Graph::ptr graph = app.graph_;
    bool ok = app.saver_->save(*graph.get(), app.file_name_.c_str());
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
            tempi::Logger::log(DEBUG, os);
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
    return TRUE;
}

bool App::launch()
{
    if (verbose_)
        tempi::Logger::getInstance().setLevel(INFO);
    if (debug_)
        tempi::Logger::getInstance().setLevel(DEBUG);
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

ClutterActor* App::createNodeActor(tempi::Node &node)
{
    const std::string& node_type = node.getTypeName();
    std::string node_name = node.getName();

    std::ostringstream os;
    os << node_name << " (" << node_type << ")";

    ClutterLayoutManager *bin_layout = clutter_bin_layout_new(
        CLUTTER_BIN_ALIGNMENT_CENTER,
        CLUTTER_BIN_ALIGNMENT_CENTER);
    ClutterActor *bin_box = clutter_box_new(bin_layout);

    // FIXME: ClutterRectangle is deprecated!
    ClutterActor *background = clutter_rectangle_new_with_color(&GRAY_LIGHT);
    clutter_actor_set_size(background, 10.0f, 10.0f);
    clutter_bin_layout_add(CLUTTER_BIN_LAYOUT(bin_layout),
        background,
        CLUTTER_BIN_ALIGNMENT_FILL,
        CLUTTER_BIN_ALIGNMENT_FILL);

    ClutterLayoutManager *table_layout = clutter_table_layout_new();
    ClutterActor *table_box = clutter_box_new(table_layout);
    clutter_bin_layout_add(CLUTTER_BIN_LAYOUT(bin_layout),
        table_box,
        CLUTTER_BIN_ALIGNMENT_CENTER,
        CLUTTER_BIN_ALIGNMENT_START);
    guint SPACING = 2;
    clutter_table_layout_set_row_spacing(CLUTTER_TABLE_LAYOUT(table_layout), SPACING);
    clutter_table_layout_set_column_spacing(CLUTTER_TABLE_LAYOUT(table_layout), SPACING);
    
    ClutterActor *label = clutter_text_new_full(FONT_NAME, os.str().c_str(), &BLUE);
    clutter_table_layout_pack(CLUTTER_TABLE_LAYOUT(table_layout), label,
        0, 0); // col, row
    clutter_table_layout_set_alignment(CLUTTER_TABLE_LAYOUT(table_layout), label,
        CLUTTER_TABLE_ALIGNMENT_START,
        CLUTTER_TABLE_ALIGNMENT_START);
    clutter_table_layout_set_span(CLUTTER_TABLE_LAYOUT(table_layout), label,
        2, 1); // col span, row span

    gint inlet_row = 1;
    gint outlet_row = 1;
    const gint inlet_col = 0;
    const gint outlet_col = 1;
    // inlets:
    std::map<std::string, tempi::Inlet::ptr> inlets = node.getInlets();
    std::map<std::string, tempi::Inlet::ptr>::const_iterator iter_inlet;
    for (iter_inlet = inlets.begin(); iter_inlet != inlets.end(); ++iter_inlet)
    {
        std::string inlet_name =  (*iter_inlet).first;
        ClutterActor *inlet_label = clutter_text_new_full(FONT_NAME, inlet_name.c_str(), &RED);
        clutter_table_layout_pack(CLUTTER_TABLE_LAYOUT(table_layout), inlet_label,
            inlet_col,
            inlet_row); // col, row
        clutter_table_layout_set_alignment(CLUTTER_TABLE_LAYOUT(table_layout), inlet_label,
            CLUTTER_TABLE_ALIGNMENT_START, // x_align
            CLUTTER_TABLE_ALIGNMENT_START); // y_align
        inlet_row++;
    }
    // outlets:
    std::map<std::string, tempi::Outlet::ptr> outlets = node.getOutlets();
    std::map<std::string, tempi::Outlet::ptr>::const_iterator iter_outlet;
    for (iter_outlet = outlets.begin(); iter_outlet != outlets.end(); ++iter_outlet)
    {
        std::string outlet_name =  (*iter_outlet).first;
        ClutterActor *outlet_label = clutter_text_new_full(FONT_NAME, outlet_name.c_str(), &MAGENTA);
        clutter_table_layout_pack(CLUTTER_TABLE_LAYOUT(table_layout), outlet_label,
            outlet_col,
            outlet_row); // col, row
        clutter_table_layout_set_alignment(CLUTTER_TABLE_LAYOUT(table_layout), outlet_label,
            CLUTTER_TABLE_ALIGNMENT_END, // x_align
            CLUTTER_TABLE_ALIGNMENT_START); // y_align
        outlet_row++;
    }
    clutter_actor_set_name(bin_box, node_name.c_str());
    return bin_box;
}

void on_node_dragged(ClutterDragAction *action, ClutterActor *actor, gfloat event_x, gfloat event_y, ClutterModifierType modifiers, gpointer user_data)
{
    UNUSED(action);
    App *app = static_cast<App *>(user_data);

    tempi::ScopedLock::ptr lock = app->engine_->acquireLock();
    const gchar *name = clutter_actor_get_name(actor);
    tempi::Node::ptr node = app->graph_->getNode(name);
    node->setAttributeValue(tempi::Node::ATTRIBUTE_POSITION, tempi::Message("fff", event_x, event_y, 0.0f));
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
        g_signal_connect(action, "drag-end", G_CALLBACK(on_node_dragged), this);
        clutter_actor_set_reactive(actor, TRUE);
        clutter_container_add_actor(CLUTTER_CONTAINER(clutter_container_find_child_by_name(CLUTTER_CONTAINER(stage_), NODES_GROUP)), actor);
    }
}

bool App::saveGraph()
{
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
        tempi::Logger::log(DEBUG, os);
    }
    engine_.reset(new tempi::ThreadedScheduler);
    // TODO: make time precision configurable
    engine_->start(5); // time precision in ms
    tempi::ScopedLock::ptr lock = engine_->acquireLock();
    {
        std::ostringstream os;
        os << "miller: Create Graph\n";
        tempi::Logger::log(DEBUG, os);
    }
    engine_->createGraph(GRAPH_NAME);

    if (this->file_name_ == "")
    {
        tempi::Logger::log(tempi::WARNING, "No file name provided.");
        return true;
    }
    // Check for XML file
    if (! this->saver_->fileExists(this->file_name_.c_str()))
    {
        std::cerr << "miller: ERROR: File \"" << this->file_name_ << "\" not found!\n";
        return false;
    }
    else
    {
        std::ostringstream os;
        os << "miller: Found " << this->file_name_;
        tempi::Logger::log(DEBUG, os);
    }
    //if (verbose_)
    //    std::cout << (*engine_.get()) << std::endl;
    
    graph_ = engine_->getGraph(GRAPH_NAME);

    // load graph
    saver_->load(*graph_.get(), this->file_name_.c_str());
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
        tempi::Logger::log(DEBUG, os);
    }
    return true;
}

void on_frame_cb(ClutterTimeline * /*timeline*/, guint * /*ms*/, gpointer user_data)
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
    clutter_container_add_actor(CLUTTER_CONTAINER(stage_), group0);

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

