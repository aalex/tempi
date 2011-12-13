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
 * The Track class.
 */
#ifndef __TEMPI_TRACK_H__
#define __TEMPI_TRACK_H__

#include <boost/tuple/tuple.hpp>
#include <vector>
#include "tempi/timer.h"
#include "tempi/message.h"
#include "tempi/sharedptr.h"

namespace tempi
{

/**
 * A Track contains timed Message objects. 
 */
class Track
{
    public:
        typedef std::tr1::shared_ptr<Track> ptr;
        Track();
        /**
         * Clears this Track.
         */
        void reset();
        /**
         * Returns the TimePosition of the last event in this Track.
         */
        TimePosition getDuration();
        /**
         * Adds an event to this track.
         */
        void add(TimePosition time, Message message);
        /** TODO ostream << thing*/
        //void print();
        // TODO eraseBetween(TimePosition from, TimePosition to);
        unsigned int numberOfEvents();
        Message *getFirst();
        Message *getLast();
        Message *getClosest(TimePosition target);
        bool getClosestBefore(TimePosition target, TimePosition &resultPosition, Message **resultMessage);
    private:
        typedef boost::tuple<TimePosition, Message> Event;
        typedef std::vector<Event>::iterator EventVecIter;
        typedef std::vector<Event> EventVec;

        std::vector<Event> events_;
        /**
         * Useful to insert an event before what it returns.
         */
        EventVecIter getIteratorAfter(TimePosition target);
};

} // end of namespace

#endif // ifndef

