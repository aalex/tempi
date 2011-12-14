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
 * @file A MIDI printer.
 */

#include "tempi/config.h"
#include "tempi/message.h"
#include "tempi/scheduler.h"
#include "tempi/threadedscheduler.h"
#include "tempi/midi/midiinput.h"
#include "tempi/midi/midioutput.h"
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/program_options.hpp>
#include <clutter/clutter.h>
#include <glib.h>
#include <iostream>
#include <sstream>

// namespaces:
namespace po = boost::program_options;

// String constants:
static const char *PROGRAM_NAME = "midilooper";

// Color constants:
static ClutterColor black = { 0x00, 0x00, 0x00, 0xff };
static ClutterColor red = { 0xcc, 0x33, 0x33, 0xff };
static ClutterColor green = { 0x33, 0xcc, 0x33, 0xff };
static ClutterColor gray = { 0x99, 0x99, 0x99, 0xff };
static ClutterColor white = { 0xcc, 0xcc, 0xcc, 0xff };

// Static functions:
static gboolean record_button_press_cb(ClutterActor *actor, ClutterEvent *event, gpointer user_data);
static gboolean playback_button_press_cb(ClutterActor *actor, ClutterEvent *event, gpointer user_data);
static void key_event_cb(ClutterActor *actor, ClutterKeyEvent *event, gpointer user_data);
static void on_frame_cb(ClutterTimeline *timeline, guint *ms, gpointer user_data);
static void list_input_midi_devices();
static void list_output_midi_devices();

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
        //void playFaster() {} // Not implemented
        //void playSlower() {} // Not implemented
        bool isRecording();
        bool isPlaying();
        /**
         * Toggles playback and returns its new state.
         */
        bool togglePlayback();
        /**
         * Toggles recording and returns its new state.
         */
        bool toggleRecord();
        //void clearScore();
    protected:
        bool setupGraph();
        bool createGUI();
    private:
        bool verbose_;
        bool recording_;
        bool playing_;
        unsigned int midi_input_port_;
        unsigned int midi_output_port_;
        bool graph_ok_;
        tempi::ThreadedScheduler::ptr engine_;
        ClutterActor *playback_button_;
        ClutterActor *record_button_;
        ClutterActor *stage_;
};

App::App() :
    verbose_(false),
    recording_(false),
    playing_(false),
    midi_input_port_(0),
    midi_output_port_(0),
    graph_ok_(false)
{
    stage_ = NULL;
}

App::~App()
{
    if (engine_.get() != 0)
    {
        if (verbose_)
            std::cout << "Waiting for Scheduler's thread to join." << std::endl;
        engine_->stop();
    }
}

bool App::isRecording()
{
    return recording_;
}

bool App::isPlaying()
{
    return playing_;
}

static gboolean record_button_press_cb(ClutterActor *actor, ClutterEvent *event, gpointer user_data)
{
    App *app = (App *) user_data;
    app->toggleRecord();
    return TRUE;
}

static gboolean playback_button_press_cb(ClutterActor *actor, ClutterEvent *event, gpointer user_data)
{
    App *app = (App *) user_data;
    app->togglePlayback();
    return TRUE;
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
        case CLUTTER_KEY_BackSpace:
            //app->clearScore();
            break;
        case CLUTTER_KEY_Up:
            //app->playFaster();
            break;
        case CLUTTER_KEY_Down:
            //app->playSlower();
            break;
        case CLUTTER_KEY_space:
            app->toggleRecord();
            break;
    }
}

bool App::togglePlayback()
{
    bool state = isPlaying();
    //std::cout << "TODO: " << __FUNCTION__ << std::endl;
    playing_ = ! state;

    if (isPlaying())
        clutter_rectangle_set_color(CLUTTER_RECTANGLE(playback_button_), &green);
    else
        clutter_rectangle_set_color(CLUTTER_RECTANGLE(playback_button_), &gray);
    
    tempi::ScopedLock::ptr lock = engine_->acquireLock();
    engine_->getGraph("graph0")->setNodeProperty("sampler.sampler0", "playing", tempi::Message("b", isPlaying()));
    return isPlaying();
}

bool App::toggleRecord()
{
    bool state = isRecording();
    //std::cout << "TODO: " << __FUNCTION__ << std::endl;
    recording_ = ! state;

    if (isRecording())
    {
        if (isPlaying())
            togglePlayback();
        clutter_rectangle_set_color(CLUTTER_RECTANGLE(record_button_), &red);
    }
    else
    {
        if (! isPlaying())
            togglePlayback();
        clutter_rectangle_set_color(CLUTTER_RECTANGLE(record_button_), &gray);
    }

    tempi::ScopedLock::ptr lock = engine_->acquireLock();
    engine_->getGraph("graph0")->setNodeProperty("sampler.sampler0", "recording", tempi::Message("b", isRecording()));
    return isRecording();
}

// void App::clearScore()
// {
//     std::cout << "TODO: " << __FUNCTION__ << std::endl;
// }

bool App::poll()
{
    if (graph_ok_)
    {
        //engine_->tick();
        return true;
    }
    else
    {
        std::cerr << "App::" << __FUNCTION__ << "(): Error: must call launch() first.\n";
        return false;
    }
}

bool App::setupGraph()
{
    using tempi::Message;
    if (graph_ok_)
    {
        std::cerr << "App::" << __FUNCTION__ << ": already called.\n";
        return false;
    }
    engine_.reset(new tempi::ThreadedScheduler);
    engine_->start(5); // time precision in ms
    if (verbose_)
        std::cout << (*engine_.get()) << std::endl;
    tempi::ScopedLock::ptr lock = engine_->acquireLock();
    engine_->createGraph("graph0");
    tempi::Graph::ptr graph = engine_->getGraph("graph0");
    // Create objects:
    graph->addNode("midi.receive", "midi.recv0");
    graph->addNode("sampler.sampler", "sampler.sampler0");
    graph->addNode("midi.send", "midi.send0");
    graph->addNode("base.print", "base.print0");
    graph->addNode("base.print", "base.print1");
    // Connections:
    //graph->connect("midi.recv0", 0, "midi.send0", 0);
    graph->connect("midi.recv0", 0, "base.print0", 0);
    graph->connect("midi.recv0", 0, "sampler.sampler0", 0);
    graph->connect("sampler.sampler0", 0, "midi.send0", 0);
    graph->connect("sampler.sampler0", 0, "base.print1", 0);
    //TODO graph->connect("sampler.sampler0", 0, "base.prepend0", 0);
    // Set node properties:
    graph->setNodeProperty("midi.recv0", "port", tempi::Message("i", midi_input_port_));
    graph->setNodeProperty("midi.send0", "port", tempi::Message("i", midi_output_port_));
    graph->setNodeProperty("base.print0", "prefix", tempi::Message("s", "input: "));
    graph->setNodeProperty("base.print1", "prefix", tempi::Message("s", "playback: "));
    if (! verbose_)
    {
        graph->setNodeProperty("base.print0", "enabled", tempi::Message("b", false));
        graph->setNodeProperty("base.print1", "enabled", tempi::Message("b", false));
    }

    graph_ok_ = true;
    if (verbose_)
    {
        //engine_->sleepThisThread(6.0f);
        std::cout << (*engine_.get()) << std::endl;
    }
}

bool App::launch()
{
    if (graph_ok_)
    {
        std::cerr << "App::" << __FUNCTION__ << "(): Already called\n";
        return false;
    }
    else
    {
        setupGraph();
        createGUI();
        if (verbose_)
            std::cout << "Running... Press ctrl-C in the terminal to quit. (or ctrl-Q in the GUI)" << std::endl;
        return true;
    }
}

static void on_frame_cb(ClutterTimeline * /*timeline*/, guint * /*ms*/, gpointer user_data)
{
    App *context = static_cast<App*>(user_data);
    context->poll();
}

bool App::createGUI()
{
    if (stage_ != 0)
    {
        std::cerr << "App::" << __FUNCTION__ << ": Stage already created.\n"; 
        return false;
    }
    else
        std::cout << "Creating GUI.\n";
    stage_ = clutter_stage_get_default();
    clutter_actor_set_size(stage_, 300, 200);
    clutter_stage_set_color(CLUTTER_STAGE(stage_), &black);
    g_signal_connect(stage_, "destroy", G_CALLBACK(clutter_main_quit), NULL);
    
    // Record button:
    record_button_ = clutter_rectangle_new_with_color(&gray);
    clutter_actor_set_size(record_button_, 50.0f, 50.0f);
    clutter_actor_set_anchor_point_from_gravity(record_button_, CLUTTER_GRAVITY_CENTER);
    clutter_actor_set_position(record_button_, 100.0f, 100.0f);
    clutter_container_add_actor(CLUTTER_CONTAINER(stage_), record_button_);
    clutter_actor_set_reactive(record_button_, TRUE);
    g_signal_connect(record_button_, "button-press-event", G_CALLBACK(record_button_press_cb), this);
    // TODO g_signal_connect(stage_, "button-release-event", G_CALLBACK(record_button_released_cb), this);

    ClutterActor *record_label = clutter_text_new_full("Sans semibold 12px", "Record", &white);
    clutter_actor_set_position(record_label, 100.0f, 150.0f);
    clutter_actor_set_anchor_point_from_gravity(record_label, CLUTTER_GRAVITY_CENTER);
    clutter_container_add_actor(CLUTTER_CONTAINER(stage_), record_label);

    // Playback button:
    playback_button_ = clutter_rectangle_new_with_color(&gray);
    clutter_actor_set_size(playback_button_, 50.0f, 50.0f);
    clutter_actor_set_anchor_point_from_gravity(playback_button_, CLUTTER_GRAVITY_CENTER);
    clutter_actor_set_position(playback_button_, 200.0f, 100.0f);
    clutter_container_add_actor(CLUTTER_CONTAINER(stage_), playback_button_);
    clutter_actor_set_reactive(playback_button_, TRUE);
    g_signal_connect(playback_button_, "button-press-event", G_CALLBACK(playback_button_press_cb), this);
    // TODO g_signal_connect(stage_, "button-release-event", G_CALLBACK(playback_button_released_cb), this);

    ClutterActor *playback_label = clutter_text_new_full("Sans semibold 12px", "Playback", &white);
    clutter_actor_set_position(playback_label, 200.0f, 150.0f);
    clutter_actor_set_anchor_point_from_gravity(playback_label, CLUTTER_GRAVITY_CENTER);
    clutter_container_add_actor(CLUTTER_CONTAINER(stage_), playback_label);

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

static void list_input_midi_devices()
{
    std::cout << "MIDI inputs you can listen to:" << std::endl;
    tempi::midi::MidiInput input;
    input.enumerateDevices();
}

static void list_output_midi_devices()
{
    std::cout << "MIDI outputs you can send to:" << std::endl;
    tempi::midi::MidiOutput output;
    output.enumerateDevices();
}

int App::parse_options(int argc, char **argv)
{
    po::options_description desc("Options");
    desc.add_options()
        ("help,h", "Show this help message and exit")
        ("version", "Show program's version number and exit")
        ("list-inputs,l", "Lists the input MIDI ports we can listen to")
        ("list-outputs,L", "Lists the output MIDI ports we can send to")
        ("input,i", po::value<unsigned int>()->default_value(0), "Sets the MIDI input port to listen to")
        ("output,o", po::value<unsigned int>()->default_value(0), "Sets the MIDI output port to send to")
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
    midi_input_port_ = options["input"].as<unsigned int>();
    midi_output_port_ = options["output"].as<unsigned int>();
    if (verbose_)
        std::cout << "MIDI input port: " << midi_input_port_ << std::endl;
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
    if (options.count("list-inputs"))
    {
        list_input_midi_devices();
        return 0;
    }
    if (options.count("list-outputs"))
    {
        list_output_midi_devices();
        return 0;
    }
    return -1;
}

int main(int argc, char *argv[])
{
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
    app.togglePlayback(); // playback is on at startup
    clutter_main();
    return 0;
}

