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
#include "tempi/averagefilter.h"
#include "tempi/sharedptr.h"
#include "tempi/types.h"
#include "tempi/average.h"
#include <iostream>

namespace tempi
{

static const NUM_AVERAGERS = 5;

AverageFilter::AverageFilter(unsigned int size) :
    Filter(),
    size_(size),
    index_(0)
{
    for (int i = 0; i < NUM_AVERAGERS; ++i)
    {
        averagers_.push_back(std::tr1::shared_ptr<Average<double> >(new Average<double>(size)));
    }
}

AverageFilter::setSize(unsigned int size)
{
    size_ = size;
    typename std::vector<std::tr1::shared_ptr<Average> >::iterator iter;
    for (iter = averagers_.begin(); iter < averagers_.end(); ++iter)
    {
        (*iter).get()->setSize(size_);
    }
}

boost::any AverageFilter::filter(boost::any data)
{
    if (data.type() == typeid(_f))
        updateAverageForIndex<float, _f, 0>(data);
    if (data.type() == typeid(_ff))
        updateAverageForIndex<float, _ff, 1>(data);
    if (data.type() == typeid(_fff))
        updateAverageForIndex<float, _fff, 2>(data);
    if (data.type() == typeid(_ffff))
        updateAverageForIndex<float, _ffff, 3>(data);
    return data; //boost::any(_d(value));
}

} // end of namespace

