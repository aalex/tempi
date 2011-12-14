/*
 * Copyright (C) 2011 Alexandre Quessy
 * 
 * This file is part of Tempi.
 * 
 * Tempi is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Tempi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Tempi.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "tempi/sampler/simpleplayback.h"
#include "tempi/sampler/region.h"
#include "tempi/sampler/player.h"
#include <iostream>

namespace tempi {
namespace sampler {

bool SimplePlayback::read(Player &player, Message &result)
{
    Region::ptr region = player.getRegion();
    TimePosition duration = region->getDuration();
    TimePosition elapsed = player.getTimer()->elapsed();
    elapsed *= player.getSpeed();
    if (duration == 0L)
        return false;
    else
    {
        TimePosition cursor = elapsed;
        if (elapsed > duration)
        {
            cursor = 0L; //elapsed % duration;
            // TODO: make sure we play the last event in the region
            //previous_timeposition_played_ = 0L; //reset the previous message sent
        }
        player.setPosition(cursor);
        Region::Event event;
        bool ok = region->getClosestBefore(cursor, event);
        if (! ok)
        {
            //std::cout << "no message to output\n";
            return false; // no message to output
        }

        TimePosition resultPosition = event.get<0>();
        if (resultPosition != previous_timeposition_played_)
        {
            previous_timeposition_played_ = resultPosition;
            result = event.get<1>();
            std::cout << resultPosition << " :" << result << std::endl;
            return true;
        }
        else
        {
            //std::cout << "same message as previous one.\n";
            return false; // same message as previous one
        }
    }
}

} // end of namespace
} // end of namespace

