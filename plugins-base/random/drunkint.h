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
 * The DrunkInt class.
 */
#ifndef __TEMPI_RANDOM_DRUNKINT_H__
#define __TEMPI_RANDOM_DRUNKINT_H__

namespace tempi {
namespace plugins_base {

class DrunkInt
{
    public:
        DrunkInt();
        bool setRange(int from, int to);
        bool setStepRange(int min_step, int max_step);
        bool setCurrent(int value);
        int step();
        int getRangeFrom() const;
        int getRangeTo() const;
        int getCurrent() const;
        int getStepRangeMin() const;
        int getStepRangeMax() const;
    private:
        int from_;
        int to_;
        int current_;
        int min_step_;
        int max_step_;
};

} // end of namespace
} // end of namespace

#endif

