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

#ifndef __TEMPI_SAMPLER_H__
#define __TEMPI_SAMPLER_H__

#include <boost/any.hpp>
#include <boost/tuple/tuple.hpp>
#include <vector>
#include "tempi/timer.h"

namespace tempi
{

typedef TimePosition TimeStamp;
typedef boost::tuple<TimeStamp, boost::any> Event;
typedef std::vector<Event> EventVec;
typedef EventVec::iterator EventVecIter;

class Sampler
{
    public:
        Sampler();
        /**
         * Clears this Track.
         */
        void reset();
        /**
         * Returns the TimeStamp of the last event in this Track.
         */
        TimeStamp getDuration();
        /**
         * Adds an event to this track.
         * FIXME: to be deprecated.
         */
        void add(boost::any value);
        /**
         * Adds an event to this track.
         */
        void add(TimeStamp time, boost::any value);
        /**
         * Reads an event.
         * Returns 0 if none is found.
         * Never free this pointer.
         */
        boost::any *readLoop();
        void print();
    private:
        Timer writer_timer_;
        Timer reader_timer_;
        EventVec events_;
        EventVecIter getClosest(TimeStamp target);
};

} // end of namespace

#endif // ifndef

