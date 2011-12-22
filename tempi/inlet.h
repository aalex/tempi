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
 * The Inlet class.
 */
#ifndef __TEMPI_INLET_H__
#define __TEMPI_INLET_H__

#include <boost/signals2.hpp>
#include <map>
#include "tempi/sharedptr.h"
#include "tempi/message.h"
#include "tempi/outlet.h"

namespace tempi
{

/**
 * A Inlet is a pad to which we can connect Outlet pads.
 * TODO: rename to Destination
 */
class Inlet
{
    public:
        typedef std::tr1::shared_ptr<Inlet> ptr;
        // TODO: rename to ReceiveSignal
        typedef boost::signals2::signal<void (Inlet *sink, const Message&)> TriggeredSignal;
        // TODO: rename to ReceiveSlot
        typedef TriggeredSignal::slot_function_type TriggeredSlot;
        Inlet();
        bool connect(Outlet::ptr source);
        bool disconnect(Outlet::ptr source);
        bool isConnected(Outlet::ptr source);
        // TODO: rename to receive
        void trigger(const Message &message);
        TriggeredSignal &getOnTriggeredSignal()
        {
            return on_triggered_signal_;
        }
        void disconnectAll();
    private:
        //typedef std::map<Outlet::ptr, std::tr1::shared_ptr<boost::signals2::scoped_connection> > OutletMap;
        //OutletMap sources_;
        typedef std::vector<Outlet::ptr> OutletsVec;
        OutletsVec sources_;
        TriggeredSignal on_triggered_signal_;
};

} // end of namespace

#endif // ifndef

