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

#include <boost/tuple/tuple.hpp>
#include <vector>
#include "tempi/timer.h"
// TODO: use boost::any
//#include <boost/any.hpp>

namespace tempi
{

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
        void print();
    private:
        tempi::Timer timer_;
        PointVec points_;
        PointVecIter getClosest(TimePoint point);
};

} // end of namespace

#endif // ifndef

