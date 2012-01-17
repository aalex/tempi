/*
 * Copyright (C) 2011 Alexandre Quessy
 * Copyright (C) 2011 Michal Seta
 * Copyright (C) 2012 Nicolas Bouillot
 *
 * This file is part of Tempi.
 *
 * This program is free software: you can redistribute it and/or
 * modify it under the terms of, either version 3 of the License, or
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
 * The Timer class.
 */
#ifndef __TEMPI_TIMER_H__
#define __TEMPI_TIMER_H__

#include "tempi/timeposition.h"

namespace tempi
{

/**
 * Nanoseconds timer.
 * Inspired from osg::Timer from OpenSceneGraph.
 */
class Timer
{
    public:
        Timer();
        TimePosition elapsed();
        void reset();
        TimePosition now();
        bool setPosition(TimePosition position);
    private:
        TimePosition start_tick_;
};

} // end of namespace

#endif // ifndef

