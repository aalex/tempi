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
 * @file The TempiClutterStageNode class.
 */

#include "tempi/config.h"
#ifdef HAVE_CLUTTER

#include <clutter/clutter.h>
#include "tempi/message.h"
#include "tempi/utils.h"
#include "plugins-base/clutter/clutterstagenode.h"
#include <iostream>
#include <tr1/memory>
#include <string>
#include <string.h> // for strlen
#include <vector>
//TODO #include <mx/mx.h>

namespace tempi {
namespace plugins_base {

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

static gboolean tempi_clutter_event_cb(ClutterActor *actor, gpointer user_data)
{
    g_return_val_if_fail(CLUTTER_IS_ACTOR(actor), FALSE);
    SignalConnection *conn = static_cast<SignalConnection *>(user_data);
    // g_message("%s: actor %s triggered event [%s][%s]",
    //     __FUNCTION__,
    //     conn->actor_name_.c_str(), 
    //     conn->signal_name_.c_str(), 
    //     conn->handler_name_.c_str());
    Message message;
    message.appendString(conn->actor_name_.c_str());
    message.appendString(conn->signal_name_.c_str());
    message.appendString(conn->handler_name_.c_str());

    conn->manager_->getTempiNode()->onMessageFromStage(message);;
    return TRUE; // handled
}

Worker::Worker(TempiClutterStageNode *node) :
    tempiNode_(node)
{
}

/**
 * Custom handler for the signals in the ClutterScript.
 * Slots are turned into Message instances
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
    SignalConnection::ptr conn(new SignalConnection);
    //std::cout << __FUNCTION__ << ": sig:" << signal_name << ", handler:" << handler_name << std::endl;

    // store info about the signal we are connecting to:
    //std::cout << "set signal name to " << signal_name << " length:" << strlen(signal_name) << std::endl;
    conn->signal_name_.assign(signal_name, strlen(signal_name));

    //std::cout << "set handler name to " << handler_name << " length:" << strlen(handler_name) << std::endl;
    conn->handler_name_.assign(handler_name, strlen(handler_name));

    if (CLUTTER_IS_ACTOR(object))
    {
        //std::cout << "set actor name to " << clutter_actor_get_name(CLUTTER_ACTOR(object)) << std::endl;
        const char *actor_name = clutter_actor_get_name(CLUTTER_ACTOR(object));
        if (actor_name != NULL)
            conn->actor_name_.assign(actor_name, strlen(actor_name));
    }
    else
        std::cout << "Not an ClutterActor." << std::endl;
    // Store the connection object in the app:
    StageManager *manager = static_cast<StageManager *>(user_data);
    conn->manager_ = manager;
    manager->connections_.push_back(conn);

    // connect the signal to our generic callback:
    if (! g_signal_connect(object, signal_name, G_CALLBACK(tempi_clutter_event_cb), conn.get()))
    {
        g_critical("%s: Couldn't connect to signal %s", __FUNCTION__, signal_name);
    }
}

static void on_stage_destroyed(gpointer *user_data)
{
    TempiClutterStageNode *self = (TempiClutterStageNode *) user_data;
    (void) self;

    clutter_threads_enter();
    clutter_main_quit();
    clutter_threads_leave();
}

TempiClutterStageNode::TempiClutterStageNode()
{
    //TODO: addInlet();
    //TODO: addOutlet();
    setShortDocumentation("Manages a Clutter stage. (not yet working)");
    addAttribute(Attribute::ptr(new Attribute("script", Message("s", ""), "JSON file to load with ClutterScript.")));
    addAttribute(Attribute::ptr(new Attribute("size", Message("ii", 640, 480), "Size of the ClutterStage.")));
    addAttribute(Attribute::ptr(new Attribute("visible", Message("b", false), "Shows the stage when enabled.")));
}

TempiClutterStageNode::~TempiClutterStageNode()
{
    clutterThread_.reset();
}

void TempiClutterStageNode::startClutterThread()
{
    if (worker_.get() == 0) // only once!
        return;
    worker_.reset(new Worker(this));
    clutterThread_.reset(new boost::thread(*(worker_.get())));
}

bool StageManager::initStage()
{
    stage_ = clutter_stage_get_default();

    float width = (float) getAttribute("size").getInt(0);
    float height =(float) getAttribute("size").getInt(1);
    clutter_actor_set_size(stage_, width, height);
    clutter_stage_set_color(CLUTTER_STAGE(stage_), &black);
    g_signal_connect(stage_, "destroy", G_CALLBACK(on_stage_destroyed), (gpointer) this);

    std::string filename = getAttribute("script").getString(0);

    GError *error = NULL;
    /* load JSON from a file */
    ClutterScript *script = clutter_script_new();
    clutter_script_load_from_file(script, filename.c_str(), &error);

    if (error != NULL)
    {
        std::cerr << "Unable to read file: " <<  error->message << std::endl;
        g_error_free(error);
        // TODO: fail
        g_critical("Could not load GUI");
        return false;
    }
    ClutterActor *group0 = CLUTTER_ACTOR(clutter_script_get_object(script, "group0")); // TODO: rename to root
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

    if (CLUTTER_IS_ACTOR(stage_))
        return true;
    else
        return false;
}

// starts the thread
void Worker::operator()()
{
    StageManager::ptr manager;

    g_thread_init(NULL);
    clutter_threads_init();

    int argc = 0;
    char **argv = NULL;

    if (clutter_init(&argc, &argv) != CLUTTER_INIT_SUCCESS)
    {
        std::cerr << __FUNCTION__ << "(): Error calling clutter_init()\n";
        return;
    }
    clutter_threads_enter();

    manager->initStage();

    clutter_main();

    clutter_threads_leave();
    return; // done with this thread
}

StageManager::StageManager(TempiClutterStageNode *node) :
    tempiNode_(node)
{
}

StageManager::~StageManager()
{
    if (CLUTTER_IS_ACTOR(stage_))
    {
        std::cout << __FUNCTION__ << "(): Destroy ClutterStage\n";
        clutter_actor_destroy(stage_);
    }
}

void TempiClutterStageNode::onMessageFromStage(const Message &message)
{
    // TODO: accumulate messages in an async queue
    std::cout << "TempiClutterStageNode::" << __FUNCTION__ << "(): " << message << std::endl;
}

// void on_button0_clicked(MxButton *actor, gpointer data)
// {
//     (void) actor;
//     (void) data;
//     std::cout << __FUNCTION__ << "" << std::endl;
// }

static void on_frame_cb(ClutterTimeline * /*timeline*/, guint * /*ms*/, gpointer user_data)
{
    TempiClutterStageNode *context = static_cast<TempiClutterStageNode*>(user_data);
    (void) context; // TODO remove this
    //context->poll();
}

static void key_event_cb(ClutterActor *actor, ClutterKeyEvent *event, gpointer user_data)
{
    TempiClutterStageNode *self = (TempiClutterStageNode *) user_data;
    (void) self; // TODO: remove this
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

void TempiClutterStageNode::processMessage(const char *inlet, const Message &message)
{
    // TODO: implement processMessage
}
void TempiClutterStageNode::onAttributeChanged(const char *name, const Message &value)
{
    if (utils::stringsMatch(name, "visible"))
    {
        if (value.getBoolean(0))
            startClutterThread();
    }
    // TODO: implement more attributes support
}

// static bool tempi_clutter_set_obj_property(GObject *object, const char *name, const Message &value)
// {
//     GParamSpec **pspecs, **p;
//     guint i = 0;
// 
//     g_datalist_init (&settings->queued_settings);
//     object_list = g_slist_prepend (object_list, settings);
// 
//     /* build up property array for all yet existing properties and queue
//      * notification for them (at least notification for internal properties
//      * will instantly be caught)
//      */
//     pspecs = g_object_class_list_properties (G_OBJECT_GET_CLASS (settings), NULL);
//     for (p = pspecs; *p; p++)
//         if ((*p)->owner_type == G_OBJECT_TYPE (settings))
//             i++;
//     settings->property_values = g_new0 (GtkSettingsPropertyValue, i);
//     i = 0;
//     g_object_freeze_notify (G_OBJECT (settings));
//     for (p = pspecs; *p; p++)
//     {
//         GParamSpec *pspec = *p;
// 
//         if (pspec->owner_type != G_OBJECT_TYPE (settings))
//             continue;
//         g_value_init (&settings->property_values[i].value, G_PARAM_SPEC_VALUE_TYPE (pspec));
//         g_param_value_set_default (pspec, &settings->property_values[i].value);
//         g_object_notify (G_OBJECT (settings), pspec->name);
//         settings->property_values[i].source = GTK_SETTINGS_SOURCE_DEFAULT;
//         i++;
//     }
//     g_object_thaw_notify (G_OBJECT (settings));
//     g_free (pspecs); 
// }
//

Message StageManager::getAttribute(const char *name)
{
    return tempiNode_->getAttributeValue(name);
}

} // end of namespace
} // end of namespace

#endif // HAVE_CLUTTER

