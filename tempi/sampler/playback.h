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

/**
 * @file
 * The Playback class.
 */
#ifndef __TEMPI_PLAYBACK_H__
#define __TEMPI_PLAYBACK_H__

#include "tempi/message.h"

namespace tempi {
namespace sampler {

class Player; // forward declaration

/**
 * A Playback manages a Player's playback.
 */
class Playback
{
    public:
        /**
         * Reads an event from a Track at the time pointed to by a Player's  internal Timer according to how it should do it.
         * Returns 0 if none is found.
         * Never free this pointer.
         */
        virtual bool read(Player &player, Message &result) = 0;
        virtual ~Playback() {}
};

} // end of namespace
} // end of namespace

#endif // ifndef

