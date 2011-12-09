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
        TimePosition cursor = elapsed % duration;
        player.setPosition(cursor);

        boost::tuple<TimePosition, Message *> current = track->getClosestBefore(cursor);
        if (current.get<0>() != previous_timeposition_played_)
        {
            previous_timeposition_played_ = current.get<0>();
            return current.get<1>();
        }
        else
            return 0;
    }
}

} // end of namespace

