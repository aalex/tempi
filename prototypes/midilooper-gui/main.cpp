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
 * @file Prototype of a GUI for midilooper.
 */

#include <clutter/clutter.h>
#include <mx/mx.h>
#include <iostream>

// Color constants:
static ClutterColor black = { 0x00, 0x00, 0x00, 0xff };
// static ClutterColor red = { 0xcc, 0x33, 0x33, 0xff };
// static ClutterColor green = { 0x33, 0xcc, 0x33, 0xff };
// static ClutterColor gray = { 0x99, 0x99, 0x99, 0xff };
// static ClutterColor white = { 0xcc, 0xcc, 0xcc, 0xff };

// static functions:
static void on_frame_cb(ClutterTimeline *timeline, guint *ms, gpointer user_data);
static void key_event_cb(ClutterActor *actor, ClutterKeyEvent *event, gpointer user_data);

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

/**
 * The App class manages the GUI
 */
class App
{
    public:
        App();
//        ~App();
    private:
        //ClutterActor *playback_button_;
        //ClutterActor *record_button_;
        ClutterActor *stage_;
};

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
    clutter_script_connect_signals(script, this);

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

static void on_button0_clicked(MxButton *actor, gpointer data)
{
    (void) actor;
    (void) data;
    std::cout << __FUNCTION__ << "" << std::endl;
}

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
    (void) on_button0_clicked;

    if (clutter_init(&argc, &argv) != CLUTTER_INIT_SUCCESS)
        return 1;
    App app;
    clutter_main();

    return 0;
}

