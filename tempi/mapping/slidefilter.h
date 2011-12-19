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
 * The SlideFilter class.
 */
#ifndef __TEMPI_SLIDEFILTER_H__
#define __TEMPI_SLIDEFILTER_H__

#include "tempi/mapping/filter.h"

namespace tempi { namespace mapping {

/**
 * The slide filter smooths changes in streams of numbers.
 */
class SlideFilter : public Filter
{
    public:
        SlideFilter();
        bool setSlide(double slide);
    private:
        // Inherited from Filter:
        virtual Message filter(const Message &message);
        double slide(double value);
        double slide_;
        double last_out_;
        bool had_some_in_yet_;
};

} // end of namespace
} // end of namespace

#endif // ifndef

