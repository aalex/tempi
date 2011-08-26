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
#include "tempi/timer.h"

namespace tempi
{

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
}

};

