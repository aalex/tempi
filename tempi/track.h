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

#ifndef __TEMPI_TRACK_H__
#define __TEMPI_TRACK_H__

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

/**
 * A Track contains timed Events. 
 * Events are any data, typically a boost::tuple.
 */
class Track
{
    public:
        Track();
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
         */
        void add(TimeStamp time, boost::any value);
        void print();
        // TODO eraseBetween(TimeStamp from, TimeStamp to);
        unsigned int numberOfEvents();
        boost::any *getFirst();
        boost::any *getLast();
        boost::any *getClosest(TimeStamp target);
    private:
        //Timer reader_timer_;
        EventVec events_;
        /**
         * Useful to insert an event before what it returns.
         */
        EventVecIter getIteratorAfter(TimeStamp target);
};

} // end of namespace

#endif // ifndef

