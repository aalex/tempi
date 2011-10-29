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
 * @file
 * The OscReceiver class
 */

#ifndef __TEMPI_OSCRECEIVER_H__
#define __TEMPI_OSCRECEIVER_H__

#include "tempi/message.h"
#include "tempi/sharedptr.h"
#include <boost/tuple/tuple.hpp>
#include <iostream>
#include <lo/lo.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <strings.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

/**
 * The OscReceiver class.
 */

namespace tempi {
namespace osc {

std::string removeFirstChar(const std::string &from);
bool oscMessageMatches(const tempi::Message &message, const char *path, const char *types);

/**
 * OpenSoundControl receiver.
 */
class OscReceiver
{
    public:
        typedef std::tr1::shared_ptr<OscReceiver> ptr;
        /**
         * Use port 0 to disable OSC receiving.
         */
        OscReceiver(unsigned int port = 0);
        virtual ~OscReceiver();
        /**
         * Flushes all the messages since last time we flushed.
         */
        std::vector<Message> poll();
        unsigned int getPort() const;
        void setDebug(bool enabled);
        bool isRunning() const;
        static bool oscMessageMatches(const tempi::Message &message, const char *path, const char *types);
    private:
        unsigned int port_;
        bool running_;
        bool debug_;
        lo_server server_;
        static void onError(int num, const char *m, const char *path);
        std::vector<Message> messages_;
        static int generic_handler(const char *path, const char *types, lo_arg **argv,
            int argc, void *data, void *user_data);
        bool start();
};

std::ostream &operator<<(std::ostream &os, const OscReceiver &osc_receiver);

} // end of namespace
} // end of namespace

#endif // include guard

