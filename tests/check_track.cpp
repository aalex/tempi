#include <iostream>
#include "tempi/sampler/track.h"

using namespace tempi;

bool check_track()
{
    Track track;
    if (track.getDuration() != 0L)
    {
        std::cout << "Duration should be 0 ns." << std::endl;
        return false;
    }

    Message m;
    m.appendFloat(0.0f);
    track.add(1L, m);
    if (track.getDuration() != 1L)
    {
        std::cout << "Duration should be 1 ns." << std::endl;
        return false;
    }

    track.add(2L, m);
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
    if (! check_track())
        return 1;
    return 0;
}

