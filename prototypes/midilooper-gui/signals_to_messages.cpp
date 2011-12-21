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
 * @file Prototype of Clutter signals to Tempi messages converter
 */

#include <clutter/clutter.h>
#include "tempi/message.h"
#include <mx/mx.h>
#include <iostream>
#include <tr1/memory>
#include <string>
#include <string.h> // for strlen
#include <vector>

// Color constants:
static ClutterColor black = { 0x00, 0x00, 0x00, 0xff };

// static functions:
static void on_frame_cb(ClutterTimeline *timeline, guint *ms, gpointer user_data);
static void key_event_cb(ClutterActor *actor, ClutterKeyEvent *event, gpointer user_data);
static gboolean tempi_clutter_event_cb(ClutterActor *actor, gpointer user_data);
static void tempi_clutter_connect_signals(
    ClutterScript *script,
    GObject *object,
    const gchar *signal_name,
    const gchar *handler_name,
    GObject *connect_object,
    GConnectFlags flags,
    gpointer user_data);

#define USE_FUNCTION(x) (void) (x)

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

class App;

struct TempiClutterSignalConnection
{
    typedef std::tr1::shared_ptr<TempiClutterSignalConnection> ptr;
    App *app_;
    std::string signal_name_;
    std::string handler_name_;
    std::string actor_name_;
};

/**
 * The App class manages the GUI
 */
class App
{
    public:
        App();
        std::vector<TempiClutterSignalConnection::ptr> connections_;
//        ~App();
    private:
        //ClutterActor *playback_button_;
        //ClutterActor *record_button_;
        ClutterActor *stage_;
};

static gboolean tempi_clutter_event_cb(ClutterActor *actor, gpointer user_data)
{
    g_return_val_if_fail(CLUTTER_IS_ACTOR(actor), FALSE);
    TempiClutterSignalConnection *conn = static_cast<TempiClutterSignalConnection *>(user_data);
    g_message("%s: actor %s triggered event [%s][%s]",
        __FUNCTION__,
        clutter_actor_get_name(actor),
        conn->signal_name_.c_str(), 
        conn->handler_name_.c_str());
    // TODO: use app pointer
    // TODO: turn it into a tempi::Message
    return TRUE; // handled
}

/**
 * Custom handler for the signals in the ClutterScript.
 * Slots are turned into tempi::Message instances
 */
static void tempi_clutter_connect_signals(
    ClutterScript *script,
    GObject *object,
    const gchar *signal_name,
    const gchar *handler_name,
    GObject *connect_object,
    GConnectFlags flags,
    gpointer user_data)
{
    TempiClutterSignalConnection::ptr conn;
    std::cout << __FUNCTION__ << ": sig:" << signal_name << ", handler:" << handler_name << std::endl;

    // store info about the signal we are connecting to:
    std::cout << "set signal name to " << signal_name << " length:" << strlen(signal_name) << std::endl;
    conn->signal_name_.assign(signal_name, strlen(signal_name));

    std::cout << "set handler name to " << handler_name << " length:" << strlen(handler_name) << std::endl;
    conn->handler_name_.assign(handler_name, strlen(handler_name));

    if (CLUTTER_IS_ACTOR(object))
    {
        std::cout << "set actor name to " << clutter_actor_get_name(CLUTTER_ACTOR(object)) << std::endl;
        const char *actor_name = clutter_actor_get_name(CLUTTER_ACTOR(object));
        if (actor_name != NULL)
            conn->actor_name_.assign(actor_name, strlen(actor_name));
    }
    else
        std::cout << "Not an ClutterActor." << std::endl;
    // Store the connection object in the app:
    App *app = static_cast<App *>(user_data);
    conn->app_ = app;
    app->connections_.push_back(conn);

    // connect the signal to our generic callback:
    if (! g_signal_connect(object, signal_name, G_CALLBACK(tempi_clutter_event_cb), conn.get()))
    {
        g_error("%s: Couldn't connect to signal %s", __FUNCTION__, signal_name);
    }
}

App::App()
{
    stage_ = clutter_stage_get_default();
    clutter_actor_set_size(stage_, 640, 480);
    clutter_stage_set_color(CLUTTER_STAGE(stage_), &black);
    g_signal_connect(stage_, "destroy", G_CALLBACK(clutter_main_quit), NULL);

    std::string filename("gui.json");

    GError *error = NULL;
    /* load JSON from a file */
    ClutterScript *script = clutter_script_new();
    clutter_script_load_from_file(script, filename.c_str(), &error);

    if (error != NULL)
    {
        std::cerr << "Unable to read file: " <<  error->message << std::endl;
        g_error_free(error);
        // TODO: fail
        g_error("Could not load GUI");
    }
    ClutterActor *group0 = CLUTTER_ACTOR(clutter_script_get_object(script, "group0"));
    clutter_container_add_actor(CLUTTER_CONTAINER(stage_), group0);
    //clutter_script_connect_signals(script, this);
    clutter_script_connect_signals_full(script, tempi_clutter_connect_signals, this);

    // timeline to attach a callback for each frame that is rendered
    ClutterTimeline *timeline;
    timeline = clutter_timeline_new(60); // ms
    clutter_timeline_set_loop(timeline, TRUE);
    clutter_timeline_start(timeline);
    g_signal_connect(timeline, "new-frame", G_CALLBACK(on_frame_cb), this);
    g_signal_connect(stage_, "key-press-event", G_CALLBACK(key_event_cb), this);
    g_object_unref(script); // avoid memory leak

    clutter_actor_show(stage_);
}

// void on_button0_clicked(MxButton *actor, gpointer data)
// {
//     (void) actor;
//     (void) data;
//     std::cout << __FUNCTION__ << "" << std::endl;
// }

static void on_frame_cb(ClutterTimeline * /*timeline*/, guint * /*ms*/, gpointer user_data)
{
    App *context = static_cast<App*>(user_data);
    (void) context; // TODO remove this
    //context->poll();
}

static void key_event_cb(ClutterActor *actor, ClutterKeyEvent *event, gpointer user_data)
{
    App *app = (App *) user_data;
    (void) app; // TODO: remove this
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

int main(int argc, char *argv[])
{
    // avoid unused warnings
    //(void) on_button0_clicked;

    if (clutter_init(&argc, &argv) != CLUTTER_INIT_SUCCESS)
        return 1;
    App app;
    clutter_main();

    return 0;
}

