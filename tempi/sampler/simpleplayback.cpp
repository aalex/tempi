#include "tempi/sampler/simpleplayback.h"
#include "tempi/sampler/track.h"
#include "tempi/sampler/player.h"
#include <iostream>

namespace tempi
{

Message *SimplePlayback::read(Player &player)
{
    Track *track = player.getTrack();
    TimePosition duration = track->getDuration();
    TimePosition elapsed = player.getTimer()->elapsed();
    elapsed *= player.getSpeed();
    if (duration == 0L)
        return 0;
    else if (duration == elapsed)
        return track->getLast();
    else
    {
        TimePosition cursor = elapsed;
        if (elapsed > duration)
        {
            cursor = elapsed % duration;
            previous_timeposition_played_ = 0L; //reset the previous message sent
        }
        player.setPosition(cursor);

        TimePosition resultPosition;
        Message *resultMessage;
        bool ok = track->getClosestBefore(cursor, resultPosition, &resultMessage);
        if (! ok)
        {
            //std::cout << "no message to output\n";
            return 0; // no message to output
        }
        if (resultPosition != previous_timeposition_played_)
        {
            previous_timeposition_played_ = resultPosition;
            if (resultMessage == 0)
            {
                //std::cerr << "INVALID MESSAGE POINTER in SimplePlayback::read\n";
                return 0;
            }
            else
                return resultMessage;
        }
        else
        {
            //std::cout << "same message as previous one.\n";
            return 0; // same message as previous one
        }
    }
}

} // end of namespace

