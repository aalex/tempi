// #include "tempi/sampler/loopingplayback.h"
// #include "tempi/sampler/track.h"
// #include "tempi/sampler/player.h"
// #include <iostream>
// 
// namespace tempi {
// namespace sampler {
// 
// Message *LoopingPlayback::read(Player &player)
// {
//     Region::ptr region = player.getRegion();
//     TimePosition duration = region->getDuration();
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
//         //std::cout << "duration: " << getDuration() << std::endl;
//         // FIXME: using the modulo here should not be mandatory
//         TimePosition cursor = elapsed % duration;
//         player.setPosition(cursor);
//         //std::cout << "elapsed modulo duration: " << cursor << std::endl;
//         return track->getClosest(cursor);
//     }
// }
// 
// } // end of namespace
// } // end of namespace
// 
