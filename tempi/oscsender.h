#ifndef __OSC_SENDER_H__
#define __OSC_SENDER_H__

#ifdef HAVE_OSCPACK

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
    void sendMessage(const std::string &OSCpath, const Message &message);

private:
    int port_;
    char *ADDRESS;
    static const int BUFF_SIZE = 1024;
    UdpTransmitSocket *socket_;
    char *oscBuffer_;
    void initialize();
};

} // end of namespace

#endif // HAVE_OSCPACK

#endif // __OSC_SENDER_H__
