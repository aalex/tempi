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
 * The Region class.
 */
#ifndef __TEMPI_SAMPLER_REGION_H__
#define __TEMPI_SAMPLER_REGION_H__

#include <boost/tuple/tuple.hpp>
#include <vector>
#include "tempi/timer.h"
#include "tempi/message.h"
#include "tempi/sharedptr.h"

namespace tempi {
namespace sampler {

/**
 * A Region contains timed Message objects. 
 */
class Region
{
    public:
        typedef std::tr1::shared_ptr<Region> ptr;
        typedef boost::tuple<TimePosition, Message> Event;
        Region();
        /**
         * Clears this Region.
         */
        void clear();
        /**
         * Adds an event to this track.
         */
        void add(TimePosition time, Message message);
        bool setDuration(TimePosition position);
        TimePosition getDuration();
        unsigned int numberOfEvents();
        bool getClosestBefore(TimePosition target, Region::Event &result);
    private:
        typedef std::vector<Event> EventVec;
        typedef EventVec::iterator EventVecIter;
        EventVec events_;
        TimePosition duration_;
        /**
         * Useful to insert an event before what it returns.
         */
        EventVecIter getIteratorAfter(TimePosition target);
        //TODO: TimePosition offset_;
        //TODO: std::string track_name_;
};

} // end of namespace
} // end of namespace

#endif // ifndef

