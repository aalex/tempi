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

#include <sys/time.h>
// TODO: use boost::chrono
//#include <ctime>
//#include <boost/chrono/chrono.hpp>
//#include <boost/chrono/high_resolution_clock.hpp>
#include "tempi/timer.h"

//#define BOOST_CHRONO_INLINED

namespace tempi
{

//typedef boost::chrono::nanoseconds TimePoint;

Timer::Timer() :
    start_tick_(0L)
{
    reset();
}

TimePosition Timer::elapsed()
{
    return now() - start_tick_;
}

void Timer::reset()
{
    start_tick_ = now();
}

TimePosition Timer::now()
{
    struct timeval tv;
    gettimeofday(&tv, 0);
    return ((TimePosition) tv.tv_sec) * 1000000000 + (TimePosition) tv.tv_usec * 1000;
    //return std::clock() * (1000000000L / CLOCKS_PER_SEC);
}

bool Timer::setPosition(TimePosition position)
{
    // FIXME: is this always valid?
    start_tick_ = now() - position;
    return true;
}

};

