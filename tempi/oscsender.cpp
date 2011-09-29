#include "oscsender.h"

namespace tempi
{
    
//#ifdef HAVE_OSCPACK

OscSender::OscSender(const std::string &host, const int port)
{
//    socket_ = new UdpTransmitSocket(IpEndpointName(host.c_str(), port));
//    oscBuffer_ = new char[BUFF_SIZE];
}

OscSender::~OscSender()
{
//    delete socket_;
//    delete[] oscBuffer_;
}

void OscSender::sendMessage(const std::string &oscPath, const Message &message)
{
// TODO
    std::cerr << "OscSender::" << __FUNCTION__ << " is not implemented" << std::endl;
//    osc::OutboundPacketStream os = osc::OutboundPacketStream(oscBuffer_, BUFF_SIZE);
//    os << osc::BeginMessage(oscPath.c_str());
//    for (unsigned int i = 0; i < message.getSize(); ++i)
//    {
//
//        ArgumentType type;
//        message.getArgumentType(i, type);
//        switch (type)
//        {
//            case BOOLEAN:
//                os << message.getBoolean(i);
//                break;
//            case CHAR:
//                os << message.getChar(i);
//                break;
//            case DOUBLE:
//                os << message.getDouble(i);
//                break;
//            case FLOAT:
//                os << message.getFloat(i);
//                break;
//            case INT:
//                os << message.getInt(i);
//                break;
//            case LONG:
//                os << message.getLong(i);
//                break;
//            case STRING:
//                os << message.getString(i).c_str();
//                break;
//            defaut:
//                std::cerr << "OscSender::" << __FUNCTION__ << ": Unsupported type." << std::endl;
//                break;
//        }
//    }
//    os << osc::EndMessage;
//    socket_->Send(os.Data(), os.Size());
}

//#endif // HAVE_OSCPACK

} // end of namespace temp
