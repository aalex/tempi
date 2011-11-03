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
#include "tempi/internals.h"
#include "tempi/nodefactory.h"
#include "tempi/graph.h"
#include "tempi/message.h"
#include <iostream>
#include <boost/thread.hpp>

using namespace tempi;
/**
 * A Scheduler holds graphs and one can send messages to them, synchronously or asynchronously.
 */
class Scheduler
{
    public:
        Scheduler()
        {
            factory_.reset(new NodeFactory);
            // TODO: do not load internals by default?
            internals::loadInternals(*(factory_.get()));
        }
        virtual bool isRunning() const = 0;
        virtual void sendMessage(const Message &message) = 0;
        bool createGraph(const char *name)
        {
            if (hasGraph(name))
            {
                std::cout << "Already has a Graph named "
                    << name << std::endl;
                return false;
            }
            else
            {
                Graph::ptr graph(new Graph(factory_));
                graphs_[name] = graph;
                return true;
            }
        }
    protected:
        bool sendToAllGraphs(const Message &message)
        {
            bool ret = false;
            std::cout << __FUNCTION__ << std::endl;
            std::map<std::string, Graph::ptr>::const_iterator iter;
            for (iter = graphs_.begin(); iter != graphs_.end(); ++iter)
            {
                if ((*iter).second.get()->handleMessage(message))
                    ret = true;
            }
            return ret;
        }
        Graph::ptr getGraph(const char *name)
        {
            if (! hasGraph(name))
            {
                std::cout << "No Graph named "
                    << name << std::endl;
                return Graph::ptr(); // NULL pointer!!
            }
            return (* (graphs_.find(std::string(name)))).second;

        }
        bool hasGraph(const char *name)
        {
            return graphs_.find(std::string(name)) != graphs_.end();
        }
        NodeFactory::ptr getFactory() const
        {
            return factory_;
        }
        bool tickGraphs()
        {
            if (! isRunning())
                return false;
            std::map<std::string, Graph::ptr>::const_iterator iter;
            for (iter = graphs_.begin(); iter != graphs_.end(); ++iter)
            {
                (*iter).second.get()->tick();
            }
            return true;
        }
    private:
        std::map<std::string, Graph::ptr> graphs_;
        NodeFactory::ptr factory_;
};

// TODO:
class SynchronousScheduler : public Scheduler
{
    public:
        virtual bool isRunning() const;
        virtual void sendMessage(const Message &message);
};

// TODO write Message::prepend(types, ...);
class ThreadedScheduler : public Scheduler
{
    public:
        ThreadedScheduler() :
            Scheduler(),
            is_running_(false),
            should_be_running_(false),
            max_messages_per_tick_(50)
        {
            // the thread is not-a-thread until we call start()
        }

        virtual void sendMessage(const Message &message)
        {
            queue_.push(message);
        }

        void start(unsigned int sleep_interval_ms)
        {
            thread_ = boost::thread(
                &ThreadedScheduler::processQueue, this, 
                sleep_interval_ms);
            is_running_ = true;
            should_be_running_ = true;
        }

        virtual bool isRunning() const
        {
            return is_running_;
        }

        void stop()
        {
            should_be_running_ = false;
            join();
        }
        
    private:
        bool is_running_;
        bool should_be_running_;
        unsigned int max_messages_per_tick_;
        boost::thread thread_;
        ConcurrentQueue<Message> queue_;

        void join()
        {
            thread_.join();
            is_running_ = false;
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
                tick();
                boost::this_thread::sleep(sleepTime);
            }
            std::cout << "ThreadedScheduler: completed" << std::endl;
        }

        virtual void handlePoppedMessage(const Message &message)
        {
            std::cout << "TODO: handle " << message << std::endl;
            sendToAllGraphs(message);
        }

        void tick()
        {
            unsigned int num_popped = 0;
            bool some_todo = true;
            while (some_todo)
            {
                Message message;
                some_todo = queue_.try_pop(message);
                if (some_todo)
                {
                    handlePoppedMessage(message);
                }
                ++ num_popped;
                if (num_popped >= max_messages_per_tick_)
                    some_todo = false;
            }
            tickGraphs();
            //std::cout << "ThreadedScheduler" << __FUNCTION__ << std::endl;
        }
};

#ifdef WITH_MAIN

int main(int argc, char* argv[])
{
    boost::posix_time::milliseconds sleepTime(25.0f);
    std::cout << "main: startup" << std::endl;
    ThreadedScheduler worker;
    worker.createGraph("graph0");
    worker.start(5); // ms
    boost::this_thread::sleep(sleepTime);
    //worker.sendMessage(Message("sif", "hello", 2, 3.14159f));
    worker.sendMessage(Message("ssss", "__tempi__", "addNode", "base.receive", "receive0"));
    boost::this_thread::sleep(sleepTime);
    worker.sendMessage(Message("ssss", "__tempi__", "addNode", "base.print", "print0"));
    std::cout << "main: waiting for thread" << std::endl;
    std::cout << "main: sleep." << std::endl;
    boost::this_thread::sleep(sleepTime);
    worker.stop();
    std::cout << "main: done" << std::endl;
    return 0;
}

#endif // WITH_MAIN

