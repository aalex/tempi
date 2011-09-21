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
 * The Slide class.
 */
#ifndef __TEMPI_SLIDE_H__
#define __TEMPI_SLIDE_H__

namespace tempi
{

/**
 * FIXME: Not working yet.
 * Slide smooths changes in streams of numbers.
 * A slide of 0 is impossible. If given 0, it will be 1.
 */
class Slide
{
    public:
        Slide();
        /**
         * A given sample output from slide is equal to the last value plus the difference between the last value and the input divided by the slide value. Given a slide value of 1, the output will therefore always equal the input. Given a slide value of 10, the output will only change 1/10th as quickly as the input. This can be particularly useful for lowpass filtering or envelope following. 
         */
        bool setSlide(double slide);
        double getSlide();
        double slide(double value);
    private:
        double slide_;
        double last_out_;
        bool had_some_in_yet_;
};

} // end of namespace

#endif // ifndef

