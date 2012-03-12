/*
 * Copyright (C) 2011 Alexandre Quessy
 * Copyright (C) 2011 Michal Seta
 * Copyright (C) 2012 Nicolas Bouillot
 *
 * This file is part of Tempi.
 *
 * This program is free software: you can redistribute it and/or
 * modify it under the terms of, either version 3 of the License, or
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
 * @file The TempiClutterHelloNode class.
 */

#include "tempi/config.h"
#ifdef HAVE_CLUTTER

#include <clutter/clutter.h>
#include "tempi/message.h"
#include "tempi/utils.h"
#include "tempi/log.h"
#include "plugins-base/clutter/clutterhello.h"
#include <iostream>
#include <tr1/memory>
#include <string>
#include <vector>

namespace tempi {
namespace plugins_base {

// Color constants:
static ClutterColor black = { 0x00, 0x00, 0x00, 0xff };
static ClutterColor red = { 0xff, 0x00, 0x00, 0xff };

// static functions:
static void on_frame_cb(ClutterTimeline *timeline, guint *ms, gpointer user_data);
static void key_event_cb(ClutterActor *actor, ClutterKeyEvent *event, gpointer user_data);

HelloWorker::HelloWorker(TempiClutterHelloNode *node) :
    tempiNode_(node)
{
}

static void on_stage_destroyed(gpointer *user_data)
{
    TempiClutterHelloNode *self = (TempiClutterHelloNode *) user_data;
    (void) self;

    clutter_threads_enter();
    clutter_main_quit();
    clutter_threads_leave();
}

TempiClutterHelloNode::TempiClutterHelloNode()
{
    setShortDocumentation("Simple Clutter test. (not yet working)");
    addAttribute(Attribute::ptr(new Attribute("size", Message("ii", 640, 480), "Size of the ClutterStage.")));
    addAttribute(Attribute::ptr(new Attribute("visible", Message("b", false), "Shows the stage when enabled.")));
}

TempiClutterHelloNode::~TempiClutterHelloNode()
{
    clutterThread_.reset();
}

void TempiClutterHelloNode::startClutterThread()
{
    if (worker_.get() != 0) // only once!
    {
        std::ostringstream os;
        os << __FILE__ << ": " << __FUNCTION__ << ": Already has working thread..";
        Logger::log(WARNING, os);
        return;
    }
    worker_.reset(new HelloWorker(this));
    clutterThread_.reset(new boost::thread(*(worker_.get())));
}

bool HelloStageManager::initStage()
{
    if (! clutter_feature_available(CLUTTER_FEATURE_STAGE_MULTIPLE))
    {
        std::ostringstream os;
        os << __FILE__ << ": " << __FUNCTION__ << ": Current Clutter backend does not support multiple stages!.";
        Logger::log(ERROR, os);
    }
    stage_ = clutter_stage_new(); // clutter_stage_get_default();
    // FIXME: check for CLUTTER_FEATURE_STAGE_MULTIPLE and create new stage

    //float width = (float) getAttribute("size").getInt(0);
    //float height = (float) getAttribute("size").getInt(1);
    float width = 320.0f;
    float height = 240.0f;

    clutter_actor_set_size(stage_, width, height);
    clutter_stage_set_color(CLUTTER_STAGE(stage_), &black);
    g_signal_connect(stage_, "destroy", G_CALLBACK(on_stage_destroyed), (gpointer) this);

    ClutterActor *rect = clutter_rectangle_new_with_color(&red);
    clutter_actor_set_size(rect, 100.0f, 100.0f);
    clutter_container_add_actor(CLUTTER_CONTAINER(stage_), rect);
    clutter_actor_show_all(rect);

    // timeline to attach a callback for each frame that is rendered
    ClutterTimeline *timeline;
    timeline = clutter_timeline_new(60); // ms
    clutter_timeline_set_loop(timeline, TRUE);
    clutter_timeline_start(timeline);
    g_signal_connect(timeline, "new-frame", G_CALLBACK(on_frame_cb), this);
    g_signal_connect(stage_, "key-press-event", G_CALLBACK(key_event_cb), this);

    clutter_actor_show(stage_);

    if (CLUTTER_IS_ACTOR(stage_))
        return true;
    else
        return false;
}

// starts the thread
void HelloWorker::operator()()
{
    {
        std::ostringstream os;
        os << __FILE__ << ": HelloWorker." << __FUNCTION__ << ": Init Clutter thread.";
        Logger::log(INFO, os);
    }
    HelloStageManager::ptr manager;
    manager.reset(new HelloStageManager(tempiNode_));

    g_thread_init(NULL);
    clutter_threads_init();

    int argc = 0;
    char **argv = NULL;

    if (clutter_init(&argc, &argv) != CLUTTER_INIT_SUCCESS)
    {
        std::ostringstream os;
        os << __FILE__ << ": " << __FUNCTION__ << ": Error calling clutter_init()";
        Logger::log(ERROR, os);
        return;
    }
    clutter_threads_enter();

    manager->initStage();
    {
        std::ostringstream os;
        os << __FILE__ << ": HelloWorker." << __FUNCTION__ << ": Running clutter_main...";
        Logger::log(INFO, os);
    }
    clutter_main();

    clutter_threads_leave();
    return; // done with this thread
}

HelloStageManager::HelloStageManager(TempiClutterHelloNode *node) :
    tempiNode_(node)
{
}

HelloStageManager::~HelloStageManager()
{
    if (CLUTTER_IS_ACTOR(stage_))
    {
        std::ostringstream os;
        os << __FILE__ << ": " << __FUNCTION__ << ": Destroy ClutterStage";
        Logger::log(INFO, os);
        clutter_actor_destroy(stage_);
    }
}

static void on_frame_cb(ClutterTimeline * /*timeline*/, guint * /*ms*/, gpointer user_data)
{
    TempiClutterHelloNode *context = static_cast<TempiClutterHelloNode*>(user_data);
    (void) context; // TODO remove this
    //context->poll();
}

static void key_event_cb(ClutterActor *actor, ClutterKeyEvent *event, gpointer user_data)
{
    TempiClutterHelloNode *self = (TempiClutterHelloNode *) user_data;
    (void) self; // TODO: remove this
    ClutterModifierType state = clutter_event_get_state((ClutterEvent*) event);
    bool ctrl_pressed = (state & CLUTTER_CONTROL_MASK ? true : false);
    switch (event->keyval)
    {
        case CLUTTER_KEY_q:
            if (ctrl_pressed)
            {
                clutter_actor_destroy(self->getWorker()->getManager()->getStage());
                //clutter_main_quit();
            }
            break;
    }
}

void TempiClutterHelloNode::processMessage(const char *inlet, const Message &message)
{
    // TODO: implement processMessage
}

bool TempiClutterHelloNode::onNodeAttributeChanged(const char *name, const Message &value)
{
    if (utils::stringsMatch(name, "visible"))
    {
        if (value.getBoolean(0))
        {
            std::ostringstream os;
            os << "Starting Clutter thread...";
            Logger::log(INFO, os);
            startClutterThread();
        }
        else
        {
            std::ostringstream os;
            os << "Not starting Clutter thread yet.";
            Logger::log(INFO, os);
        }
    }
    return true;
}

Message HelloStageManager::getAttribute(const char *name)
{
    // TODO: exit clutter thread?
    return tempiNode_->getAttributeValue(name);
}

} // end of namespace
} // end of namespace

#endif // HAVE_CLUTTER

