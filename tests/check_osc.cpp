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
    m.appendFloat(3.14159);
    m.appendString("hello");
    m.appendInt(2);

    if (VERBOSE)
    {
        std::cout << m << std::endl;
    }
    OscSender sender("localhost", 17666);
    //TODO: OscReceiver(17666);
    sender.sendMessage("/bogus", m);
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

int main(int argc, char *argv[])
{
    if (! check_oscsender())
        return 1;
    if (! check_oscreceiver())
        return 1;
    return 0;
}

