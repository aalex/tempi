/*
 * Copyright (C) 2011 Alexandre Quessy
 * Copyright (C) 2011 Michal Seta
 * Copyright (C) 2012 Nicolas Bouillot
 *
 * This file is part of Tempi.
 *
 * This program is free software: you can redistribute it and/or
 * modify it under the terms of, either version 3 of the License, or
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
         * Reads events from a Track up to the time pointed to by a Player's internal Timer according to how it should do it.
         */
        virtual bool read(Player &player, std::vector<Message> &result) = 0;
        virtual ~Playback() {}
};

} // end of namespace
} // end of namespace

#endif // ifndef

