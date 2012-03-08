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
 * @file The Scheduler class.
 */

#ifndef __TEMPI_SCHEDULER_H__
#define __TEMPI_SCHEDULER_H__

#include "tempi/nodefactory.h"
#include "tempi/namedobjectmap.h"
#include "tempi/graph.h"
#include "tempi/message.h"
#include "tempi/sampler_region.h"
#include <boost/thread.hpp>
#include "tempi/sharedptr.h"
#include <boost/thread/locks.hpp>

namespace tempi {

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

/**
 * A Scheduler holds graphs and one can send messages to them, synchronously or asynchronously.
 */
class Scheduler
{
    public:
        typedef std::tr1::shared_ptr<Scheduler> ptr;
        Scheduler();
        /**
         * Returns if the scheduler is running or not.
         */
        virtual bool isRunning() const = 0;
//        /**
//         * Sends a message to all its graphs.
//         * See tempi::Graph::handleMessage for handled messages.
//         */
//        void sendMessage(const Message &message);
        /**
         * Create a Graph identified by a name.
         */
        void createGraph(const char *name)
            throw(BadIndexException);
        /**
         * Create a Region identified by a name.
         */
        void createRegion(const char *name)
            throw(BadIndexException);
        NodeFactory::ptr getFactory() const;

        /**
         * Return the list of all the Graph names.
         */
        std::vector<std::string> listGraphs() const;
        /**
         * Return the list of all the Region names.
         */
        std::vector<std::string> listRegions() const;
        /**
         * Retrieves a Region.
         */
        sampler::Region::ptr getRegion(const char *name) const
            throw(BadIndexException);
        /**
         * Retrieves a Graph.
         */
        Graph::ptr getGraph(const char *name) const
            throw(BadIndexException);
        /**
         * Deletes a Graph.
         */
        void removeGraph(const char *name)
            throw(BadIndexException);
        /**
         * Deletes a Region.
         */
        void removeRegion(const char *name)
            throw(BadIndexException);
        /**
         * Acquires a lock for a mutex and returns a scoped lock, which
         * frees that mutex when deleted.
         * Might block forever if a lock has already been acquired.
         * Users are responsible for acquiring a lock before altering graphs, and the like.
         */
        ScopedLock::ptr acquireLock()
        {
            return doAcquireLock();
        }
        //virtual bool canGetGraphPtr() const
        //{
        //    return true;
        //}
        ///**
        // * Same as canGetGraphPtr, but prints an error message if the user did not lock the mutex.
        // */
        //bool makeSureLockIsAcquired() const;
        /**
         * Checks if a given named Graph exists.
         */
        bool hasGraph(const char *name) const;
        /**
         * Checks if a given named Region exists.
         */
        bool hasRegion(const char *name) const;
        void setFactory(NodeFactory::ptr factory)
        {
            factory_ = factory;
        }
    protected:
        //bool sendToAllGraphs(const Message &message);
        bool tickGraphs();
        virtual ScopedLock::ptr doAcquireLock() = 0;
    private:
        // TODO: rename this
        //virtual void doSendMessage(const Message &message) = 0;
        NamedObjectMap<Graph> graphs_;
        NamedObjectMap<sampler::Region> regions_;
        NodeFactory::ptr factory_;
};

std::ostream &operator<<(std::ostream &os, const Scheduler &scheduler);

} // end of namespace

#endif // ifndef

