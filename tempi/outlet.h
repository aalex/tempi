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
 * The Outlet class.
 */
#ifndef __TEMPI_OUTLET_H__
#define __TEMPI_OUTLET_H__

#include <boost/signals2.hpp>
#include "tempi/sharedptr.h"
#include "tempi/message.h"
#include <string>

namespace tempi
{

/**
 * A Outlet is a pad that can be connected to Inlet pads.
 */
class Outlet
{
    public:
        typedef std::tr1::shared_ptr<Outlet> ptr;
        typedef boost::signals2::signal<void (const Message&)> TriggeredSignal;
        Outlet(const char *name, const char *documentation);
        // TODO: return success
        void trigger(const Message &message);
        TriggeredSignal &getOnTriggeredSignal()
        {
            return on_triggered_signal_;
        }
        std::string getName() const;
        std::string getDocumentation() const;
    private:
         TriggeredSignal on_triggered_signal_;
         std::string name_;
         std::string documentation_;
};

} // end of namespace

#endif // ifndef


