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
 * @file The SynchronousScheduler class.
 */

#ifndef __TEMPI_SYNCHRONOUSSCHEDULER_H__
#define __TEMPI_SYNCHRONOUSSCHEDULER_H__

#include "tempi/scheduler.h"

namespace tempi {

class SynchronousScheduler : public Scheduler
{
    public:
        virtual bool isRunning() const;
    protected:
        // Not implemented:
        virtual ScopedLock::ptr doAcquireLock();
};

} // end of namespace

#endif // ifndef

