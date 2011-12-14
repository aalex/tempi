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
 * The Recorder class.
 */
#ifndef __TEMPI_RECORDER_H__
#define __TEMPI_RECORDER_H__

#include <boost/any.hpp>
#include "tempi/sharedptr.h"
#include "tempi/timeposition.h"
#include "tempi/sampler/region.h"

namespace tempi {
namespace sampler {

/**
 * A Recorder records events to a Region.
 */
class Recorder
{
    public:
        typedef std::tr1::shared_ptr<Recorder> ptr;
        Recorder(Region::ptr region);
        void setRegion(Region::ptr region);
        Region::ptr getRegion();
        /**
         * Starts recording at the beginning of its Region.
         */
        void start();
        /**
         * Starts recording at the given position.
         */
        void start(TimePosition position);
        /**
         * Clears the Region.
         */
        void clear();
        /**
         * Stops recording and sets the duration of its Region if not already greater than current time position.
         */
        void stop();
        /**
         * Sets the position of the write head.
         */
        bool setPosition(TimePosition position);
        /**
         * Adds an event to its Region at the time specified by its internal timer.
         */
        void add(Message value);
    private:
        Timer timer_;
        Region::ptr region_;
};

} // end of namespace
} // end of namespace

#endif // ifndef

