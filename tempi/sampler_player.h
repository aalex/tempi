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
 * The Player class.
 */
#ifndef __TEMPI_PLAYER_H__
#define __TEMPI_PLAYER_H__

#include "tempi/sharedptr.h"
#include "tempi/timer.h"
#include "tempi/sampler_region.h"
#include "tempi/sampler_playback.h"
#include "tempi/message.h"
#include "tempi/timeposition.h"

namespace tempi {
namespace sampler {

/**
 * A Player plays events from a Region.
 */
class Player
{
    public:
        typedef std::tr1::shared_ptr<Player> ptr;
        Player(Region::ptr region);
        void setRegion(Region::ptr region);
        /**
         * Returns the Region played by this Timer.
         */
        Region::ptr getRegion();
        /**
         * Returns this Timer's internal Timer.
         * Never free this pointer.
         */
        Timer *getTimer();
        /**
         * Resets the internal Timer of this Player.
         */
        void reset();
        bool setPosition(TimePosition position);
        /**
         * Changes the speed of this Player. Default is 1.0.
         */
        void setSpeed(double factor);
        double getSpeed() const;
        /**
         * Reads events from the track at the time pointed to by the internal Timer.
         * Returns false if none is found.
         */
        bool read(std::vector<Message> &result);
        // TODO: add a signal
        
        /**
         * Sets the Playback mode.
         * You can create a new instance of a Playback class and not care about deleting it.
         * The memory will be freed by the Player's shared_ptr.
         */
        void setPlaybackMode(Playback *playback);

        Playback *getPlaybackMode();
        // TODO: add start stop
    private:
        Timer timer_;
        Region::ptr region_;
        double speed_;
        std::tr1::shared_ptr<Playback> playback_;
};

} // end of namespace
} // end of namespace

#endif // ifndef
