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
 * The Source class.
 */
#ifndef __TEMPI_SOURCE_H__
#define __TEMPI_SOURCE_H__

#include <boost/signals2.hpp>
#include "tempi/sharedptr.h"
#include "tempi/message.h"

namespace tempi
{

/**
 * A Source is a pad that can be connected to Sink pads.
 */
class Source
{
    public:
        typedef std::tr1::shared_ptr<Source> ptr;
        typedef boost::signals2::signal<void (const Message&)> TriggeredSignal;
        Source();
        // TODO: return success
        void trigger(const Message &message);
        TriggeredSignal &getOnTriggeredSignal()
        {
            return on_triggered_signal_;
        }
    private:
         TriggeredSignal on_triggered_signal_;
};

} // end of namespace

#endif // ifndef


