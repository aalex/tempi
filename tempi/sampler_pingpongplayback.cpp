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
// #include "tempi/sampler_pingpongplayback.h"
// #include "tempi/sampler_track.h"
// #include "tempi/sampler_player.h"
// #include <iostream>
// 
// namespace tempi
// {
// 
// PingPongPlayback::PingPongPlayback() :
//     direction_is_forward_(true)
// {
//     // pass
// }
// 
// Message *PingPongPlayback::read(Player &player)
// {
//     // FIXME: should the Timer belong to the Playback?
//     Track *track = player.getTrack();
//     TimePosition duration = track->getDuration();
//     TimePosition elapsed = player.getTimer()->elapsed();
//     elapsed *= player.getSpeed();
//     if (duration == 0L)
//     {
//         //std::cout << "Read first point." << std::endl;
//         return track->getFirst();
//     }
//     else if (duration == elapsed)
//     {
//         //std::cout << "Read last point." << std::endl;
//         // FIXME: should we move the cursor to the beginning?
//         return  track->getLast();
//     }
//     else
//     {
//         if (elapsed >= duration)
//         {
//             direction_is_forward_ = ! direction_is_forward_;
//             player.setPosition(0L);
//             elapsed = 0L;
//         }
//         TimePosition cursor = elapsed % duration;
//         player.setPosition(cursor);
//         if (direction_is_forward_)
//             return track->getClosest(cursor);
//         else
//             return track->getClosest(duration - cursor);
//     }
// }
// 
// } // end of namespace
// 
