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

#include <boost/any.hpp>
#include <boost/tuple/tuple.hpp>
#include <vector>
#include "tempi/timer.h"

namespace tempi
{

typedef TimePosition TimeStamp;
typedef boost::tuple<TimeStamp, boost::any> Point;
typedef std::vector<Point> PointVec;
typedef PointVec::iterator PointVecIter;

class Sampler
{
    public:
        Sampler();
        void reset();
        TimeStamp getDuration();
        void add(boost::any value);
        boost::any *readLoop();
        void print();
    private:
        Timer writer_timer_;
        Timer reader_timer_;
        PointVec points_;
        PointVecIter getClosest(TimeStamp target);
};

} // end of namespace

#endif // ifndef

