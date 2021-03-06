/*
 * Copyright (C) 2011 Alexandre Quessy
 *
 * This file is part of Tempi.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software ither version 3 of the License, or
 * (at your option) any later version.
 * 
 * Tempi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with Tempi.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

/**
 * @file The SynchronousScheduler class.
 */

#ifndef __TEMPI_SYNCHRONOUSSCHEDULER_H__
#define __TEMPI_SYNCHRONOUSSCHEDULER_H__

#include "tempi/scheduler.h"
#include <tr1/memory>

namespace tempi {

class SynchronousScheduler : public Scheduler
{
    public:
        typedef std::tr1::shared_ptr<SynchronousScheduler> ptr;
        virtual bool isRunning() const;
        bool tick();
    protected:
        virtual ScopedLock::ptr doAcquireLock()
        {
            return ScopedLock::ptr(new ScopedLock(mutex_));
        }
    private:
        boost::mutex mutex_; // FIXME: useless!!!
};

} // end of namespace

#endif // ifndef

