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

#include "sink.h"
#include "source.h"
#include <iostream>

namespace tempi
{

Sink::Sink()
{
    // pass
}

bool Sink::connect(std::tr1::shared_ptr<Source>  source)
{
    if (! hasSource(source.get()))
    {
        sources_.push_back(source);
        source.get()->on_triggered_signal_.connect(boost::bind(&Sink::onTriggered, this, _1, _2));
        return true;
    }
    return false;
}

bool Sink::hasSource(Source *source)
{
    typename std::vector<std::tr1::shared_ptr<Source> >::iterator iter;
    for (iter = sources_.begin(); iter != sources_.end(); ++iter)
    {
        if ((*iter).get() == source)
        {
            return true;
        }
    }
    return false;
}

void Sink::onTriggered(Source *source, boost::any data)
{
    // TODO
    //std::cout << __FUNCTION__ << std::endl;
    on_triggered_signal_(source, data);
}

} // end of namespace

