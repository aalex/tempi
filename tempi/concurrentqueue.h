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
 * The ConcurrentQueue class
 */

// Written by Anthony Williams, 2008
// Public domain based on his comment:
// "Yes, you can just copy the code presented here and use it for whatever you 
// like. There won't be any licensing issues. I'm glad you find it helpful."
// Reference:
// http://www.justsoftwaresolutions.co.uk/threading/implementing-a-thread-safe-queue-using-condition-variables.html

#ifndef _TEMPI_CONCURRENT_QUEUE_H_
#define _TEMPI_CONCURRENT_QUEUE_H_

#include <queue>
#include <boost/thread/thread.hpp>
#include <boost/thread/condition.hpp>

namespace tempi {

/**
 * Asynchronous FIFO used to communicate between threads.
 * FIFO stands for "First In First Out".
 */
template<typename Data>
class ConcurrentQueue
{
    public:
        ConcurrentQueue() :
            queue_(),
            mutex_(),
            condition_()
        {}

        /**
         * Adds a message to the FIFO.
         */
        void push(Data const& data)
        {
            boost::mutex::scoped_lock lock(mutex_);
            queue_.push(data);
            lock.unlock();
            condition_.notify_one();
        }

        /**
         * Clears the contents of the queue.
         */
        void clear()
        {
            boost::mutex::scoped_lock lock(mutex_);
            while (! queue_.empty())
                queue_.pop();
        }

        /**
         * Checks whether the FIFO is empty or not.
         */
        bool empty() const
        {
            boost::mutex::scoped_lock lock(mutex_);
            return queue_.empty();
        }

        /**
         * Tries to read and remove a message from the FIFO.
         */
        bool try_pop(Data& popped_value)
        {
            boost::mutex::scoped_lock lock(mutex_);
            if (queue_.empty())
            {
                return false;
            }

            popped_value = queue_.front();
            queue_.pop();
            return true;
        }

        /**
         * Waits until it can read and remove a message from the FIFO.
         */
        void wait_and_pop(Data& popped_value)
        {
            boost::mutex::scoped_lock lock(mutex_);
            while (queue_.empty())
            {
                condition_.wait(lock);
            }

            popped_value = queue_.front();
            queue_.pop();
        }
    private:
        std::queue<Data> queue_;
        mutable boost::mutex mutex_;
        boost::condition_variable condition_;
};

} // end of namespace

#endif //ifndef

