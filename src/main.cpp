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

#include "legacy.h"
#include "particlegenerator.h"
#include "sampler.h"
#include "tempi/config.h"
#include "tempi/oscreceiver.h"
#include "tempi/tempi.h"
#include <boost/lexical_cast.hpp>
#include <boost/program_options.hpp>
#include <clutter/clutter.h>
#include <glib.h>
#include <iostream>
#include <tr1/memory>
#include <unistd.h>

namespace po = boost::program_options;

class App
{
    public:
        App();
        void startRecording();
        void stopRecording();
        void toggleFullscreen();
        void setFullscreen(bool enabled);
        void write(float x, float y);
        bool isRecording();
        bool launch(int argc, char **argv);
        bool parse_options(int argc, char **argv);
        void onDraw();
        void playSlower();
        void playFaster();
        void setSpeed(float factor);
        void clearAll();
    private:
        unsigned int current_;
        unsigned int osc_recv_port_;
        bool fullscreen_;
        bool recording_;
        bool verbose_;
        float speed_;
        ClutterActor *stage_;
        std::vector<std::tr1::shared_ptr<Sampler> > samplers_;
        std::tr1::shared_ptr<tempi::OscReceiver> osc_receiver_;
        void pollOSC();
        bool startOSC();
        void drawSamplers();
        bool handleOscMessage(const tempi::Message &message);
        unsigned int addSampler();
        Sampler *getCurrentlyRecordingSampler();
        void updateSpeed();
};

App::App() : 
    current_(0),
    osc_recv_port_(0),
    fullscreen_(false),
    recording_(false),
    verbose_(false),
    speed_(1.0f)
{
}

unsigned int App::addSampler()
{
    samplers_.push_back(std::tr1::shared_ptr<Sampler>(new Sampler()));

    unsigned int index = samplers_.size() - 1;
    Sampler *sampler = samplers_[index].get();
    clutter_container_add_actor(CLUTTER_CONTAINER(stage_), sampler->getGenerator()->getRoot());
    return samplers_.size() - 1;
}

void App::clearAll()
{
    std::vector<std::tr1::shared_ptr<Sampler> >::iterator iter;
    for (iter = samplers_.begin(); iter < samplers_.end(); ++iter)
    {
        Sampler *sampler = (*iter).get();
        // deletes the ClutterActors
        clutter_container_remove_actor(CLUTTER_CONTAINER(stage_), sampler->getGenerator()->getRoot());
    }
    samplers_.clear();
}

Sampler *App::getCurrentlyRecordingSampler()
{
    if (samplers_.size() == 0)
        return 0;
    else
        return samplers_[samplers_.size() - 1].get();
}

void App::startRecording()
{
    if (verbose_)
        std::cout << __FUNCTION__ << std::endl;
    if (recording_)
    {
        std::cerr << "Already recording." << std::endl;
        return;
    }
    //current_ = addSampler();

    addSampler();
    Sampler *current = getCurrentlyRecordingSampler();
    if (current)
    {
        current->startRecording();
        recording_ = true;
    }
}

void App::stopRecording()
{
    if (verbose_)
        std::cout << "App::" << __FUNCTION__ << "();" << std::endl;
    if (! recording_)
    {
        std::cerr << "Was not recording." << std::endl;
        return;
    }
    Sampler *current = getCurrentlyRecordingSampler();
    if (current)
    {
        current->stopRecording();
        recording_ = false;
    }
    else
        std::cerr << "invalid sampler\n";
}

void App::write(float x, float y)
{
    if (verbose_)
        std::cout << "App::" << __FUNCTION__ << "(" << x << ", " << y << ");" << std::endl;
    if (! recording_)
    {
        std::cerr << "Was not recording." << std::endl;
        return;
    }
    Sampler *current = getCurrentlyRecordingSampler();
    if (current)
    {
        tempi::Message m;
        m.appendFloat(x);
        m.appendFloat(y);
        if (verbose_)
            std::cout << "write " << x << " " << y << std::endl;
        current->getRecorder()->add(m);
    }
    else
        std::cerr << "Invalid sampler \n";
}

bool App::isRecording()
{
    return recording_;
}

void App::toggleFullscreen()
{
    fullscreen_ = ! fullscreen_;
    if (verbose_)
        std::cout << "App::" << __FUNCTION__ << ": " << fullscreen_ << std::endl;
    if (fullscreen_)
        clutter_stage_set_fullscreen(CLUTTER_STAGE(stage_), TRUE);
    else
        clutter_stage_set_fullscreen(CLUTTER_STAGE(stage_), FALSE);
}

void App::setFullscreen(bool enabled)
{
    if (fullscreen_ != enabled)
    {
        toggleFullscreen();
    }
}

void App::pollOSC()
{
    if (0 == osc_receiver_.get())
        return;
    if (osc_receiver_.get()->isRunning())
    {
        std::vector<tempi::Message> messages = osc_receiver_.get()->poll();
        std::vector<tempi::Message>::iterator iter;
        for (iter = messages.begin(); iter != messages.end(); ++iter)
        {
            tempi::Message msg = (*iter);
            if (msg.indexMatchesType(0, tempi::STRING))
                if (! handleOscMessage(msg))
                    std::cout << "Unhandled OSC message: " << msg << std::endl;
                else if (verbose_)
                    std::cout << "Handled OSC message: " << msg << std::endl;
        }
    }
}

bool App::handleOscMessage(const tempi::Message &message)
{
    //if (tempi::osc::oscMessageMatches(message, "/tempi/rec/select", "i"))
    //{
    //    std::cout << "TODO: /tempi/rec/select i" << std::endl;
    //    return true;
    //}
    if (tempi::osc::oscMessageMatches(message, "/tempi/rec/start", ""))
    {
        startRecording();
        return true;
    }
    if (tempi::osc::oscMessageMatches(message, "/tempi/rec/stop", ""))
    {
        stopRecording();
        return true;
    }
    if (tempi::osc::oscMessageMatches(message, "/tempi/play/speed", "f"))
    {
        setSpeed(message.getFloat(1));
        return true;
    }
    if (tempi::osc::oscMessageMatches(message, "/tempi/preferences/fullscreen", "b"))
    {
        setFullscreen(message.getBoolean(1));
        return true;
    }
    if (tempi::osc::oscMessageMatches(message, "/tempi/rec/write", "ff"))
    {
        if (isRecording())
            write(message.getFloat(1), message.getFloat(2));
        return true;
    }

    return false;
}

void App::drawSamplers()
{
    // Draw each sampler
    std::vector<std::tr1::shared_ptr<Sampler> >::iterator iter;
    for (iter = samplers_.begin(); iter < samplers_.end(); ++iter)
    {
        Sampler *sampler = (*iter).get();
        tempi::Message *m = sampler->getPlayer()->read();
        if (m)
        {
            if (m->typesMatch("ff"))
            {
                float x = m->getFloat(0);
                float y = m->getFloat(1);
                sampler->getGenerator()->setSourcePosition(x, y);
            }
            else
                std::cout << "types don't match: " << m->getTypes() << std::endl;
        }
        sampler->getGenerator()->onDraw();
    }
}

void App::onDraw()
{
    // Poll incoming OSC messages
    pollOSC();
    drawSamplers();
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
            app->playFaster();
            break;
        case CLUTTER_KEY_Down:
            app->playSlower();
            break;
    }
}

void App::updateSpeed()
{
    std::vector<std::tr1::shared_ptr<Sampler> >::iterator iter;
    for (iter = samplers_.begin(); iter < samplers_.end(); ++iter)
    {
        Sampler *sampler = (*iter).get();
        sampler->getPlayer()->setSpeed(speed_);
    }
}

void App::playFaster()
{
    speed_ = speed_ * 2.0;
    updateSpeed();
}

void App::playSlower()
{
    speed_ = speed_ * 0.5;
    updateSpeed();
}

void App::setSpeed(float speed)
{
    speed_ = speed;
    updateSpeed();
}

bool App::launch(int argc, char **argv)
{
    if (osc_recv_port_ == 0)
        std::cout << "OSC receiving disabled." << std::endl;
    else
        startOSC();
    // Poll OSC receiver only when we render a Clutter frame.

    if (stage_)
    {
        std::cerr << "cannot create stage twice" << std::endl;
        return false;
    }
    stage_ = clutter_stage_get_default();
    clutter_actor_set_size(stage_, 1024, 768);
    ClutterColor black = { 0x00, 0x00, 0x00, 0xff };
    clutter_stage_set_color(CLUTTER_STAGE(stage_), &black);
    g_signal_connect(stage_, "destroy", G_CALLBACK(clutter_main_quit), NULL);
    clutter_actor_set_reactive(stage_, TRUE);


    // timeline to attach a callback for each frame that is rendered
    ClutterTimeline *timeline;
    timeline = clutter_timeline_new(60); // ms
    clutter_timeline_set_loop(timeline, TRUE);
    clutter_timeline_start(timeline);

    g_signal_connect(timeline, "new-frame", G_CALLBACK(on_frame_cb), this);
    g_signal_connect(stage_, "key-press-event", G_CALLBACK(key_event_cb), this);
    g_signal_connect(stage_, "button-press-event", G_CALLBACK(button_press_cb), this);
    g_signal_connect(stage_, "button-release-event", G_CALLBACK(button_released_cb), this);
    g_signal_connect(stage_, "motion-event", G_CALLBACK(motion_event_cb), this);

    if (fullscreen_)
    {
        fullscreen_ = false;
        toggleFullscreen();
    }

    clutter_actor_show(stage_);
    return true;
}

bool App::startOSC()
{
    // starts OSC
    if (osc_recv_port_ == 0)
    {
        std::cout << "OSC receiving disabled." << std::endl;
        return false;
    }
    else
    {
        osc_receiver_.reset(new tempi::OscReceiver(osc_recv_port_));
        std::cout << "Receive OSC from " << (*osc_receiver_.get()) << std::endl;
    }
    return true;
}

/**
 * Return true if should exit the program.
 * Call this begore launching the app.
 */
bool App::parse_options(int argc, char **argv)
{
    po::options_description desc("Options");
    desc.add_options()
        ("help,h", "Show this help message and exit")
        ("version", "Show program's version number and exit")
        ("verbose,v", po::bool_switch(), "Enables a verbose output")
        ("fullscreen,f", po::bool_switch(), "Runs in fullscreen mode")
        ("osc-receive-port,p", po::value<unsigned int>()->default_value(0), "Listening OSC port [1024,65535]")
        ;
    po::variables_map options;
    po::store(po::parse_command_line(argc, argv, desc), options);
    po::notify(options);
    
    verbose_ = options["verbose"].as<bool>();
    fullscreen_ = options["fullscreen"].as<bool>();
    // Options that makes the program exit:
    if (options.count("help"))
    {
        std::cout << desc << std::endl;
        return true;
    }
    if (options.count("version"))
    {
        std::cout << PACKAGE << " " << PACKAGE_VERSION << std::endl;
        return true; 
    }
    // Options that leaves the App alive:
    if (options.count("osc-receive-port"))
    {
        osc_recv_port_ = options["osc-receive-port"].as<unsigned int>();
        if (osc_recv_port_ > 65535)
        {
            std::cerr << "OSC receiving Port number too high: " << osc_recv_port_ << std::endl;
            //return false;
        }
    }
    return false;
}

int main(int argc, char *argv[])
{
    if (clutter_init(&argc, &argv) != CLUTTER_INIT_SUCCESS)
        return 1;

    App app;
    if (app.parse_options(argc, argv))
        return 0;
    app.launch(argc, argv);

    clutter_main();
    return 0;
}

