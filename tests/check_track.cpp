#include <iostream>
#include "tempi/tempi.h"

using namespace tempi;

bool check_simple()
{
    Track track;
    if (track.getDuration() != 0L)
    {
        std::cout << "Duration should be 0 ns." << std::endl;
        return false;
    }

    track.add(1L, types::_f(0.0f));
    if (track.getDuration() != 1L)
    {
        std::cout << "Duration should be 1 ns." << std::endl;
        return false;
    }

    track.add(2L, types::_f(0.0f));
    if (track.getDuration() != 2L)
    {
        std::cout << "Duration should be 2 ns." << std::endl;
        return false;
    }

    if (track.numberOfEvents() != 2)
    {
        std::cout << "Should contain 2 events." << std::endl;
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

