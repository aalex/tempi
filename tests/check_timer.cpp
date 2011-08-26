#include <iostream>
#include <unistd.h>
#include "tempi/timer.h"

using namespace tempi;

bool check_simple()
{
    Timer timer;
    // TODO: make it faster than 1 second
    std::cout << "Please wait 1 second..." << std::endl;
    sleep(1); // 1 second

    if (timer.elapsed() < 1000000000L)
    {
        std::cout << "At least one second should have gone away." << std::endl;
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

