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

#include "miller-macros.h"
#include "miller-command.h"
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
static const char * const FONT_NAME = "Monospace Bold 12px";

// Static functions:
static void key_event_cb(ClutterActor *actor, ClutterKeyEvent *event, gpointer user_data);
static void on_frame_cb(ClutterTimeline *timeline, guint *ms, gpointer user_data);
static void on_fullscreen(ClutterStage* stage, gpointer user_data);
static void on_unfullscreen(ClutterStage* stage, gpointer user_data);
static ClutterActor* createNodeActor(tempi::Node &node);
static std::string makePadName(const char *node_name, const char *pad_name, bool is_inlet);
static std::string makeNodeName(const char *node_name);
static std::string makeConnectionName(const char *from_node, const char *outlet, const char *to_node, const char *inlet);
static ClutterActor* createConnectionsActor(tempi::Graph &graph);
static void updateAllConnectionsGeometry(ClutterActor *connectionsGroup, ClutterActor *nodesGroup, tempi::Graph &graph);

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
        static gboolean on_idle(gpointer data);
        void pushCommand(Command::ptr command);
        static gboolean on_group0_scrolled(ClutterActor *actor, ClutterEvent *event, gpointer user_data);
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

std::string makePadName(const char *node_name, const char *pad_name, bool is_inlet)
{
    std::ostringstream os;
    os << node_name;
    if (is_inlet)
        os << ":inlet:";
    else
        os << ":outlet:";
    os << pad_name;
    return os.str();
}

std::string makeNodeName(const char *node_name)
{
    // just to make sure we are consistent
    return std::string(node_name);
}

ClutterActor* createPadActor(const char *node_name, const char *pad_name, bool is_inlet)
{
    ClutterLayoutManager *layout = clutter_bin_layout_new(
        CLUTTER_BIN_ALIGNMENT_CENTER,
        CLUTTER_BIN_ALIGNMENT_CENTER);
    ClutterActor *background = clutter_rectangle_new_with_color(&WHITE);
    clutter_actor_set_size(background, 1.0f, 1.0f);
    ClutterActor *box = clutter_box_new(layout);
    clutter_bin_layout_add(CLUTTER_BIN_LAYOUT(layout),
        background,
        CLUTTER_BIN_ALIGNMENT_FILL,
        CLUTTER_BIN_ALIGNMENT_FILL);
    ClutterActor *label = clutter_text_new_full(FONT_NAME, pad_name, &RED);
    clutter_bin_layout_add(CLUTTER_BIN_LAYOUT(layout),
        label,
        CLUTTER_BIN_ALIGNMENT_CENTER,
        is_inlet ? CLUTTER_BIN_ALIGNMENT_START : CLUTTER_BIN_ALIGNMENT_END);
    clutter_actor_set_name(box, makePadName(node_name, pad_name, is_inlet).c_str());
    return box;
}

ClutterActor* createNodeActor(tempi::Node &node)
{
    const std::string& node_type = node.getTypeName();
    std::string node_name = node.getName();

    std::ostringstream os;
    os << node_name << " (" << node_type << ")";

    ClutterLayoutManager *node_layout = clutter_bin_layout_new(
        CLUTTER_BIN_ALIGNMENT_CENTER,
        CLUTTER_BIN_ALIGNMENT_CENTER);
    ClutterActor *node_box = clutter_box_new(node_layout);

    // FIXME: ClutterRectangle is deprecated!
    ClutterActor *background = clutter_rectangle_new_with_color(&GRAY_LIGHT);
    clutter_actor_set_size(background, 1.0f, 1.0f);
    clutter_bin_layout_add(CLUTTER_BIN_LAYOUT(node_layout),
        background,
        CLUTTER_BIN_ALIGNMENT_FILL,
        CLUTTER_BIN_ALIGNMENT_FILL);

    ClutterLayoutManager *table_layout = clutter_table_layout_new();
    ClutterActor *table_box = clutter_box_new(table_layout);
    clutter_bin_layout_add(CLUTTER_BIN_LAYOUT(node_layout),
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
        std::string pad_name = (*iter_inlet).first;
        ClutterActor *pad_actor = createPadActor(node_name.c_str(), pad_name.c_str(), true); // is an inlet
        clutter_table_layout_pack(CLUTTER_TABLE_LAYOUT(table_layout), pad_actor,
            inlet_col,
            inlet_row); // col, row
        clutter_table_layout_set_alignment(CLUTTER_TABLE_LAYOUT(table_layout), pad_actor,
            CLUTTER_TABLE_ALIGNMENT_START, // x_align
            CLUTTER_TABLE_ALIGNMENT_START); // y_align
        inlet_row++;
    }
    // outlets:
    std::map<std::string, tempi::Outlet::ptr> outlets = node.getOutlets();
    std::map<std::string, tempi::Outlet::ptr>::const_iterator iter_outlet;
    for (iter_outlet = outlets.begin(); iter_outlet != outlets.end(); ++iter_outlet)
    {
        std::string pad_name =  (*iter_outlet).first;
        ClutterActor *pad_actor = createPadActor(node_name.c_str(), pad_name.c_str(), false); // is not an inlet
        clutter_table_layout_pack(CLUTTER_TABLE_LAYOUT(table_layout), pad_actor,
            outlet_col,
            outlet_row); // col, row
        clutter_table_layout_set_alignment(CLUTTER_TABLE_LAYOUT(table_layout), pad_actor,
            CLUTTER_TABLE_ALIGNMENT_END, // x_align
            CLUTTER_TABLE_ALIGNMENT_START); // y_align
        outlet_row++;
    }
    clutter_actor_set_name(node_box, makeNodeName(node_name.c_str()).c_str());
    return node_box;
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

    ClutterActor *connections_actor = createConnectionsActor(*(graph_.get()));

    updateAllConnectionsGeometry(connections_actor, clutter_container_find_child_by_name(CLUTTER_CONTAINER(stage_), NODES_GROUP), *graph_.get());
    clutter_actor_set_name(connections_actor, CONNECTIONS_ACTOR);
    clutter_container_add_actor(CLUTTER_CONTAINER(stage_), connections_actor);
}

std::string makeConnectionName(const char *from_node, const char *outlet, const char *to_node, const char *inlet)
{
    std::ostringstream os;
    os << from_node << "." << outlet << "->" << to_node << inlet;
    return os.str();
}

void on_paint_connection(ClutterActor *actor, gpointer user_data)
{
    ClutterGeometry geom;
    clutter_actor_get_allocation_geometry(actor, &geom);
    ClutterColor color;
    guint8 tmp_alpha = clutter_actor_get_paint_opacity(actor) * color.alpha / 255;
    clutter_rectangle_get_color(CLUTTER_RECTANGLE(actor), &color);
    cogl_set_source_color4ub(color.red, color.green, color.blue, tmp_alpha);
    cogl_path_line(geom.x, geom.y, geom.width, geom.height);
}

// a single connection
ClutterActor* createConnectionActor(const char *source_node, const char *outlet, const char *sink_node, const char *inlet)
{
    // assumes that the node actors are already created
    std::string outlet_name = makePadName(source_node, outlet, false);
    std::string inlet_name = makePadName(sink_node, inlet, true);
    std::string connection_name = makeConnectionName(source_node, outlet, sink_node, inlet);

    ClutterActor *actor = clutter_rectangle_new_with_color(&MAGENTA);
    g_signal_connect(actor, "paint", G_CALLBACK(on_paint_connection), NULL);
    clutter_actor_set_name(actor, connection_name.c_str());
    return actor;
}

void updateConnectionGeometry(ClutterActor *connectionsGroup, ClutterActor *nodesGroup, const char *source_node, const char *outlet, const char *sink_node, const char *inlet)
{
    std::string outlet_name = makePadName(source_node, outlet, false);
    std::string inlet_name = makePadName(sink_node, inlet, true);
    std::string connection_name = makeConnectionName(source_node, outlet, sink_node, inlet);
    ClutterActor *outlet_actor = clutter_container_find_child_by_name(
        CLUTTER_CONTAINER(nodesGroup), outlet_name.c_str());
    ClutterActor *inlet_actor = clutter_container_find_child_by_name(
        CLUTTER_CONTAINER(nodesGroup), inlet_name.c_str());
    ClutterActor *connection_actor = clutter_container_find_child_by_name(
        CLUTTER_CONTAINER(connectionsGroup), connection_name.c_str());

    gfloat x = 0.0f;
    gfloat y = 0.0f;
    gfloat w = 0.0f;
    gfloat h = 0.0f;
    clutter_actor_get_position(outlet_actor, &x, &y);
    clutter_actor_get_position(inlet_actor, &w, &h);
    w = w - x;
    h = h - y;

    clutter_actor_set_position(connection_actor, x, y);
    clutter_actor_set_size(connection_actor, w, h);
}

void updateAllConnectionsGeometry(ClutterActor *connectionsGroup, ClutterActor *nodesGroup, tempi::Graph &graph)
{
    std::vector<tempi::Graph::Connection> connections = graph.getAllConnections();
    std::vector<tempi::Graph::Connection>::const_iterator iter;

    for (iter = connections.begin(); iter != connections.end(); ++iter)
    {
        std::string source_node = (*iter).get<0>();
        std::string outlet = (*iter).get<1>();
        std::string sink_node = (*iter).get<2>();
        std::string inlet = (*iter).get<3>();
        updateConnectionGeometry(connectionsGroup, nodesGroup, source_node.c_str(), outlet.c_str(), sink_node.c_str(), inlet.c_str());
    }
}

// all the connections
ClutterActor* createConnectionsActor(tempi::Graph &graph)
{
    ClutterActor *group = clutter_group_new();
    // connections are a tuple of string, string, string, string
    // Tuple containing the name of the source node, outlet name, name of sink node, inlet name.
    std::vector<tempi::Graph::Connection> connections = graph.getAllConnections();
    std::vector<tempi::Graph::Connection>::const_iterator iter;

    for (iter = connections.begin(); iter != connections.end(); ++iter)
    {
        std::string source_node = (*iter).get<0>();
        std::string outlet = (*iter).get<1>();
        std::string sink_node = (*iter).get<2>();
        std::string inlet = (*iter).get<3>();

        ClutterActor * connection_actor = createConnectionActor(
            source_node.c_str(), outlet.c_str(), sink_node.c_str(), inlet.c_str());
        clutter_container_add_actor(CLUTTER_CONTAINER(group), connection_actor);
    }
    return group;
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
    if (! this->saver_->fileExists(this->file_name_.c_str()))
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
    clutter_actor_set_reactive(group0, TRUE);
    g_signal_connect(group0, "scroll-event", G_CALLBACK(App::on_group0_scrolled), this);
    clutter_container_add_actor(CLUTTER_CONTAINER(stage_), group0);

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

