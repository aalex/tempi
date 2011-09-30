#include "oscreceiver.h"
#include <boost/lexical_cast.hpp>
#include <stdio.h>
#include <string>

namespace tempi
{

OscReceiver::OscReceiver(unsigned int port) :
    port_(port),
    running_(false)
{
    // TODO: handle errors
    server_ = lo_server_new(boost::lexical_cast<std::string>(port_).c_str(), onError);
    /* add method that will match any path and args */
    lo_server_add_method(server_, NULL, NULL, generic_handler, this);
    /* get the file descriptor of the server socket, if supported */
    //lo_fd_ = lo_server_get_socket_fd(server_);
}

void OscReceiver::onError(int num, const char *msg, const char *path)
{
    std::cout << "liblo server error " << num << " in path " << path << ":"<< msg << std::endl;
}

OscReceiver::~OscReceiver()
{
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
    Message args;
    OscReceiver *context = static_cast<OscReceiver *>(user_data);
    printf("path: <%s>\n", path);
    for (int i = 0; i < argc; i++)
    {
        //printf("arg %d '%c' ", i, types[i]);
        lo_arg_pp(static_cast<lo_type>(types[i]), argv[i]);
        //printf("\n");
        switch (types[i])
        {
            case 'i':
                args.appendInt(argv[i]->i);
                break;
            case 'f':
                args.appendFloat(argv[i]->f);
                break;
            default:
                std::cout << "cannot handle lo arg type " << types[i] << std::endl;
                break;
        }
    }
    //printf("\n");
    //fflush(stdout);
    // TODO: time OSC messages so that timing be exact.
    context->messages_.push_back(OscMessage(std::string(path), args));
    return 1;
}

std::vector<OscMessage> OscReceiver::poll()
{
    lo_server_recv_noblock(server_, 0);
    std::vector<OscMessage> ret = messages_;
    messages_.clear();
    return ret;
}

} // end of namespace temp

