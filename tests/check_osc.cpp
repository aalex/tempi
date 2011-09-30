#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include "tempi/message.h"
#include "tempi/oscsender.h"
#include "tempi/oscreceiver.h"

using namespace tempi;

static const bool VERBOSE = true;

bool check_oscsender()
{
    Message m;
    m.appendString("/hello");
    m.appendFloat(3.14159);
    m.appendString("spam");
    m.appendInt(2);

    if (VERBOSE)
    {
        std::cout << m << std::endl;
    }
    OscSender sender("localhost", 17666);
    sender.sendMessage(m);
    return true;
}

bool check_oscreceiver()
{
    OscReceiver receiver(14444);
    std::cout << receiver << std::endl;
    for (int i = 0; i < 10; ++i)
    {
        std::cout << "Please wait 1 second..." << std::endl;
        sleep(1); // 1 second
        std::vector<Message> messages = receiver.poll();
        std::vector<Message>::iterator iter;
        for (iter = messages.begin(); iter != messages.end(); ++iter)
        {
            std::cout << "Got " << (*iter) << std::endl;
        }
    }
    return true;
}

bool messages_match(const Message &a, const Message &b)
{
    if (a.getTypes() != b.getTypes())
    {
        return false;
    }
    for (unsigned int i = 1; i < a.getSize(); ++i)
    {
        ArgumentType type;
        a.getArgumentType(i, type);
        switch (type)
        {
            case BOOLEAN:
                if (a.getBoolean(i) != b.getBoolean(i))
                    return false;
                break;
            case CHAR:
                if (a.getChar(i) != b.getChar(i))
                    return false;
                break;
            case DOUBLE:
                if (a.getDouble(i) != b.getDouble(i))
                    return false;
                break;
            case FLOAT:
                if (a.getFloat(i) != b.getFloat(i))
                    return false;
                break;
            case INT:
                if (a.getInt(i) != b.getInt(i))
                    return false;
                break;
            case LONG:
                if (a.getLong(i) != b.getLong(i))
                    return false;
                break;
            case STRING:
                if (a.getString(i) != b.getString(i))
                    return false;
                break;
            defaut:
                std::cerr << __FUNCTION__ << ": Unsupported type." << std::endl;
                break;
        }
    }
    return true;
}

bool sent_and_received_matches(OscSender &sender, OscReceiver &receiver, const Message &message)
{
    sender.sendMessage(message);
    std::vector<Message> messages = receiver.poll();
    if (messages.size() == 0)
    {
        std::cout << "Did not receive the message." << std::endl;
        return false;
    }
    if (messages.size() > 1)
    {
        std::cout << "Received too many messages." << std::endl;
        return false;
    }
    std::vector<Message>::iterator iter;
    for (iter = messages.begin(); iter != messages.end(); ++iter)
    {
        std::cout << "Got " << (*iter) << std::endl;
        Message received = (*iter);
        if (! messages_match(message, received))
        {
            std::cout << "Messages don't match: Sent " << message << " but got " << received << std::endl;
            return false;
        }
    }
    return true;
}

bool check_send_and_receive()
{
    unsigned int port = 15555;
    OscSender sender("localhost", port);
    OscReceiver receiver(port);

    Message m;
    m.appendString("/foo/bar");
    m.appendString("hello");
    m.appendInt(2);
    m.appendFloat(3.14159);
    m.appendBoolean(true);
    if (! sent_and_received_matches(sender, receiver, m))
        return false;

    return true;
}

int main(int argc, char *argv[])
{
    //if (! check_oscsender())
    //    return 1;
    //if (! check_oscreceiver())
    //    return 1;
    if (! check_send_and_receive())
        return 1;
    return 0;
}

