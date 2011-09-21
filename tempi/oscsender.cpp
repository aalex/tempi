#include "oscsender.h"

namespace tempi
{
    
//#ifdef HAVE_OSCPACK

OscSender::OscSender(const std::string &host, const int port)
{
    socket_ = new UdpTransmitSocket(IpEndpointName(host.c_str(), port));
    oscBuffer_ = new char[BUFF_SIZE];
}

OscSender::~OscSender()
{
    delete socket_;
    delete[] oscBuffer_;
}

void OscSender::sendMessage(const std::string &oscPath, const Message &message)
{
    osc::OutboundPacketStream os = osc::OutboundPacketStream(oscBuffer_, BUFF_SIZE);
    os << osc::BeginMessage(oscPath.c_str());
    for (unsigned int i = 0; i < message.getSize(); ++i)
    {
        ArgumentType type;
        message.getArgumentType(i, type);
        switch (type)
        {
            case BOOLEAN:
            {
                bool value;
                message.getBoolean(i, value);
                os << value;
                break;
            }
            case CHAR:
            {
                char value;
                message.getChar(i, value);
                os << value;
                break;
            }
            case DOUBLE:
            {
                double value;
                message.getDouble(i, value);
                os << value;
                break;
            }
            case FLOAT:
            {
                float value;
                message.getFloat(i, value);
                os << value;
                break;
            }
            case INT:
            {
                int value;
                message.getInt(i, value);
                os << value;
                break;
            }
            case LONG:
            {
                long long int value;
                message.getLong(i, value);
                os << value;
                break;
            }
            case STRING:
            {
                std::string value;
                message.getString(i, value);
                os << value.c_str();
                break;
            }
            defaut:
            {
                std::cout << "OscSender::" << __FUNCTION__ << ": Unsupported type." << std::endl;
                break;
            }
        }
    }
    os << osc::EndMessage;
    socket_->Send(os.Data(), os.Size());
}

//#endif // HAVE_OSCPACK

} // end of namespace temp
