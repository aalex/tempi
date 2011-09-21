#include <iostream>
#include <unistd.h>
#include "tempi/timer.h"

using namespace tempi;

static const bool VERBOSE = false;

bool check_simple()
{
    Timer timer;
    // TODO: make it faster than 1 second
    std::cout << "Please wait 1 second..." << std::endl;
    sleep(1); // 1 second

    TimePosition elapsed = timer.elapsed();
    if (VERBOSE)
        std::cout << "Elapsed: " << elapsed << std::endl;
    if (elapsed < 1000000000L || elapsed > 1100000000L )
    {
        std::cout << "About 1 second should have gone away. But it's " << elapsed << std::endl;
        return false;
    }

    timer.setPosition(2000000000L);
    elapsed = timer.elapsed();
    if (VERBOSE)
        std::cout << "Elapsed: " << elapsed << std::endl;
    if (elapsed < 2000000000L || elapsed > 2100000000L )
    {
        std::cout << "About 2 second should have gone away. But it's " << elapsed << std::endl;
        return false;
    }

    std::cout << "Please wait 1 more second..." << std::endl;
    sleep(1); // 1 second

    elapsed = timer.elapsed();
    if (VERBOSE)
        std::cout << "Elapsed: " << elapsed << std::endl;
    if (elapsed < 3000000000L || elapsed > 3100000000L )
    {
        std::cout << "About 3 second should have gone away. But it's " << elapsed << std::endl;
        return false;
    }
    return true;
}

int main(int argc, char *argv[])
{
    if (not check_simple())
        return 1;
    return 0;
}

