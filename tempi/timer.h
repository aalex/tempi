/*
 * Copyright (C) 2011 Alexandre Quessy
 *
 * This file is part of Tempi.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software ither version 3 of the License, or
 * (at your option) any later version.
 * 
 * Tempi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with Tempi.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

/**
 * @file
 * The Timer class.
 */
#ifndef __TEMPI_TIMER_H__
#define __TEMPI_TIMER_H__

#include "tempi/timeposition.h"

namespace tempi
{

/**
 * Nanoseconds timer.
 * Inspired from osg::Timer from OpenSceneGraph.
 */
class Timer
{
    public:
        Timer();
        /**
         * Returns how many nanoseconds elapsed since start.
         */
        TimePosition elapsed();
        /**
         * Sets the start time to now.
         */
        void reset();
        /**
         * Returns the time now.
         */
        TimePosition now();
        /**
         * Sets the time now.
         */
        bool setPosition(TimePosition position);
    private:
        TimePosition start_tick_;
};

} // end of namespace

#endif // ifndef

