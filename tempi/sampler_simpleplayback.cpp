/*
 * Copyright (C) 2011 Alexandre Quessy
 *
 * This file is part of Tempi.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software ither version 3 of the License, or
 * (at your option) any later version.
 * 
 * Tempi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with Tempi.  If not, see
 * <http://www.gnu.org/licenses/>.
 */
#include "tempi/sampler_simpleplayback.h"
#include "tempi/sampler_region.h"
#include "tempi/sampler_player.h"
#include <iostream>

namespace tempi {
namespace sampler {

bool SimplePlayback::read(Player &player, std::vector<Message> &result)
{
    bool ret = false;
    Region::ptr region = player.getRegion();
    TimePosition duration = region->getDuration();
    TimePosition now = player.getTimer()->elapsed();
    now *= player.getSpeed(); // FIXME: might be wrong when speed != 1.0
    if (now > duration) // check for looping:
    {
        if (duration == 0L) // make sure we don't divide by 0
            now = 0L;
        else
            now = (now % duration);
        player.setPosition(now);
    }
    std::vector<Region::Event> events;
    if (previous_timeposition_played_ <= now)
        region->getRange(previous_timeposition_played_, now, events);
    else
    {
        region->getRange(previous_timeposition_played_, duration, events);
        region->getRange(0L, now, events, true);
    }
    if (events.size() > 0)
        ret = true;
    std::vector<Region::Event>::const_iterator iter;
    for (iter = events.begin(); iter != events.end(); ++iter)
        result.push_back((*iter).get<1>());
    previous_timeposition_played_ = now;
    return ret;
}


} // end of namespace
} // end of namespace

