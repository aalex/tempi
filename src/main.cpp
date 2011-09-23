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

#include "particlegenerator.h"
#include "tempi/tempi.h"
#include "tempi/pingpongplayback.h"
#include <clutter/clutter.h>
#include <glib.h>
#include <iostream>
#include <tr1/memory>
#include <unistd.h>

static const unsigned int NUM_SAMPLER = 3;
static const bool VERBOSE = false;

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
#endif

struct Sampler
{
    public:
        Sampler();
        tempi::Track track_;
        std::tr1::shared_ptr<tempi::Recorder> recorder_;
        std::tr1::shared_ptr<tempi::Player> player_;
        bool recording_;
        ParticleGenerator generator_;
};

Sampler::Sampler()
{
    recorder_.reset(new tempi::Recorder(&track_));
    player_.reset(new tempi::Player(&track_));
    player_.get()->setPlaybackMode(new tempi::PingPongPlayback());
    recording_ = false;
}

struct App
{
    public:
        App();
        void startRecording();
        void stopRecording();
        void toggleFullscreen();
        void write(float x, float y);
        bool isRecording();
        void onDraw();
        unsigned int current_;
        std::vector<std::tr1::shared_ptr<Sampler> > samplers_;
        ClutterActor *stage_;
        bool fullscreen_;
};

App::App() : 
    current_(0)
{
    for (unsigned int i = 0; i < NUM_SAMPLER; ++i)
    {
        samplers_.push_back(std::tr1::shared_ptr<Sampler>(new Sampler()));
    }
}

void App::startRecording()
{
    if (samplers_.size() != 0)
    {
        Sampler *sampler = samplers_[current_].get();
        sampler->recording_ = true;
        sampler->track_.reset(); // clears the track
        sampler->recorder_.get()->reset();
    }
}

void App::stopRecording()
{
    if (samplers_.size() != 0)
    {
        samplers_[current_].get()->recording_ = false;
        current_ = (current_ + 1) % samplers_.size();
    }
}
void App::write(float x, float y)
{
    if (samplers_.size() != 0)
    {
        Sampler *sampler = samplers_[current_].get();
        tempi::Message m;
        m.appendFloat(x);
        m.appendFloat(y);
        if (VERBOSE)
            std::cout << "write " << x << " " << y << std::endl;
        sampler->recorder_.get()->add(m);
    }
}
bool App::isRecording()
{
    return samplers_[current_].get()->recording_;
}

void App::toggleFullscreen()
{
    fullscreen_ = ! fullscreen_;
    if (fullscreen_)
        clutter_stage_set_fullscreen(CLUTTER_STAGE(stage_), TRUE);
    else
        clutter_stage_set_fullscreen(CLUTTER_STAGE(stage_), FALSE);
}

void App::onDraw()
{
    std::vector<std::tr1::shared_ptr<Sampler> >::iterator iter;
    for (iter = samplers_.begin(); iter < samplers_.end(); ++iter)
    {
        Sampler *sampler = (*iter).get();

        tempi::Message *m = sampler->player_.get()->read();
        if (m)
        {
            if (m->typesMatch("ff"))
            {
                float x  = m->getFloat(0);
                float y = m->getFloat(1);
                sampler->generator_.setSourcePosition(x, y);
            }
            else
                std::cout << "types don't match: " << m->getTypes() << std::endl;
        }
        sampler->generator_.onDraw();
    }
}

static void on_frame_cb(ClutterTimeline * /*timeline*/, guint * /*ms*/, gpointer user_data)
{
    App *app = (App *) user_data;
    app->onDraw();
}

static gboolean motion_event_cb(ClutterActor *stage, ClutterEvent *event, gpointer user_data)
{
    App *app = (App *) user_data;
    gfloat x, y;
    clutter_event_get_coords(event, &x, &y);

    if (app->isRecording())
        app->write(x, y);
    return TRUE;
}

static gboolean button_released_cb(ClutterActor *stage, ClutterEvent *event, gpointer user_data)
{
    App *app = (App *) user_data;
    gfloat x, y;
    clutter_event_get_coords(event, &x, &y);

    app->write(x, y);
    app->stopRecording();
    return TRUE;
}

static gboolean button_press_cb(ClutterActor *actor, ClutterEvent *event, gpointer user_data)
{
    App *app = (App *) user_data;
    gfloat x, y;
    clutter_event_get_coords(event, &x, &y);
    app->startRecording();
    app->write(x, y);
    return TRUE;
}

static void key_event_cb(ClutterActor *actor, ClutterKeyEvent *event, gpointer user_data)
{
    App *app = (App *) user_data;
    switch (event->keyval)
    {
        case CLUTTER_Escape:
            app->toggleFullscreen();
            break;
        case CLUTTER_KEY_q:
            clutter_main_quit();
            break;
        case CLUTTER_KEY_space:
            //app->track_.print();
            break;
        case CLUTTER_KEY_Up:
            //app->player_.get()->setSpeed(app->player_.get()->getSpeed() * 1.1);
            break;
        case CLUTTER_KEY_Down:
            //app->player_.get()->setSpeed(app->player_.get()->getSpeed() / 1.1);
            break;
    }
}

int main(int argc, char *argv[])
{
    ClutterColor black = { 0x00, 0x00, 0x00, 0xff };
    ClutterColor red = { 0xff, 0x00, 0x00, 0xff };
    ClutterActor *stage;

    if (clutter_init(&argc, &argv) != CLUTTER_INIT_SUCCESS)
        return 1;
    stage = clutter_stage_get_default();
    App app;
    app.fullscreen_ = false;;
    app.stage_ = stage;
    clutter_actor_set_size(stage, 1024, 768);
    clutter_stage_set_color(CLUTTER_STAGE(stage), &black);
    g_signal_connect(stage, "destroy", G_CALLBACK(clutter_main_quit), NULL);
    clutter_actor_set_reactive(stage, TRUE);

    std::vector<std::tr1::shared_ptr<Sampler> >::iterator iter;
    for (iter = app.samplers_.begin(); iter < app.samplers_.end(); ++iter)
    {
        Sampler *sampler = (*iter).get();
        clutter_container_add_actor(CLUTTER_CONTAINER(stage), sampler->generator_.getRoot());
    }

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

    clutter_actor_show(stage);

    clutter_main();
    return 0;
}

