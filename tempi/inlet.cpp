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

#include "tempi/inlet.h"
#include "tempi/outlet.h"
#include <iostream>

namespace tempi {

Inlet::Inlet(const char *name, const char *short_documentation,
    const char * long_documentation) :
        Pad(name, short_documentation, long_documentation)
{
}

Inlet::~Inlet()
{
    // just in case.
    this->disconnectAll();
}

bool Inlet::connect(Outlet::ptr source)
{
    if (! this->isConnected(source))
    {
        this->sources_.push_back(source);
        source.get()->getOnTriggeredSignal().connect(boost::bind(&Inlet::trigger, this, _1));
        return true;
    }
    return false;
}

void Inlet::disconnectAll()
{
    on_triggered_signal_.disconnect_all_slots();
    sources_.clear();
}

bool Inlet::disconnect(Outlet::ptr source)
{
    if (isConnected(source))
    {
        source.get()->getOnTriggeredSignal().disconnect(boost::bind(&Inlet::trigger, this, _1));
        sources_.erase(std::find(sources_.begin(), sources_.end(), source));
        return true;
    }
    else
        return false;
}

bool Inlet::isConnected(Outlet::ptr source)
{
    return std::find(sources_.begin(), sources_.end(), source) != sources_.end();
}

} // end of namespace

