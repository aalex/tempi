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
 * The Player class.
 */
#ifndef __TEMPI_PLAYER_H__
#define __TEMPI_PLAYER_H__

#include "tempi/sharedptr.h"
#include "tempi/timer.h"
#include "tempi/sampler/track.h"
#include "tempi/sampler/playback.h"
#include "tempi/message.h"
#include "tempi/timeposition.h"

namespace tempi
{

/**
 * A Player plays events from a Track.
 */
class Player
{
    public:
        Player(Track *track);
        void setTrack(Track *track);
        /**
         * Returns the Track played by this Timer.
         */
        Track *getTrack();
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
         * Reads an event from the track at the time pointed to by the internal Timer.
         * Returns 0 if none is found.
         * Never free this pointer.
         */
        Message *read();
        // TODO: add a signal
        
        /**
         * Sets the Playback mode.
         * You can create a new instance of a Playback class and not care about deleting it.
         * The memory will be freed by the Player's shared_ptr.
         */
        void setPlaybackMode(Playback *playback);

        Playback *getPlaybackMode();
    private:
        Timer timer_;
        Track *track_;
        double speed_;
        std::tr1::shared_ptr<Playback> playback_;
};

} // end of namespace

#endif // ifndef
