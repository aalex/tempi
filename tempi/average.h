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
 * The Average class.
 */
#ifndef __TEMPI_AVERAGE_H__
#define __TEMPI_AVERAGE_H__

#include <vector>

namespace tempi
{

/**
 * Average smooths changes in a stream of numbers by outputting the average of the last n numbers.
 */
template <typename T>
class Average
{
    public:
        /**
         * Constructor.
         */
        Average(unsigned int size) : 
            first_in_(true)
        {
            setSize(size);
        }
        /**
         * Sets the size of the set.
         */
        bool setSize(unsigned int size)
        {
            size_ = size;
            if (size == 0)
            {
                last_n_.resize(1);
                return false;
            }
            last_n_.resize(size);
            return true;
        }
        /**
         * Returns the size of the set.
         */
        unsigned int getSize()
        {
            return size_;
        }
        /**
         * Adds a number to the se of numbers.
         * Returns the average in the set.
         */
        T add(T value)
        {
            if (first_in_)
            {
                for (int i = 0; i < size_; ++i)
                    last_n_[i] = value;
                first_in_ = false;
            }
            else
                last_n_[index_] = value;
            index_ = (index_ + 1) % size_;
            return getAverage();
        }
        /**
         * Returns the average in the set.
         */
        T getAverage() const
        {
            T ret = 0;
            for (int i = 0; i < size_; ++i)
                ret += last_n_[i];
            return ret / size_;
        }
    private:
        unsigned int size_;
        std::vector<T> last_n_;
        unsigned int index_;
        bool first_in_;
};

} // end of namespace

#endif // ifndef

