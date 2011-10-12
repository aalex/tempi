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
 * The Sink class.
 */
#ifndef __TEMPI_SINK_H__
#define __TEMPI_SINK_H__

#include <boost/signals2.hpp>
#include <map>
#include "tempi/sharedptr.h"
#include "tempi/message.h"
#include "tempi/source.h"

namespace tempi
{

/**
 * A Sink is a pad to which we can connect Source pads.
 * TODO: rename to Destination
 */
class Sink
{
    public:
        typedef std::tr1::shared_ptr<Sink> ptr;
        // TODO: rename to ReceiveSignal
        typedef boost::signals2::signal<void (Sink *sink, const Message&)> TriggeredSignal;
        // TODO: rename to ReceiveSlot
        typedef TriggeredSignal::slot_function_type TriggeredSlot;
        Sink();
        bool connect(Source::ptr source);
        bool disconnect(Source::ptr source);
        bool isConnected(Source::ptr source);
        // TODO: rename to receive
        void trigger(const Message &message);
        TriggeredSignal &getOnTriggeredSignal()
        {
            return on_triggered_signal_;
        }
        void disconnectAll();
    private:
        //typedef std::map<Source::ptr, std::tr1::shared_ptr<boost::signals2::scoped_connection> > SourceMap;
        //SourceMap sources_;
        typedef std::vector<Source::ptr> SourcesVec;
        SourcesVec sources_;
        TriggeredSignal on_triggered_signal_;
};

} // end of namespace

#endif // ifndef

