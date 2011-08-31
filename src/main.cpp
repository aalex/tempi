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

#include <clutter/clutter.h>
#include <iostream>
#include "tempi/tempi.h"
#include "tempi/timer.h"
#include "sampler.h"
#include <unistd.h>

struct App
{
    public:
        tempi::Sampler sampler_;
        bool recording_;
        ClutterActor *rectangle_;
};

static void on_frame_cb(ClutterTimeline * /*timeline*/, guint * /*ms*/, gpointer user_data)
{
    App *app = (App *) user_data;
    tempi::ff value = app->sampler_.readLoop();
    //std::cout << "Read " << value.get<0>() << ", " << value.get<1>() << std::endl;
    clutter_actor_set_position(app->rectangle_, value.get<0>(), value.get<1>());
}

static gboolean motion_event_cb(ClutterActor *stage, ClutterEvent *event, gpointer user_data)
{
    App *app = (App *) user_data;
    gfloat x, y;
    clutter_event_get_coords(event, &x, &y);

    if (app->recording_)
        app->sampler_.add(x, y);
    return TRUE;
}

static gboolean button_released_cb(ClutterActor *stage, ClutterEvent *event, gpointer user_data)
{
    App *app = (App *) user_data;
    gfloat x, y;
    clutter_event_get_coords(event, &x, &y);

    app->sampler_.add(x, y);
    app->recording_ = false;
    return TRUE;
}


static gboolean button_press_cb(ClutterActor *actor, ClutterEvent *event, gpointer user_data)
{
    App *app = (App *) user_data;
    gfloat x, y;
    clutter_event_get_coords(event, &x, &y);

    app->sampler_.reset();
    app->sampler_.add(x, y);
    return TRUE;
}

static void key_event_cb(ClutterActor *actor, ClutterKeyEvent *event, gpointer data)
{
    switch (event->keyval)
    {
        case CLUTTER_Escape:
            clutter_main_quit();
            break;
    }
}

int main(int argc, char *argv[])
{
    ClutterColor black = { 0x00, 0x00, 0x00, 0xff };
    ClutterColor red = { 0xff, 0x00, 0x00, 0xff };
    ClutterActor *stage;
    ClutterActor *rectangle;

    if (clutter_init(&argc, &argv) != CLUTTER_INIT_SUCCESS)
        return 1;

    stage = clutter_stage_get_default();
    clutter_actor_set_size(stage, 400, 400);
    clutter_stage_set_color(CLUTTER_STAGE(stage), &black);
    g_signal_connect(stage, "destroy", G_CALLBACK(clutter_main_quit), NULL);
    clutter_actor_set_reactive(stage, TRUE);

    rectangle = clutter_rectangle_new_with_color(&red);
    clutter_actor_set_size(rectangle, 50, 50);
    clutter_actor_set_position(rectangle, 0, 0);

    App app;
    app.recording_ = false;
    app.rectangle_ = rectangle;

    // timeline to attach a callback for each frame that is rendered
    ClutterTimeline *timeline;
    timeline = clutter_timeline_new(60); // ms
    clutter_timeline_set_loop(timeline, TRUE);
    clutter_timeline_start(timeline);

    g_signal_connect(timeline, "new-frame", G_CALLBACK(on_frame_cb), &app);
    g_signal_connect(stage, "key-press-event", G_CALLBACK(key_event_cb), &app);
    g_signal_connect(stage, "button-press-event", G_CALLBACK(button_press_cb), &app);
    g_signal_connect(stage, "button-release-event", G_CALLBACK(button_released_cb), &app);
    g_signal_connect(stage, "motion-event", G_CALLBACK(motion_event_cb), &app);

    clutter_container_add(CLUTTER_CONTAINER(stage), rectangle, NULL);
    clutter_actor_show(stage);

    clutter_main();
    return 0;
}

