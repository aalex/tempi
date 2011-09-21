#include "oscsender.h"


namespace tempi
{
    
#ifdef HAVE_OSCPACK

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

void OscSender::sendMessage(const std::string &oscpath, const Message &message)
{
    osc::OutboundPacketStream os = osc::OutboundPacketStream(oscBuffer_, BUFF_SIZE);
    os << osc::BeginMessage(oscpath.c_str());
    for (unsigned int i = 0; i < message.getSize(); ++i)
    {
	ArgumentType type;
	message.getArgumentType(i, type);
	if (type == BOOLEAN)
	{
	    bool value;
	    message.getBoolean(i, value);
	    os << value;
	}
	else if (type == CHAR)
	{
	    char value;
	    message.getChar(i, value);
	    os << value;
	}
	else if (type == DOUBLE)
	{
	    double value;
	    message.getDouble(i, value);
	    os << value;
	}
	else if (type == FLOAT)
	{
	    float value;
	    message.getFloat(i, value);
	    os << value;
	}
	else if (type == INT)
	{
	    int value;
	    message.getInt(i, value);
	    os << value;
	}
	else if (type == LONG)
	{
	    long long int value;
	    message.getLong(i, value);
	    os << value;
	}
	else if (type == STRING)
	{
	    std::string value;
	    message.getString(i, value);
	    //os << value;
	    // FIXME!!!
	}
    }
    os << osc::EndMessage;
    socket_->Send(os.Data(), os.Size());
}

#endif // HAVE_OSCPACK

} // end of namespace temp
