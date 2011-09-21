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

//#ifdef HAVE_OSCPACK

#include "oscpack/ip/PacketListener.h"
#include "oscpack/osc/OscPacketListener.h"
#include "oscpack/ip/UdpSocket.h"
#include "oscpack/osc/OscPrintReceivedElements.h"
#include "oscpack/osc/OscReceivedElements.h"
#include "tempi/message.h"
//#include <cstdlib>
//#include <cstring>
#include <iostream>
#include <string>

namespace tempi
{

class OscDumpPacketListener : public osc::OscPacketListener
{
    public:
        //virtual void ProcessPacket(const char *data, int size, const IpEndpointName& remoteEndpoint)
        //{
        //    std::cout << osc::ReceivedPacket(data, size);
        //}
        virtual void ProcessMessage(const osc::ReceivedMessage &message, const IpEndpointName &remoteEndpoint)
        {
            //std::cout << message << " " << remoteEndpoint << std::endl;
        }
};

class OscReceiver
{
    public:
        OscReceiver(const int port=17666);
        virtual ~OscReceiver();
        void poll(std::string &oscPath, Message &message);
    private:
        int port_;
        static const int BUFF_SIZE = 1024;
        OscDumpPacketListener *listener_;
        UdpListeningReceiveSocket *socket_;
};

} // end of namespace

//#endif // HAVE_OSCPACK

#endif // include guard

