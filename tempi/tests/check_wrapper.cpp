#include "tempi/wrapper.h"
#include <iostream>
#include <unistd.h>

using namespace tempi;
static const bool VERBOSE = false;

bool check_wrapper()
{
    Wrapper engine;
    if (VERBOSE)
        engine.setLogLevel("ERROR");
    // engine.setSynchronous(false);
    engine.createGraph("graph0");
    engine.createNode("graph0", "base.metro", "metro0");
    engine.createNode("graph0", "base.print", "print0");
    engine.setNodeAttributeValue("graph0", "metro0", "interval", Message("i", 100));
    engine.setNodeAttributeValue("graph0", "metro0", "running", Message("b", true));
    if (! VERBOSE)
        engine.setNodeAttributeValue("graph0", "print0", "enabled",
            Message("b", false));

    engine.connect("graph0", "metro0", "0", "print0", "0");

    for (int i = 0; i < 10; ++i)
    {
        // engine.tick();
        usleep(50 * 1000); // 50 ms
    }
    return true;
}

int main(int argc, char *argv[])
{
    if (! check_wrapper())
        return 1;
    return 0;
}

