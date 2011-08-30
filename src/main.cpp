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

#include <boost/any.hpp>
//#include <boost/chrono/chrono.hpp>
//#include <boost/chrono/high_resolution_clock.hpp>
#include <boost/tuple/tuple.hpp>
#include <clutter/clutter.h>
#include <iostream>
#include <string>
#include <vector>
#include "tempi/tempi.h"
#include "tempi/timer.h"

//#define BOOST_CHRONO_INLINED
//typedef boost::chrono::nanoseconds TimePoint;
typedef boost::tuple<double, double> ff;
typedef tempi::TimePosition TimePoint;
typedef boost::tuple<TimePoint, ff> Point;
typedef std::vector<Point> PointVec;
typedef PointVec::iterator PointVecIter;

class Sampler
{
    public:
        Sampler();
        void reset();
        TimePoint getDuration();
        void add(double x, double y);
        ff readLoop();
    private:
        tempi::Timer timer_;
        PointVec points_;
        PointVecIter getClosest(TimePoint point);
};

Sampler::Sampler()
{
    // pass
}

void Sampler::reset()
{
    timer_.reset();
    points_.clear();
}

TimePoint Sampler::getDuration()
{
    if (points_.size() == 0)
        return 0L;
    return ((points_[points_.size() - 1])).get<0>();
}

void Sampler::add(double x, double y)
{
    points_.push_back(Point(timer_.elapsed(), ff(x, y)));
}

ff Sampler::readLoop()
{
    if (points_.size() == 0)
    {
        std::cout << "No point to read." << std::endl;
        return ff(0.0, 0.0);
    }
    else
    {
        TimePoint cursor = timer_.elapsed() % getDuration();
        return (*getClosest(cursor)).get<1>();
    }
}

PointVecIter Sampler::getClosest(TimePoint point)
{
    PointVecIter iter;
    PointVecIter ret;
    ret = points_.begin();
    TimePoint smallest = getDuration();
    
    for (iter = points_.begin(); iter < points_.end(); ++iter)
    {
        TimePoint current = (*iter).get<0>();
        if (point - current < smallest)
        {
            ret = iter;
            smallest = current;
        }
    }
    return ret;
}

int main(int argc, char *argv[])
{
    tempi::hello();
    return 0;
}

