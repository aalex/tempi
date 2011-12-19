// #include "tempi/sampler/pingpongplayback.h"
// #include "tempi/sampler/track.h"
// #include "tempi/sampler/player.h"
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
