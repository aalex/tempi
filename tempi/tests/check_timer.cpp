#include <iostream>
#include <unistd.h>
#include "tempi/timer.h"

using namespace tempi;

static const bool VERBOSE = false;

bool check_timer()
{
    Timer timer;
    // TODO: make it faster than 1 second
    //if (VERBOSE)
    //std::cout << "Please wait while testing tempi::Timer..." << std::endl;
    usleep(1000); // 1 ms

    TimePosition elapsed = timer.elapsed();
    if (VERBOSE)
        std::cout << "Elapsed: " << elapsed << std::endl;
    if (elapsed < 1000000L || elapsed > 1400000L )
    {
        std::cout << "About 1 ms should have gone away. But it's " << elapsed << std::endl;
        return false;
    }

    timer.setPosition(2000000L);
    elapsed = timer.elapsed();
    if (VERBOSE)
        std::cout << "Elapsed: " << elapsed << std::endl;
    if (elapsed < 2000000L || elapsed > 2300000L )
    {
        std::cout << "About 2 ms should have gone away. But it's " << elapsed << std::endl;
        return false;
    }

    if (VERBOSE)
        std::cout << "Please wait 1 more ms..." << std::endl;
    usleep(1000); // 1 ms

    elapsed = timer.elapsed();
    if (VERBOSE)
        std::cout << "Elapsed: " << elapsed << std::endl;
    if (elapsed < 3000000L || elapsed > 3900000L )
    {
        std::cout << "About 3 ms should have gone away. But it's " << elapsed << std::endl;
        return false;
    }
    return true;
}

int main(int argc, char *argv[])
{
    if (! check_timer())
        return 1;
    return 0;
}

