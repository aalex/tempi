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
#include "tempi/pad.h"
#include <string>

namespace tempi {

/**
 * A Inlet is a pad to which we can connect Outlet pads.
 */
class Inlet : public Pad
{
    public:
        typedef std::tr1::shared_ptr<Inlet> ptr;
        Inlet(const char *name, const char *short_documentation="",
            const char *long_documentation="");
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
        /**
         * Disconnects all Outlets connected to this Inlet.
         */
        void disconnectAll();
    private:
        typedef std::vector<Outlet::ptr> OutletsVec;
        OutletsVec sources_;
        void onMessageReceivedFromSource(Pad *outlet, const Message &message);
};

} // end of namespace

#endif // ifndef

