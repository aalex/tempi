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

#ifndef __TEMPI_RECORDER_H__
#define __TEMPI_RECORDER_H__

#include <boost/any.hpp>
#include "sampler.h" // TODO: change for tempi/track.h

namespace tempi
{

typedef Sampler Track; // FIXME: get rid of this

/**
 * A Recorder records events to a Track.
 */
class Recorder
{
    public:
        Recorder(Track *track);
        void setTrack(Track *track);
        Track *getTrack();
        /**
         * Resets the internal timer of this Recorder.
         */
        void reset();
        /**
         * Adds an event to the track now.
         */
        void add(boost::any value);
    private:
        Timer timer_;
        Track *track_;
};

} // end of namespace

#endif // ifndef

