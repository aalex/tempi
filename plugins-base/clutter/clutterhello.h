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
 * The ClutterHelloNodHelloNode class.
 */

#ifndef __TEMPI_CLUTTER_HELLO_H__
#define __TEMPI_CLUTTER_HELLO_H__

#include "tempi/config.h"

#ifdef HAVE_CLUTTER

#include "tempi/node.h"
#include "tempi/sharedptr.h"
#include "tempi/message.h"
#include <string>
#include <clutter/clutter.h>
#include <boost/thread.hpp>  
// #include <mx/mx.h>
#include <vector>

namespace tempi {
namespace plugins_base {

// forward declarations
class HelloStageManager;
class TempiClutterHelloNode;

/**
 * Manages a ClutterStage and communicates to the TempiClutterHelloNode.
 */
class HelloStageManager
{
    public:
        typedef std::tr1::shared_ptr<HelloStageManager> ptr;
        HelloStageManager(TempiClutterHelloNode *node);
        ~HelloStageManager();
        TempiClutterHelloNode *getTempiNode() { return tempiNode_; }
        bool initStage();
        ClutterActor *getStage()
        {
            return stage_;
        }
    private:
        TempiClutterHelloNode *tempiNode_;
        ClutterActor *stage_;
        bool clutterInitOk_;
        Message getAttribute(const char *name);
        //bool initClutter();
};

/**
 * Thread worker for the ClutterStage.
 */
class HelloWorker
{
    public:
        typedef std::tr1::shared_ptr<HelloWorker> ptr;
        HelloWorker(TempiClutterHelloNode *node);
        HelloStageManager::ptr getManager()
        {
            return manager_;
        }
        void operator()();
    private:
        HelloStageManager::ptr manager_;
        TempiClutterHelloNode *tempiNode_;
};

/**
 * The TempiClutterHelloNode contains a ClutterStage and is scriptable via JSON ClutterScript.
 */
class TempiClutterHelloNode : public Node
{
    public:
        TempiClutterHelloNode();
        ~TempiClutterHelloNode();
        HelloWorker::ptr getWorker()
        {
            return worker_;
        }
    protected:
        virtual void processMessage(const char *inlet, const Message &message);
        virtual bool onNodeAttributeChanged(const char *name, const Message &value);
    private:
        // TODO: accumulate messages in an async queue
        std::tr1::shared_ptr<boost::thread> clutterThread_;
        HelloWorker::ptr worker_;
        void startClutterThread();
};

} // end of namespace
} // end of namespace

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

#endif // HAVE_CLUTTER
#endif // ifndef

