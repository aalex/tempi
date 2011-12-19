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

#include "tempi/sampler/recorder.h"
#include <iostream>

namespace tempi {
namespace sampler {

Recorder::Recorder(Region::ptr region)
{
    region_ = region;
}

void Recorder::setRegion(Region::ptr region)
{
    region_ = region;
}

Region::ptr Recorder::getRegion()
{
    return region_;
}

void Recorder::start()
{
    timer_.reset();
    //setPosition(0L);
}

void Recorder::start(TimePosition position)
{
    start();
    setPosition(position);
}

void Recorder::stop()
{
    TimePosition end = timer_.elapsed();
    if (region_->getDuration() < timer_.elapsed())
        region_->setDuration(timer_.elapsed());
}

bool Recorder::setPosition(TimePosition position)
{
    return timer_.setPosition(position);
}

void Recorder::add(Message value)
{
    region_->add(timer_.elapsed(), value);
}

void Recorder::clear()
{
    region_->clear();
}

} // end of namespace
} // end of namespace

