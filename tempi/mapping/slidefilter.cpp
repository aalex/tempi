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
#include "tempi/mapping/slidefilter.h"
#include "tempi/sharedptr.h"
#include <iostream>

namespace tempi { namespace mapping {

SlideFilter::SlideFilter() :
    Filter(),
    slide_(1.0),
    last_out_(0.0),
    had_some_in_yet_(false)
{
}

Message SlideFilter::filter(const Message &message)
{
    if (message.typesMatch("d"))
    {
        Message ret = message;
        double value = message.getDouble(0);
        value = slide(value);
        ret.setDouble(0, value);
        return ret;
    }
    else
        return message;
}

double SlideFilter::slide(double value)
{
    if (! had_some_in_yet_)
    {
        last_out_ = value;
        had_some_in_yet_ = true;
        return last_out_;
    }
    //A given sample output from slide is equal to the last value plus the difference between the last value and the input divided by the slide value. Given a slide value of 1, the output will therefore always equal the input. Given a slide value of 10, the output will only change 1/10th as quickly as the input. This can be particularly useful for lowpass filtering or envelope following. 
    last_out_ = last_out_ + (last_out_ - (value / slide_));
    return last_out_;
}

bool SlideFilter::setSlide(double slide)
{
    if (slide >= 0.0)
    {
        slide_ = slide;
        return true;
    }
    else
        return false;
}

} // end of namespace
} // end of namespace

