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
 * The DelayNode class.
 */
#ifndef __TEMPI_BASE_DELAYNODE_H__
#define __TEMPI_BASE_DELAYNODE_H__

#include "tempi/node.h"
#include "tempi/timer.h"
#include "tempi/timeposition.h"
#include <boost/tuple/tuple.hpp>

namespace tempi {
namespace base {

/**
 * The DelayNode stores message and outputs them later after a given delay.
 */
class DelayNode : public Node
{
    public:
        DelayNode();
    protected:
        virtual void processMessage(const char *inlet, const Message &message);
        virtual void doTick();
    private:
        typedef boost::tuple<TimePosition, Message> Event;
        std::vector<Event> events_;
        Timer timer_; // only for its now() method
};

} // end of namespace
} // end of namespace

#endif // ifndef

