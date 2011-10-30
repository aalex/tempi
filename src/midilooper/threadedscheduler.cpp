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
 * @file The ThreadedScheduler class.
 */

#define WITH_MAIN 1

#include "tempi/concurrentqueue.h"
#include "tempi/graph.h"
#include "tempi/message.h"
#include <iostream>
#include <boost/thread.hpp>

class Scheduler
{
    public:
        virtual bool isRunning() const = 0;
        virtual void pushMessage(const tempi::Message &message) = 0;
};

class ThreadedScheduler : public Scheduler
{
    public:
        ThreadedScheduler() :
            is_running_(false),
            should_be_running_(false)
        {
            // the thread is not-a-thread until we call start()
        }
        virtual void pushMessage(const tempi::Message &message)
        {}

        void start(unsigned int sleep_interval_ms)
        {
            thread_ = boost::thread(&ThreadedScheduler::processQueue, this, sleep_interval_ms);
            is_running_ = true;
            should_be_running_ = true;
        }

        virtual bool isRunning() const
        {
            return is_running_;
        }

        void join()
        {
            thread_.join();
            is_running_ = false;
        }

        void stop()
        {
            should_be_running_ = false;
            join();
        }
        
        void processQueue(unsigned int sleep_interval_ms)
        {
            float ms = sleep_interval_ms;
            boost::posix_time::milliseconds sleepTime(ms);
            std::cout << "ThreadedScheduler: started, will work every "
                      << ms << "ms"
                      << std::endl;
            // We're busy, honest!
            while (should_be_running_)
            {
                doStuff();
                boost::this_thread::sleep(sleepTime);
            }
            std::cout << "ThreadedScheduler: completed" << std::endl;
        }

        virtual void doStuff()
        {
            std::cout << __FUNCTION__ << std::endl;

        }
    private:
        bool is_running_;
        bool should_be_running_;
        boost::thread thread_;
};

#ifdef WITH_MAIN

int main(int argc, char* argv[])
{
    std::cout << "main: startup" << std::endl;
    ThreadedScheduler worker;
    worker.start(5); // ms
    std::cout << "main: waiting for thread" << std::endl;
    std::cout << "main: sleep." << std::endl;
    boost::posix_time::milliseconds sleepTime(25.0f);
    boost::this_thread::sleep(sleepTime);
    worker.stop();
    std::cout << "main: done" << std::endl;
    return 0;
}

#endif // WITH_MAIN

