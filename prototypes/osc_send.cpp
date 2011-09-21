#include <iostream>
#include <oscpack/osc/OscOutboundPacketStream.h>
#include <oscpack/ip/UdpSocket.h>

const char *ADDRESS = "localhost";
int PORT = 7000;
#define BUFF_SIZE 1024

int main(int argc, char *argv[])
{
    UdpTransmitSocket transmitSocket(IpEndpointName(ADDRESS, PORT));
    char buffer[BUFF_SIZE];
    osc::OutboundPacketStream packet (buffer, BUFF_SIZE);
    packet << osc::BeginMessage("/test1") << true << 10 << (float)3.14 << "Gotcha!" << osc::EndMessage;

    transmitSocket.Send(packet.Data(), packet.Size());
    return 0;    
}

