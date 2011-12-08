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

#ifndef __TEMPI_THREADEDSCHEDULER_H__
#define __TEMPI_THREADEDSCHEDULER_H__

#include "tempi/concurrentqueue.h"
#include "tempi/scheduler.h"
#include "tempi/message.h"
#include "tempi/sharedptr.h"
#include <boost/thread.hpp>
#include <boost/thread/locks.hpp>

namespace tempi {

// // TODO:
// class SynchronousScheduler : public Scheduler
// {
//     public:
//         virtual bool isRunning() const;
//         virtual void sendMessage(const Message &message);
// };

// TODO write Message::prepend(types, ...);

/**
 * Scoped lock for a mutex.
 */
class ScopedLock
{
    public:
        typedef std::tr1::shared_ptr<ScopedLock> ptr;
        ScopedLock(boost::mutex &mutex);
    private:
        boost::lock_guard<boost::mutex> lock_;
};

class ThreadedScheduler : public Scheduler
{
    public:
        typedef std::tr1::shared_ptr<ThreadedScheduler> ptr;
        ThreadedScheduler();
        /**
         * Starts the thread.
         */
        void start(unsigned int sleep_interval_ms);
        virtual bool isRunning() const;
        /**
         * Stops the thread.
         * Blocks until the thread joins.
         */
        void stop();
        /**
         * Makes the thread from which you call it sleep.
         * Usually, you might call this from the main thread.
         */
        void sleepThisThread(float ms);
        /**
         * Acquires a lock for a mutex and returns a scoped lock, which
         * frees that mutex when deleted.
         * Might block forever if a lock has already been acquired.
         * Users are responsible for acquiring a lock before altering graphs, and the like.
         */
        ScopedLock::ptr acquireLock()
        {
            return ScopedLock::ptr(new ScopedLock(mutex_));
        }
    private:
        bool is_running_;
        bool should_be_running_;
        unsigned int max_messages_per_tick_;
        boost::thread thread_;
        ConcurrentQueue<Message> queue_;
        boost::mutex mutex_;

        void join();
        // TODO: replace sleep_interval by dynamically-specified microseconds, sleeping waiting until it's elapsed on each iteration, instead of just sleeping a fixed duration.
        void run(unsigned int sleep_interval_ms);
        virtual void handlePoppedMessage(const Message &message);
        void tick();
        virtual void doSendMessage(const Message &message);
};

} // end of namespace

#endif // ifndef

