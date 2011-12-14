// /*
//  * Copyright (C) 2011 Alexandre Quessy
//  * 
//  * This file is part of Tempi.
//  * 
//  * Tempi is free software: you can redistribute it and/or modify
//  * it under the terms of the GNU General Public License as published by
//  * the Free Software Foundation, either version 3 of the License, or
//  * (at your option) any later version.
//  * 
//  * Tempi is distributed in the hope that it will be useful,
//  * but WITHOUT ANY WARRANTY; without even the implied warranty of
//  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  * GNU General Public License for more details.
//  * 
//  * You should have received a copy of the GNU General Public License
//  * along with Tempi.  If not, see <http://www.gnu.org/licenses/>.
//  */
// 
// /**
//  * @file
//  * The PingPongPlayback class.
//  */
// #ifndef __TEMPI_PINGPONGPLAYBACK_H__
// #define __TEMPI_PINGPONGPLAYBACK_H__
// 
// #include "tempi/sampler/playback.h"
// 
// namespace tempi
// {
// 
// /**
//  * A PingPongPlayback plays a Track from beginning to end, and then from the end to the beginning.
//  */
// class PingPongPlayback : public Playback
// {
//     public:
//         PingPongPlayback();
//         virtual Message *read(Player &player);
//     private:
//         bool direction_is_forward_;
// };
// 
// } // end of namespace
// 
// #endif // ifndef
// 
