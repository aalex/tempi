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
 * The Inlet class.
 */
#ifndef __TEMPI_INLET_H__
#define __TEMPI_INLET_H__

#include <boost/signals2.hpp>
#include <map>
#include "tempi/sharedptr.h"
#include "tempi/message.h"
#include "tempi/outlet.h"
#include <string>

namespace tempi
{

/**
 * A Inlet is a pad to which we can connect Outlet pads.
 */
class Inlet
{
    public:
        typedef std::tr1::shared_ptr<Inlet> ptr;
        // TODO: rename to ReceiveSignal
        typedef boost::signals2::signal<void (Inlet *sink, const Message&)> TriggeredSignal;
        // TODO: rename to ReceiveSlot
        typedef TriggeredSignal::slot_function_type TriggeredSlot;
        Inlet(const char *name, const char *documentation="");
        ~Inlet();
        /**
         * Connects this given Outlet to this Inlet.
         * @param source The Outlet to connect from.
         */
        bool connect(Outlet::ptr source);
        /**
         * Disconnects this given Outlet from this Inlet.
         * @param source The Outlet to disconnect from.
         */
        bool disconnect(Outlet::ptr source);
        /**
         * Checks if the given Outlet is connected to this Inlet.
         * @param source The Outlet to check if it is connected to this Inlet.
         */
        bool isConnected(Outlet::ptr source);
        // TODO: rename to receive
        /**
         * Called when a message is receive via this Inlet.
         */
        void trigger(const Message &message);
        TriggeredSignal &getOnTriggeredSignal()
        {
            return on_triggered_signal_;
        }
        /**
         * Disconnects all Outlets connected to this Inlet.
         */
        void disconnectAll();
        /**
         * Returns the name of this Inlet.
         * Each Node should take care of naming its inlets.
         */
        std::string getName() const;
        /**
         * Returns the documentation string for this Inlet.
         * Each Node should take care of documenting its inlets.
         */
        std::string getDocumentation() const;
    private:
        //typedef std::map<Outlet::ptr, std::tr1::shared_ptr<boost::signals2::scoped_connection> > OutletMap;
        //OutletMap sources_;
        std::string name_;
        std::string documentation_;
        typedef std::vector<Outlet::ptr> OutletsVec;
        OutletsVec sources_;
        TriggeredSignal on_triggered_signal_;
};

} // end of namespace

#endif // ifndef

