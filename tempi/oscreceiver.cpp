#include "oscreceiver.h"

namespace tempi
{
    
//#ifdef HAVE_OSCPACK

OscReceiver::OscReceiver(const int port)
{
	listener_ = new OscDumpPacketListener();
    socket_ = new UdpListeningReceiveSocket(
            IpEndpointName(IpEndpointName::ANY_ADDRESS, port),
            listener_);
}

OscReceiver::~OscReceiver()
{
    delete socket_;
    delete listener_;
}

void OscReceiver::poll(std::string & /*oscpath*/, Message & /*message*/)
{
    // TODO
}

//#endif // HAVE_OSCPACK

} // end of namespace temp
