#include "tempi/loopingplayback.h"
#include "tempi/track.h"
#include "tempi/player.h"
#include <iostream>

namespace tempi
{

boost::any *LoopingPlayback::read(Player &player)
{
    Track *track = player.getTrack();
    TimeStamp duration = track->getDuration();
    TimeStamp elapsed = player.getTimer()->elapsed();
    elapsed *= player.getSpeed(); // FIXME: speed factor is broken here.
    //std::cout << __FUNCTION__ << std::endl;
    if (track->numberOfEvents() == 0)
    {
        //std::cout << "No point to read." << std::endl;
        return 0;
    }
    else if (duration == 0L)
    {
        //std::cout << "Read first point." << std::endl;
        return track->getFirst();
    }
    else if (duration == elapsed)
    {
        //std::cout << "Read last point." << std::endl;
        return  track->getLast();
    }
    else
    {
        //std::cout << "duration: " << getDuration() << std::endl;
        // FIXME: using the modulo here should not be mandatory
        TimeStamp cursor = elapsed % duration;
        player.setPosition(cursor);
        //std::cout << "elapsed modulo duration: " << cursor << std::endl;
        return track->getClosest(cursor);
    }
}

} // end of namespace

