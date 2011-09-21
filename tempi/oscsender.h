
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
 * The OscSender class.
 */

#ifndef __OSC_SENDER_H__
#define __OSC_SENDER_H__

//#ifdef HAVE_OSCPACK

#include <iostream>
#include <string>
#include "tempi/message.h"
#include "oscpack/osc/OscOutboundPacketStream.h"
#include "oscpack/ip/UdpSocket.h"

namespace tempi
{

class OscSender
{
    public:
        /**
         * Constructor will create a UdpTransmitSocket to host:port
         */
        OscSender(const std::string  &host, const int port=17666);
        virtual ~OscSender();
        void sendMessage(const std::string &oscPath, const Message &message);

    private:
        int port_;
        char *ADDRESS;
        static const int BUFF_SIZE = 1024;
        UdpTransmitSocket *socket_;
        char *oscBuffer_;
};

} // end of namespace

//#endif // HAVE_OSCPACK

#endif // __OSC_SENDER_H__

