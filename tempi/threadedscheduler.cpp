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

#include "tempi/threadedscheduler.h"
#include <iostream>

namespace tempi {

ScopedLock::ScopedLock(boost::mutex &mutex) :
    lock_(mutex)
{
}

ThreadedScheduler::ThreadedScheduler() :
    Scheduler(),
    is_running_(false),
    should_be_running_(false),
    max_messages_per_tick_(50)
{
    // the thread is not-a-thread until we call start()
}

//// TODO: add graph ID argument
//void ThreadedScheduler::doSendMessage(const Message &message)
//{
//    queue_.push(message);
//}

void ThreadedScheduler::start(unsigned int sleep_interval_ms)
{
    thread_ = boost::thread(&ThreadedScheduler::run, this, sleep_interval_ms);
    is_running_ = true;
    should_be_running_ = true;
}

bool ThreadedScheduler::isRunning() const
{
    return is_running_;
}

void ThreadedScheduler::stop()
{
    should_be_running_ = false;
    join();
}

void ThreadedScheduler::join()
{
    thread_.join();
    is_running_ = false;
}

void ThreadedScheduler::run(unsigned int sleep_interval_ms)
{
    float ms = sleep_interval_ms;
    boost::posix_time::milliseconds sleepTime(ms);
    std::cout << "ThreadedScheduler: started, will work every "
        << ms << "ms"
        << std::endl;
    while (should_be_running_)
    {
        tick();
        boost::this_thread::sleep(sleepTime);
    }
    std::cout << "ThreadedScheduler: completed" << std::endl;
}

//void ThreadedScheduler::handlePoppedMessage(const Message &message)
//{
//    sendToAllGraphs(message);
//}

void ThreadedScheduler::tick()
{
    //unsigned int num_popped = 0;
    //bool some_todo = true;
    //while (some_todo)
    //{
    //    Message message;
    //    some_todo = queue_.try_pop(message);
    //    if (some_todo)
    //    {
    //        handlePoppedMessage(message);
    //    }
    //    ++ num_popped;
    //    if (num_popped >= max_messages_per_tick_)
    //        some_todo = false;
    //}
    ScopedLock::ptr lock = acquireLock();
    tickGraphs();
    //std::cout << "ThreadedScheduler" << __FUNCTION__ << std::endl;
}

void ThreadedScheduler::sleepThisThread(float ms)
{
    boost::posix_time::milliseconds sleepTime(ms);
    boost::this_thread::sleep(sleepTime);
}

} // end of namespace


