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

#include "recorder.h"
#include <iostream>

namespace tempi
{

Recorder::Recorder(Track *track)
{
    track_ = track;
}

void Recorder::setTrack(Track *track)
{
    track_ = track;
}

Track *Recorder::getTrack()
{
    return track_;
}

void Recorder::reset()
{
    timer_.reset();
}

bool Recorder::setPosition(TimePosition position)
{
    return timer_.setPosition(position);
}

void Recorder::add(Message value)
{
    if (track_)
        track_->add(timer_.elapsed(), value);
    else
        std::cout << "This Recorder has an invalid Track. Cannot add event." << std::endl;
}

} // end of namespace

