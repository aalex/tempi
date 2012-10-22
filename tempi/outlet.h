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
 * The Outlet class.
 */
#ifndef __TEMPI_OUTLET_H__
#define __TEMPI_OUTLET_H__

#include <boost/signals2.hpp>
#include "tempi/sharedptr.h"
#include "tempi/message.h"
#include "tempi/pad.h"
#include <string>

namespace tempi {

/**
 * A Outlet is a pad that can be connected to Inlet pads.
 */
class Outlet : public Pad
{
    public:
        typedef std::tr1::shared_ptr<Outlet> ptr;
        Outlet(const char *name, const char *short_documentation="",
            const char *long_documentation="");
};

} // end of namespace

#endif // ifndef


