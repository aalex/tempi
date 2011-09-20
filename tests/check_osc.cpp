#include <iostream>
#include <string>
#include "tempi/message.h"
#include "tempi/oscsender.h"

using namespace tempi;

static const bool VERBOSE = true;

bool check_oscsender()
{
    Message m;
    m.appendFloat(3.14159);
    m.appendString(std::string("hello"));
    m.appendInt(2);

    if (VERBOSE)
    {
        std::cout << m << std::endl;
    }
    OscSender sender("localhost");
    sender.sendMessage("/bogus", m);
    return true;
}

int main(int argc, char *argv[])
{
    if (not check_oscsender())
        return 1;
    return 0;
}

