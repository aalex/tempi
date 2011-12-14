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

#include "tempi/sampler/score.h"
#include <iostream>

namespace tempi {
namespace sampler {

Score::Score()
{
    // pass
}

bool Score::addMarker(TimePosition timePosition, Identifier identifier)
{
    if (getMarker(identifier) != 0)
    {
        std::cout << "Score::" << __FUNCTION__ << ": There is already a marker with ID " << identifier << std::endl;
        return false;
    }
    markers_[timePosition] = MarkerPtr(new Marker(identifier));
    return true;
}

Marker *Score::getMarker(Identifier identifier)
{
    std::map<TimePosition, MarkerPtr>::iterator iter;
    for (iter = markers_.begin(); iter != markers_.end(); ++iter)
    {
        Marker *current = iter->second.get();
        if (current->getIdentifier() == identifier)
            return current;
    }
    return 0;
}

std::map<TimePosition, Marker::ptr> Score::getMarkers()
{
    return markers_; // FIXME: is this ok?
}

bool Score::removeMarker(Identifier identifier)
{
    std::map<TimePosition, Marker::ptr>::iterator iter;
    for (iter = markers_.begin(); iter != markers_.end(); ++iter)
    {
        Marker *current = iter->second.get();
        if (current->getIdentifier() == identifier)
        {
            markers_.erase(iter);
            return true;
        }
    }
    std::cout << "Score::" << __FUNCTION__ << ": No marker " << identifier << std::endl;
    return false;
}

Track *Score::getTrack(Identifier identifier)
{
    if (tracks_.find(identifier) != tracks_.end())
    {
        return tracks_.find(identifier)->second.get();
    }
    return 0;
}

} // end of namespace
} // end of namespace

