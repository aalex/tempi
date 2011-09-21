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
 * The AverageFilter class.
 */
#ifndef __TEMPI_AVERAGEFILTER_H__
#define __TEMPI_AVERAGEFILTER_H__

#include <boost/any.hpp>
#include <vector>
#include "tempi/sharedptr.h"
#include "tempi/filter.h"
#include "tempi/average.h"
#include "tempi/types.h"

namespace tempi
{

/**
 * The AverageFilter smooths changes in streams of numbers by outputting the average of the last n numbers.
 */
class AverageFilter : public Filter
{
    public:
        AverageFilter(unsigned int size);

        bool setSize(unsigned int size);
        unsigned int getSize();
    private:
        unsigned int size_;
        // Inherited from Filter:
        virtual boost::any filter(boost::any data);
        std::vector<std::tr1::shared_ptr<Average<double> > > averagers_;
        template <typename T, typename U, typename N>
        void updateAverageForIndex(boost::any &data)
        {
            T value = boost::any_cast<U>(data).get<N>();
            Average<double> *average = averagers_[N].get();
            average->add(value0);
            value = (T) average->getAverage();
        }
};

} // end of namespace

#endif // ifndef

