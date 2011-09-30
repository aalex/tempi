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

#ifndef __OSC_RECEIVER_H__
#define __OSC_RECEIVER_H__

#include "tempi/message.h"
#include <boost/tuple/tuple.hpp>
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <strings.h>
#include <unistd.h>
#include <lo/lo.h>

namespace tempi
{

/**
 * OpenSoundControl receiver.
 * To use it, you must extend this class and implement its onMessageReceived virtual method.
 */
class OscReceiver
{
    public:
        OscReceiver(unsigned int port);
        virtual ~OscReceiver();
        std::vector<Message> poll();
        unsigned int getPort() const;
        void setDebug(bool enabled);
        bool start();
    private:
        unsigned int port_;
        bool running_;
        bool debug_;
        lo_server server_;
        static void onError(int num, const char *m, const char *path);
        std::vector<Message> messages_;
        static int generic_handler(const char *path, const char *types, lo_arg **argv,
            int argc, void *data, void *user_data);
};

std::ostream &operator<<(std::ostream &os, const OscReceiver &osc_receiver);

} // end of namespace

#endif // include guard

