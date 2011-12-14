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
#include "tempi/sampler/region.h"
#include <boost/tuple/tuple_io.hpp>
#include <iostream>
#include <cmath> // for std::abs

namespace tempi {
namespace sampler {

Region::Region()
{
    // pass
}

void Region::clear()
{
    events_.clear();
    duration_ = 0L;
}

TimePosition Region::getDuration()
{
    return duration_;
}

void Region::add(TimePosition position, Message value)
{
    if (getDuration() > position)
        setDuration(position);
    events_.insert(getIteratorAfter(position), Event(position, value));
}

bool Region::setDuration(TimePosition position)
{
    if (events_.size() == 0)
    {
        duration_ = position;
        return true;
    }
    else if (events_[events_.size() - 1].get<0>() < position)
    {
        duration_ = position;
        return true;
    }
    else
        return false;
}

Region::EventVecIter Region::getIteratorAfter(TimePosition target)
{
    EventVecIter iter;
    EventVecIter ret;
    ret = events_.begin();
    TimePosition smallest = duration_;
    bool found_it = false;
    
    for (iter = events_.begin(); iter < events_.end(); ++iter)
    {
        TimePosition pos = (*iter).get<0>();
        //if (target >= pos)
        if (pos >= target)
        {
            ret = iter;
            found_it = true;
            break;
        }
    }
    if (! found_it)
        ret = events_.end();
    return ret;
}

bool Region::getClosestBefore(TimePosition target, Region::Event &result)
{
    if (numberOfEvents() == 0)
        return false;

    EventVecIter iter;
    EventVecIter cursor = events_.begin();
    TimePosition smallest = duration_;
    for (iter = events_.begin(); iter < events_.end(); ++iter)
    {
        TimePosition pos = (*iter).get<0>();
        if (pos <= target)
        {
            TimePosition distance = (TimePosition) std::abs(((double) target) - ((double) pos));
            if (distance < smallest)
            {
                cursor = iter;
                smallest = distance;
            }
        }
    }
    result = (*cursor);
    return true;
}

unsigned int Region::numberOfEvents()
{
    return events_.size();
}

} // end of namespace
} // end of namespace

