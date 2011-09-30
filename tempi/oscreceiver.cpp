#include "oscreceiver.h"
#include <boost/lexical_cast.hpp>
#include <stdio.h>
#include <string>

namespace tempi
{

OscReceiver::OscReceiver(unsigned int port) :
    port_(port),
    running_(false),
    debug_(false)
{
    if (port_ != 0)
        start();
}

bool OscReceiver::start()
{
    if (running_)
        return false;
    // TODO: handle errors
    server_ = lo_server_new(boost::lexical_cast<std::string>(port_).c_str(), onError);
    /* add method that will match any path and args */
    lo_server_add_method(server_, NULL, NULL, generic_handler, this);
    running_ = true;
    return true;
}

void OscReceiver::onError(int num, const char *msg, const char *path)
{
    std::cout << "liblo server error " << num << " in path " << path << ":"<< msg << std::endl;
}

OscReceiver::~OscReceiver()
{
    if (running_)
        lo_server_free(server_);
}

/**
 * catch any incoming messages.
 * Returning 1 means that the message has not been fully handled
 * and the server should try other methods
 */
int OscReceiver::generic_handler(const char *path, const char *types, lo_arg **argv,
                    int argc, void *data, void *user_data)
{
    Message message;
    message.appendString(path);
    OscReceiver *context = static_cast<OscReceiver *>(user_data);
    if (context->debug_)
        printf("path: <%s>\n", path);
    for (int i = 0; i < argc; i++)
    {
        if (context->debug_)
        {
            printf("arg %d '%c' ", i, types[i]);
            lo_arg_pp(static_cast<lo_type>(types[i]), argv[i]);
            printf("\n");
            fflush(stdout);
        }
        switch (types[i])
        {
            case 'i':
                message.appendInt(argv[i]->i);
                break;
            case 'f':
                message.appendFloat(argv[i]->f);
                break;
            case 's':
                message.appendString(static_cast<const char *>(&argv[i]->s));
                break;
            case 'c':
                message.appendChar(argv[i]->c);
                break;
            case 'd':
                message.appendChar(argv[i]->d);
                break;
            case 'T':
                message.appendBoolean(true);
                break;
            case 'F':
                message.appendBoolean(false);
                break;
            default:
                std::cerr << "OscReceiver::" << __FUNCTION__ << ": cannot handle lo arg type " << types[i] << std::endl;
                break;
        }
    }
    // TODO: time OSC messages so that timing be exact.
    context->messages_.push_back(message);
    return 1;
}

std::vector<Message> OscReceiver::poll()
{
    if (running_)
    {
        lo_server_recv_noblock(server_, 0);
        std::vector<Message> ret = messages_;
        messages_.clear();
        return ret;
    }
    else
    {
        std::cerr << "OscReceiver::" << __FUNCTION__ << ": Not running" << std::endl;
        std::vector<Message> ret;
        return ret;
    }
}

unsigned int OscReceiver::getPort() const
{
    return port_;
}

void OscReceiver::setDebug(bool enabled)
{
    debug_ = enabled;
}

bool OscReceiver::isRunning() const
{
    return running_;
}

std::ostream &operator<<(std::ostream &os, const OscReceiver &osc_receiver)
{
    os << "tempi::OscReceiver listening on osc.udp://localhost:" << osc_receiver.getPort();
    return os;
}

} // end of namespace temp

